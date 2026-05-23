#include "utils.h"
#include "stdbool.h"
#include "string.h"

// -------------------------------------------------------------------------------------
// Hashmap
// -------------------------------------------------------------------------------------
unsigned int hashString(char* name) {
	unsigned int output = 1;

	int i = 0;

	while (name[i]) {
		output += name[i] * name[i-1 + (i == 0)];
		++i;
	}

	return output % HASH_MAP_SIZE;
}


// -------------------------------------------------------------------------------------
// File utils
// -------------------------------------------------------------------------------------
void doForEachFileInFolder(char* directoryPath, void (*func)(int, struct dirent*)) {
	struct dirent* directoryEntry;

	DIR* dir = opendir(directoryPath);

	if (dir == NULL) {
		printf("Could not open requested directory [%s]\n", directoryPath);
		return;
	}
	

	for (int i = 0; (directoryEntry = readdir(dir)) != NULL; i++) {
		func(i, directoryEntry);
	}

	closedir(dir);
}


// -------------------------------------------------------------------------------------
// Math
// -------------------------------------------------------------------------------------
float min(float a, float b) {
	return ((a < b) * a) + ((a >= b) * b);
}

float max(float a, float b) {
	return ((a > b) * a) + ((a <= b) * b);
}

float boolToSign(bool input) {
	return (input * 2) - 1;
}


//------------------------------------------------------------------------------------
// Asset loading
//------------------------------------------------------------------------------------
GenHashmapDefinition(Texture2D, TextureMap) 
TextureMap textures = {0};

void loadSprite(int index, struct dirent* entry) {
	if (index < 2) {
		return;
	}
	
	// build texture path
	char texturePath[500];	
	snprintf(texturePath, 500, "./resources/textures/%s", entry->d_name);
		
	// build identifier
	char identifier[500];
	int copyLength = strlen(entry->d_name)-4;
	strncpy(identifier, entry->d_name, 255);
	identifier[copyLength] = 0;

	printf("Loading sprite [%s] from path [%s] \n", identifier, texturePath);

	HashMapPut(textures, identifier, LoadTexture(texturePath)); 

}

void loadAssets() {
	doForEachFileInFolder("./resources/textures/", &loadSprite);

}


Texture2D* getSprite(char* spriteName) {
	return &HashMapGet(textures, spriteName);
}

// -------------------------------------------------------------------------------------
// Texture drawing
// -------------------------------------------------------------------------------------
#define MAX_TEXTURE_LAYERS 8
#define MAX_TEXTURES_PER_LAYER 1024

typedef struct {
	Texture2D* texture;
	Rectangle source;
	Rectangle destination;
	Vector2 origin;
	float rotation;
	Color color;
} DrawArguments;

void drawSpriteFromArguments(DrawArguments* arguments) {
	DrawTexturePro(
		*arguments->texture,
		arguments->source,
		arguments->destination,
		arguments->origin,
		arguments->rotation,
		arguments->color
	);
}


GenArrayDefinition(DrawArguments, MAX_TEXTURES_PER_LAYER,DrawArgumentArray)
DrawArgumentArray drawQueue[MAX_TEXTURE_LAYERS] = {0};




void sprMain(char* spriteName, float x, float y, float width, float height, bool flipX, bool flipY, float rotation, Color color, int layer) {
	DrawArgumentArray* target = &drawQueue[layer];
	
	Texture2D* texture = getSprite(spriteName); 

	DrawArguments args = {
		texture, 
		(Rectangle){0, 0, texture->width * boolToSign(!flipX), texture->height * boolToSign(!flipY)}, 
		(Rectangle){x, y, texture->width * width, texture->height * height},
		(Vector2) {texture->width / 2.0f * width, texture->height / 2.0f * height},
		rotation / PI * 180, 
		color
	};

	ArrayPush((*target), args);
}


void spr(char* spriteName, float x, float y, int layer) {
	sprMain(spriteName, x, y, 1.0f, 1.0f, false, false, 0.0f, WHITE, layer);
}

void sprFRC(char* spriteName, float x, float y, bool flipX, bool flipY, float rotation, Color color, int layer) {
	sprMain(spriteName, x, y, 1.0f, 1.0f, flipX, flipY, rotation, color, layer);
}

// -------------------------------------------------------------------------------------
// Rendering
// -------------------------------------------------------------------------------------
int worldWidth;
int worldHeight;
int windowWidth;
int windowHeight;
bool toggleFullscreen = false;
RenderTexture2D renderTexture;
float scaleFactor;
float renderWidth;
float renderHeight;
Shader shader;

void recalculateRenderingValues() {

	scaleFactor = min((float)windowHeight / worldHeight, (float) windowWidth / worldWidth);
	renderWidth = renderTexture.texture.width * scaleFactor;
	renderHeight = renderTexture.texture.height * scaleFactor;
	
}

// Initializes a new window. Wraps a few raylib functions to handle screen scaling.
void InitTextureWindow(int newWindowWidth, int newWindowHeight, int newWorldWidth, int newWorldHeight, char* windowTitle) {
	worldWidth = newWorldWidth;
	worldHeight = newWorldHeight;
	windowWidth = newWindowWidth;
	windowHeight = newWindowHeight;

	InitWindow(windowWidth, windowHeight, windowTitle);
	
	renderTexture = LoadRenderTexture(worldWidth, worldHeight);


	recalculateRenderingValues();
	
	// load assets
	loadAssets();
	shader = LoadShader(0, 0);
}


void Render() {
	// render to texture
	BeginTextureMode(renderTexture);
	ClearBackground(BLACK);	
	for (int layerIndex = 0; layerIndex < MAX_TEXTURE_LAYERS; layerIndex++) {
		DrawArgumentArray* layer = &drawQueue[layerIndex];
		for (unsigned int i = 0; i < layer->count; i++) {
			DrawArguments* args = &ArrayGet((*layer), i);

			drawSpriteFromArguments(args);

		}

		layer->count = 0;
	}

	EndTextureMode();

	// render to screen
	BeginDrawing();
	
	ClearBackground(BLACK);
	BeginShaderMode(shader);

	DrawTexturePro(renderTexture.texture,
		(Rectangle){ 0, 0, (float)renderTexture.texture.width, (float)-renderTexture.texture.height },
		(Rectangle){ windowWidth / 2.0, windowHeight / 2.0, renderWidth, renderHeight },
		(Vector2){ renderWidth / 2.0 , renderHeight / 2.0 }, 0, WHITE);
	
	EndShaderMode();

	EndDrawing();


	// This nonsense is here to make sure that wayland updates the window properly after toggling fullscreen
	if (toggleFullscreen) {
		toggleFullscreen = false;
		SetWindowSize(1,1);
		SetWindowSize(windowWidth, windowHeight);
	}
}

// changes the current window resolution
// if fullscreen is true changes to monitor resolution instead and switches fullscreen on
void SwitchResolution(int width, int height, bool fullscreen) {
	

	int targetResolutionWidth = width;
	int targetResolutionHeight = height;

	if (fullscreen) {
		int monitor = GetCurrentMonitor();
		targetResolutionWidth = GetMonitorWidth(monitor);
		targetResolutionHeight = GetMonitorHeight(monitor);
	}

	if (windowWidth == targetResolutionWidth && windowHeight == targetResolutionHeight && fullscreen == IsWindowFullscreen()) {
		return; // exit early if resolution is the same
	}

	// change resolution
	printf("changing resolution from [%d, %d] to [%d, %d]\n", windowWidth, windowHeight, targetResolutionWidth, targetResolutionHeight);
	SetWindowSize(targetResolutionWidth, targetResolutionHeight);
	
	// update rendering variables
	windowWidth = targetResolutionWidth;
	windowHeight = targetResolutionHeight;
	recalculateRenderingValues();

	if ((fullscreen && !IsWindowFullscreen()) || (!fullscreen && IsWindowFullscreen())) {
		ToggleFullscreen();
		SetWindowSize(targetResolutionWidth, targetResolutionHeight);
		toggleFullscreen = true;	
	}

}


void UseShader(char* vertexPath, char* fragmentPath){
	
	printf("Using shader [vertex : %s] [fragment : %s]\n", vertexPath, fragmentPath);	
	shader = LoadShader(vertexPath, fragmentPath);

}

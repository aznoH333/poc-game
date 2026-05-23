#include "utils.h"
#include "stdbool.h"


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

	shader = LoadShader(0, 0);
}


void BeginTextureRendering() {
	BeginTextureMode(renderTexture);
}

void EndTextureRendering() {
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


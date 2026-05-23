#include "raylib.h"
#include "utils.h"
#include "string.h"


//------------------------------------------------------------------------------------
// Sprite loading
//------------------------------------------------------------------------------------
GenHashmapDefinition(Texture2D, TextureMap) 
TextureMap textures = {0};

void loadSprite(int index, struct dirent* entry) {
	if (index < 2) {
		return;
	}
	
	// build texture path
	char texturePath[128];	
	snprintf(texturePath, 128, "./resources/textures/%s", entry->d_name);
		
	// build identifier
	char identifier[128];
	int copyLength = strlen(entry->d_name)-4;
	strncpy(identifier, entry->d_name, copyLength);
	identifier[copyLength] = 0;

	printf("Loading sprite [%s] from path [%s] \n", identifier, texturePath);

	HashMapPut(textures, identifier, LoadTexture(texturePath)); 

}

void loadSprites() {
	doForEachFileInFolder("./resources/textures/", &loadSprite);

}


Texture2D* getSprite(char* spriteName) {
	return &HashMapGet(textures, spriteName);
}



//------------------------------------------------------------------------------------
// Sprite drawing
//------------------------------------------------------------------------------------
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


void spr(char* spriteName, float x, float y, float width, float height, bool flipX, bool flipY, int layer) {
	DrawArgumentArray* target = &drawQueue[layer];
	
	Texture2D* texture = getSprite(spriteName); 

	DrawArguments args = {
		texture, 
		(Rectangle){0, 0, texture->width * -1, texture->height}, 
		(Rectangle){x, y, texture->width * width, texture->height * height},
		(Vector2) {texture->width / 2.0f * width, texture->height / 2.0f * height},
		0.0f, WHITE
	};

	ArrayPush((*target), args);
}

void renderTextures() {
	for (int layerIndex = 0; layerIndex < MAX_TEXTURE_LAYERS; layerIndex++) {
		DrawArgumentArray* layer = &drawQueue[layerIndex];
		for (int i = 0; i < layer->count; i++) {
			DrawArguments* args = &ArrayGet((*layer), i);

			drawSpriteFromArguments(args);

		}

		layer->count = 0;
	}
}


//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main(void)
{
	SetTraceLogLevel(LOG_WARNING); 
    const int screenWidth = 800;
    const int screenHeight = 400;


	InitTextureWindow(screenWidth, screenHeight, 256, 224, "window test");	
	loadSprites();
	UseShader("./resources/shaders/shaderVert.vs", "./resources/shaders/shaderFrag.fs");
    SetTargetFPS(60);

	int poo = 0;
    // Main game loop
    while (!WindowShouldClose())
	{
        // Update
        //----------------------------------------------------------------------------------
        // TODO: Update your variables here
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        //BeginDrawing();

		if (IsKeyDown(KEY_L)) {
			SwitchResolution(0, 0, true);
		}

		spr("debug_0001", 100, 10, 2, 1, false, false, 0);
		spr("debug_0001", 100, 26, 2, 1, true, false, 0);
		BeginTextureRendering();

		ClearBackground(BLUE);
        DrawText("Congrats! You created your first window!", 190, 200, 20, LIGHTGRAY);
		renderTextures();
		
		EndTextureRendering();


        //EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}

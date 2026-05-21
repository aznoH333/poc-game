#include "raylib.h"

int worldWidth;
int worldHeight;
int windowWidth;
int windowHeight;
RenderTexture2D renderTexture;
// Initializes a new window. Wraps a few raylib functions to handle screen scaling.
void InitTextureWindow(int newWindowWidth, int newWindowHeight, int newWorldWidth, int newWorldHeight, char* windowTitle) {
	worldWidth = newWorldWidth;
	worldHeight = newWorldHeight;
	windowWidth = newWindowWidth;
	windowHeight = newWindowHeight;

	InitWindow(windowWidth, windowHeight, windowTitle);
	
	renderTexture = LoadRenderTexture(worldWidth, worldHeight);
}


void BeginTextureRendering() {
	BeginTextureMode(renderTexture);
}

void EndTextureRendering() {
	EndTextureMode();

	// render to screen
	BeginDrawing();
	ClearBackground(BLACK);

	float worldRatio = worldWidth / worldHeight;
	float renderScale = worldWidth / windowWidth;
		
	DrawTexturePro(renderTexture.texture,
		(Rectangle){ 0, 0, (float)renderTexture.texture.width, (float)-renderTexture.texture.height },
		(Rectangle){ windowWidth / 2.0, windowHeight / 2.0, worldWidth * renderScale, worldHeight * renderScale },
		(Vector2){ renderTexture.texture.width/2.0f, renderTexture.texture.height/2.0f }, 0, WHITE);
	EndDrawing();
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

	SetWindowSize(targetResolutionWidth, targetResolutionHeight);
	windowWidth = targetResolutionWidth;
	windowHeight = targetResolutionHeight;

	if ((fullscreen && !IsWindowFullscreen()) || !fullscreen && IsWindowFullscreen()) {
		ToggleFullscreen();
	}
}



//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------

int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 450;

    // InitWindow(screenWidth, screenHeight, "raylib [core] example - basic window");

	InitTextureWindow(screenWidth, screenHeight, 600, 400, "window test");

    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        // TODO: Update your variables here
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        //BeginDrawing();

		if (IsKeyDown(KEY_A)) {
			SwitchResolution(200, 200, false);
		}
		if (IsKeyDown(KEY_S)) {
			SwitchResolution(800, 450, false);
		}
		if (IsKeyDown(KEY_D)) {
			SwitchResolution(0, 0, true);
		}



		BeginTextureRendering();

		ClearBackground(BLUE);
        DrawText("Congrats! You created your first window!", 190, 200, 20, LIGHTGRAY);

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

#include "raylib.h"
#include "utils.h"



float min(float a, float b) {
	return ((a < b) * a) + ((a >= b) * b);
}


int worldWidth;
int worldHeight;
int windowWidth;
int windowHeight;
bool toggleFullscreen = false;
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


	float scaleFactor = min((float)windowHeight / worldHeight, (float) windowWidth / worldWidth);

	float renderWidth = renderTexture.texture.width * scaleFactor;
	float renderHeight = renderTexture.texture.height * scaleFactor;


	DrawTexturePro(renderTexture.texture,
		(Rectangle){ 0, 0, (float)renderTexture.texture.width, (float)-renderTexture.texture.height },
		(Rectangle){ windowWidth / 2.0, windowHeight / 2.0, renderWidth, renderHeight },
		(Vector2){ renderWidth / 2.0 , renderHeight / 2.0 }, 0, WHITE);
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


	printf("changing resolution from [%d, %d] to [%d, %d]\n", windowWidth, windowHeight, targetResolutionWidth, targetResolutionHeight);
	SetWindowSize(targetResolutionWidth, targetResolutionHeight);
	windowWidth = targetResolutionWidth;
	windowHeight = targetResolutionHeight;


	if ((fullscreen && !IsWindowFullscreen()) || !fullscreen && IsWindowFullscreen()) {
		ToggleFullscreen();
		SetWindowSize(targetResolutionWidth, targetResolutionHeight);
		toggleFullscreen = true;	
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
    const int screenHeight = 400;

    // InitWindow(screenWidth, screenHeight, "raylib [core] example - basic window");

	InitTextureWindow(screenWidth, screenHeight, 600, 400, "window test");

    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------
	int peam = 1000;
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
		if (IsKeyDown(KEY_F)) {
			SwitchResolution(800, peam, false);
			peam--;
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

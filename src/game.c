#include "raylib.h"
#include "utils.h"
#include "string.h"

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main(void)
{
	SetTraceLogLevel(LOG_WARNING); 
    const int screenWidth = 800;
    const int screenHeight = 400;

	InitTextureWindow(screenWidth, screenHeight, 256, 224, "window test");	
	UseShader("./resources/shaders/shaderVert.vs", "./resources/shaders/shaderFrag.fs");
    SetTargetFPS(60);
	float poo = 0.0f;
    // Main game loop
    while (!WindowShouldClose())
	{
		
		if (IsKeyPressed(KEY_SPACE)) {
			playSound("debug", 1.0f, 1.0f);	
		}

		if (IsKeyDown(KEY_L)) {
			SwitchResolution(0, 0, true);
		}
		poo += 0.1f;

		sprMain("debug_0003", 100 + poo, 10, 2, 1, false, false, 0.0f, WHITE, 0);
		sprMain("debug_0003", 100, 26, 2, 1, false, false, 3.14f, WHITE, 0);
		spr("debug_0002", 70, 40, 0);
		sprFRC("debug_0002", 200, 100, true, false, 1.67f, WHITE, 0);
		drawText("penis", 100, 100, 10, WHITE);
		Render();

    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}

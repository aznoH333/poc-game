#include "raylib.h"
#include "utils.h"

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------

int main(void)
{
    

	//SetTraceLogLevel(LOG_WARNING); 
	// Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 400;


	InitTextureWindow(screenWidth, screenHeight, 256, 224, "window test");	
	UseShader("./resources/shaderVert.vs", "./resources/shaderFrag.fs");
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

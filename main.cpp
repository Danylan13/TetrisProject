#include "raylib.h"
#include "Game.h"

int main()
{
    // Window dimensions
    const int screenWidth = 300;
    const int screenHeight = 600;

    InitWindow(screenWidth, screenHeight, "Tetris");
    SetTargetFPS(60);

    Game game = Game();

    while (!WindowShouldClose())
    {
        // Input
        game.HandleInput();

        // Update (Game logic will go here later)

        // Drawing
        BeginDrawing();
        ClearBackground(DARKBLUE);
        game.Draw();
        EndDrawing();
    }

    CloseWindow();
    return 0;
}

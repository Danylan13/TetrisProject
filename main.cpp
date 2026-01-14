#include "raylib.h"
#include "Game.h"
#include <iostream>
#include <cstdio>

double lastUpdateTime = 0;

bool EventTriggered(double interval)
{
    double currentTime = GetTime();
    if (currentTime - lastUpdateTime >= interval)
    {
        lastUpdateTime = currentTime;
        return true;
    }
    return false;
}

int main()
{
    const int screenWidth = 500;
    const int screenHeight = 620;

    // Modern Dark Theme Colors
    Color background = {20, 20, 25, 255};
    Color uiBox = {35, 35, 45, 255};

    InitWindow(screenWidth, screenHeight, "Tetris");
    SetTargetFPS(60);

    Game game = Game();

    while (!WindowShouldClose())
    {
        UpdateMusicStream(game.music);
        game.HandleInput();

        // Speed increases with level
        double moveInterval = 0.5 / (1.0 + (game.level - 1) * 0.2);
        if (EventTriggered(moveInterval))
        {
            game.MoveBlockDown();
        }

        BeginDrawing();
        ClearBackground(background);

        // 1. Draw UI Background Elements FIRST
        // Score Box
        DrawText("SCORE", 340, 20, 20, LIGHTGRAY);
        DrawRectangleRounded({320, 45, 170, 50}, 0.2, 6, uiBox);

        // High Score Box
        DrawText("BEST", 340, 110, 20, LIGHTGRAY);
        DrawRectangleRounded({320, 135, 170, 50}, 0.2, 6, uiBox);

        // Next Block Box
        DrawText("NEXT", 340, 200, 20, LIGHTGRAY);
        DrawRectangleRounded({320, 225, 170, 140}, 0.2, 6, uiBox);

        // Level Box
        DrawText("LEVEL", 340, 380, 20, LIGHTGRAY);
        DrawRectangleRounded({320, 405, 170, 50}, 0.2, 6, uiBox);

        // 2. Draw Game Content (Grid, Blocks, and NEXT block)
        // This ensures blocks are drawn OVER the UI boxes if there's any overlap
        game.Draw();

        // 3. Draw Text and Overlays LAST
        char scoreText[10];
        sprintf(scoreText, "%d", game.score);
        int scoreWidth = MeasureText(scoreText, 25);
        DrawText(scoreText, 320 + (170 - scoreWidth) / 2, 57, 25, WHITE);

        char highscoreText[10];
        sprintf(highscoreText, "%d", game.highscore);
        int highscoreWidth = MeasureText(highscoreText, 25);
        DrawText(highscoreText, 320 + (170 - highscoreWidth) / 2, 147, 25, GOLD);

        char levelText[10];
        sprintf(levelText, "%d", game.level);
        int levelWidth = MeasureText(levelText, 25);
        DrawText(levelText, 320 + (170 - levelWidth) / 2, 417, 25, SKYBLUE);

        // Controls Info
        DrawText("ARROWS: Move", 330, 500, 15, GRAY);
        DrawText("UP: Rotate", 330, 520, 15, GRAY);
        DrawText("SPACE: Drop", 330, 540, 15, GRAY);
        DrawText("P: Pause", 330, 560, 15, GRAY);

        if (game.paused)
        {
            DrawRectangle(0, 0, screenWidth, screenHeight, Fade(BLACK, 0.5f));
            DrawText("PAUSED", 150, screenHeight / 2 - 20, 40, WHITE);
        }

        if (game.gameOver)
        {
            DrawRectangle(0, 0, screenWidth, screenHeight, Fade(BLACK, 0.8f));
            DrawText("GAME OVER", 110, screenHeight / 2 - 40, 40, RED);
            DrawText("Press any key", 140, screenHeight / 2 + 10, 20, WHITE);
        }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}

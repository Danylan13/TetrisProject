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

enum GameState { MENU, PLAYING };

int main()
{
    int screenWidth = 520;
    const int screenHeight = 750;

    Color background = {20, 20, 25, 255};
    Color uiBox = {35, 35, 45, 255};

    InitWindow(screenWidth, screenHeight, "Tetris");
    SetTargetFPS(60);
    SetExitKey(0); // Disable ESC closing the window

    Game game = Game();
    GameState state = MENU;

    while (!WindowShouldClose())
    {
        if (state == MENU) {
            if (IsKeyPressed(KEY_ONE)) {
                game.isMultiplayer = false;
                game.isAIActive = false;
                screenWidth = 520; // Single player width
                SetWindowSize(screenWidth, screenHeight);
                game.Reset();
                state = PLAYING;
            }
            if (IsKeyPressed(KEY_TWO)) {
                game.isMultiplayer = true;
                game.isAIActive = false;
                screenWidth = 1020; // Multiplayer width
                SetWindowSize(screenWidth, screenHeight);
                game.Reset();
                state = PLAYING;
            }
            if (IsKeyPressed(KEY_THREE)) {
                game.isMultiplayer = false;
                game.isAIActive = true;
                screenWidth = 1020; // AI mode width
                SetWindowSize(screenWidth, screenHeight);
                game.Reset();
                state = PLAYING;
            }

            BeginDrawing();
            ClearBackground(background);
            DrawText("TETRIS", screenWidth/2 - MeasureText("TETRIS", 60)/2, 150, 60, WHITE);
            DrawText("1. Single Player", screenWidth/2 - 100, 300, 20, LIGHTGRAY);
            DrawText("2. Local Multiplayer (WASD)", screenWidth/2 - 100, 350, 20, LIGHTGRAY);
            DrawText("3. Play vs AI", screenWidth/2 - 100, 400, 20, LIGHTGRAY);
            EndDrawing();
            continue;
        }

        // Exit to menu with ESC
        if (IsKeyPressed(KEY_ESCAPE)) {
            state = MENU;
            screenWidth = 520;
            SetWindowSize(screenWidth, screenHeight);
            continue;
        }

        UpdateMusicStream(game.music);
        game.HandleInput();

        if (game.isAIActive) {
            game.UpdateAI();
        }

        double moveInterval = 0.5 / (1.0 + (game.level[0] - 1) * 0.2);
        if (EventTriggered(moveInterval))
        {
            game.MoveBlockDown(0);
            if (game.isMultiplayer || game.isAIActive) {
                game.MoveBlockDown(1);
            }
        }

        BeginDrawing();
        ClearBackground(background);

        // Adjusted offsets to fit within the new screen width
        int offsetsX[2] = {11, 511};
        int count = (game.isMultiplayer || game.isAIActive) ? 2 : 1;

        for (int i = 0; i < count; i++) {
            int ox = offsetsX[i];
            DrawText(TextFormat("PLAYER %d", i+1), ox + 340, 20, 20, (i==0?SKYBLUE:GOLD));

            DrawText("SCORE", ox + 340, 50, 15, LIGHTGRAY);
            DrawRectangleRounded({(float)ox + 320, 70, 170, 40}, 0.2, 6, uiBox);

            char scoreText[10];
            sprintf(scoreText, "%d", game.score[i]);
            int scoreWidth = MeasureText(scoreText, 20);
            DrawText(scoreText, ox + 320 + (170 - scoreWidth) / 2, 80, 20, WHITE);

            DrawText("NEXT", ox + 340, 200, 20, LIGHTGRAY);
            DrawRectangleRounded({(float)ox + 320, 225, 170, 140}, 0.2, 6, uiBox);

            DrawText("LEVEL", ox + 340, 380, 20, LIGHTGRAY);
            DrawRectangleRounded({(float)ox + 320, 405, 170, 50}, 0.2, 6, uiBox);

            char levelText[10];
            sprintf(levelText, "%d", game.level[i]);
            int levelWidth = MeasureText(levelText, 25);
            DrawText(levelText, ox + 320 + (170 - levelWidth) / 2, 417, 25, SKYBLUE);

            DrawText("HOLD", ox + 340, 470, 20, LIGHTGRAY);
            DrawRectangleRounded({(float)ox + 320, 495, 170, 140}, 0.2, 6, uiBox);

            // Instructions for each player
            int instrY = 650;
            if (i == 0) {
                DrawText("P1: ARROWS to Move/Rotate", ox + 10, instrY, 15, GRAY);
                DrawText("SPACE: Drop | C: Hold", ox + 10, instrY + 20, 15, GRAY);
            } else if (game.isMultiplayer) {
                DrawText("P2: WASD to Move/Rotate", ox + 10, instrY, 15, GRAY);
                DrawText("F: Drop | R: Hold", ox + 10, instrY + 20, 15, GRAY);
            } else if (game.isAIActive) {
                DrawText("AI PLAYER ACTIVE", ox + 10, instrY, 15, GOLD);
            }
        }

        // General instructions
        DrawText("P: Pause | ESC: Menu", screenWidth/2 - 80, screenHeight - 30, 15, LIGHTGRAY);

        game.Draw();

        if (game.paused)
        {
            DrawRectangle(0, 0, screenWidth, screenHeight, Fade(BLACK, 0.5f));
            DrawText("PAUSED", screenWidth/2 - 70, screenHeight / 2 - 20, 40, WHITE);
        }

        if (game.gameOver)
        {
            DrawRectangle(0, 0, screenWidth, screenHeight, Fade(BLACK, 0.8f));
            DrawText("GAME OVER", screenWidth/2 - 110, screenHeight / 2 - 40, 40, RED);
            DrawText("Press any key to restart", screenWidth/2 - 120, screenHeight / 2 + 10, 20, WHITE);
        }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}

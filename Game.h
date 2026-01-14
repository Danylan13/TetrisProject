#ifndef GAME_H_INCLUDED
#define GAME_H_INCLUDED

#pragma once
#include "Grid.h"
#include "Blocks.h"
#include <vector>

class Game
{
public:
    Game();
    ~Game();
    void Draw();
    void HandleInput();
    void MoveBlockLeft(int playerIdx);
    void MoveBlockRight(int playerIdx);
    void MoveBlockDown(int playerIdx);
    void LockBlock(int playerIdx);
    void Reset();
    void RotateBlock(int playerIdx);
    void HoldBlock(int playerIdx);
    bool BlockMovementValid(int playerIdx, int rows, int columns);
    bool BlockFits(int playerIdx);

    // AI Methods
    void UpdateAI();
    float EvaluateBoard(const Grid& tempGrid);
    struct AIMove {
        int rotation;
        int column;
        float score;
    };
    AIMove GetBestMove();

    bool gameOver;
    bool paused;
    bool isMultiplayer;
    bool isAIActive;

    int score[2];
    int highscore;
    int linesClearedTotal[2];
    int level[2];
    Music music;

private:
    Grid grid[2];
    Sound rotateSound;
    Sound clearSound;
    std::vector<Block> blocks[2];
    Block currentBlock[2];
    Block nextBlock[2];
    Block holdBlock[2];
    bool canHold[2];
    bool hasHoldBlock[2];

    Block GetRandomBlock(int playerIdx);
    std::vector<Block> GetAllBlocks();
    void UpdateScore(int playerIdx, int linesCleared, int moveDownPoints);
    void DrawGhostBlock(int playerIdx);
    void SaveHighScore();
    void LoadHighScore();

    // AI specific
    double lastAIMoveTime;
    int targetColumn;
    int targetRotation;
};

#endif // GAME_H_INCLUDED

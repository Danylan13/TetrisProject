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
    void MoveBlockLeft();
    void MoveBlockRight();
    void MoveBlockDown();
    void LockBlock();
    void Reset();
    void RotateBlock();
    bool BlockMovementValid(int rows, int columns);
    bool BlockFits();

    bool gameOver;
    bool paused;
    int score;
    int highscore;
    int linesClearedTotal;
    int level;
    Music music;

private:
    Grid grid;
    Sound rotateSound;
    Sound clearSound;
    std::vector<Block> blocks;
    Block currentBlock;
    Block nextBlock;

    Block GetRandomBlock();
    std::vector<Block> GetAllBlocks();
    void UpdateScore(int linesCleared, int moveDownPoints);
    void DrawGhostBlock();
    void SaveHighScore();
    void LoadHighScore();
};

#endif // GAME_H_INCLUDED

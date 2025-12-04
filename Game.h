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
    void Draw();
    void HandleInput();
    void MoveBlockLeft();
    void MoveBlockRight();
    void MoveBlockDown();
    void LockBlock();
    void ResetBlock();
    void RotateBlock();
    bool BlockMovementValid(int rows, int columns);
    bool BlockFits();

private:
    Grid grid;
    std::vector<Block> blocks;
    Block currentBlock;
    Block nextBlock;
    Block GetRandomBlock();
    std::vector<Block> GetAllBlocks();
};

#endif // GAME_H_INCLUDED

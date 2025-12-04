#ifndef BLOCK_H_INCLUDED
#define BLOCK_H_INCLUDED

#pragma once

#include <vector>
#include <map>
#include "Position.h"
#include <raylib.h>

class Block
{
public:
    Block();
    void Draw();
    void Move(int rows, int columns);
    std::vector<Position> GetCellPositions();
    void Rotate();
    void UndoRotation();

    int id;
    std::map<int, std::vector<Position>> cells;

private:
    int cellSize;
    int rotationState;
    int rowOffset;
    int columnOffset;
};

#endif // BLOCK_H_INCLUDED

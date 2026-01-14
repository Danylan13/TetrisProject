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
    void Draw(int offsetX, int offsetY);
    void DrawPreview(int offsetX, int offsetY);
    void Move(int rows, int columns);
    std::vector<Position> GetCellPositions();
    void Rotate();
    void UndoRotation();

    int id;
    std::map<int, std::vector<Position>> cells;
    std::vector<Color> colors;

private:
    int cellSize;
    int rotationState;
    int rowOffset;
    int columnOffset;
    void FillColors();
};

#endif // BLOCK_H_INCLUDED

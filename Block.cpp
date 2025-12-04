#include "Block.h"
#include <iostream>

Block::Block()
{
    cellSize = 30;
    rotationState = 0;
    rowOffset = 0;
    columnOffset = 0;
}

void Block::Draw()
{
    std::vector<Position> tilePositions = GetCellPositions();
    for (Position item : tilePositions)
    {
        DrawRectangle(item.column * cellSize + 1, item.row * cellSize + 1, cellSize - 1, cellSize - 1, RED); // Using RED temporarily
    }
}

void Block::Move(int rows, int columns)
{
    rowOffset += rows;
    columnOffset += columns;
}

std::vector<Position> Block::GetCellPositions()
{
    std::vector<Position> tiles = cells[rotationState];
    std::vector<Position> movedTiles;
    for (Position item : tiles)
    {
        Position newPos = {item.row + rowOffset, item.column + columnOffset};
        movedTiles.push_back(newPos);
    }
    return movedTiles;
}

void Block::Rotate()
{
    rotationState++;
    if (rotationState == cells.size())
    {
        rotationState = 0;
    }
}

void Block::UndoRotation()
{
    rotationState--;
    if (rotationState == -1)
    {
        rotationState = cells.size() - 1;
    }
}

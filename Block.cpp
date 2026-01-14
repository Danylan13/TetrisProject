#include "Block.h"
#include <iostream>

Block::Block()
{
    cellSize = 30;
    rotationState = 0;
    rowOffset = 0;
    columnOffset = 0;
    FillColors();
}

void Block::FillColors()
{
    colors.push_back({20, 20, 25, 255});    // 0: Empty
    colors.push_back({0, 240, 240, 255});   // 1: I-Block (Cyan)
    colors.push_back({0, 0, 240, 255});     // 2: J-Block (Blue)
    colors.push_back({240, 160, 0, 255});   // 3: L-Block (Orange)
    colors.push_back({240, 240, 0, 255});   // 4: O-Block (Yellow)
    colors.push_back({0, 240, 0, 255});     // 5: S-Block (Green)
    colors.push_back({160, 0, 240, 255});   // 6: T-Block (Purple)
    colors.push_back({240, 0, 0, 255});     // 7: Z-Block (Red)
}

void Block::Draw(int offsetX, int offsetY)
{
    std::vector<Position> tilePositions = GetCellPositions();
    for (Position item : tilePositions)
    {
        DrawRectangle(item.column * cellSize + offsetX, item.row * cellSize + offsetY, cellSize - 1, cellSize - 1, colors[id]);
        DrawRectangle(item.column * cellSize + offsetX, item.row * cellSize + offsetY, cellSize - 1, 2, Fade(WHITE, 0.3f));
        DrawRectangle(item.column * cellSize + offsetX, item.row * cellSize + offsetY, 2, cellSize - 1, Fade(WHITE, 0.3f));
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

void Block::DrawPreview(int offsetX, int offsetY)
{
    std::vector<Position> tilePositions = cells[0];
    for (Position item : tilePositions)
    {
        DrawRectangle(item.column * cellSize + offsetX, item.row * cellSize + offsetY, cellSize - 1, cellSize - 1, colors[id]);
        DrawRectangle(item.column * cellSize + offsetX, item.row * cellSize + offsetY, cellSize - 1, 2, Fade(WHITE, 0.3f));
        DrawRectangle(item.column * cellSize + offsetX, item.row * cellSize + offsetY, 2, cellSize - 1, Fade(WHITE, 0.3f));
    }
}

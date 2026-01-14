#include "Grid.h"
#include <iostream>
#include <raylib.h>

Grid::Grid()
{
    numRows = 20;
    numCols = 10;
    cellSize = 30;
    Initialize();
    FillColors();
}

void Grid::Initialize()
{
    for (int r = 0; r < numRows; r++)
    {
        for (int c = 0; c < numCols; c++)
        {
            grid[r][c] = 0;
        }
    }
}

void Grid::Print()
{
    for (int r = 0; r < numRows; r++)
    {
        for (int c = 0; c < numCols; c++)
        {
            std::cout << grid[r][c] << " ";
        }
        std::cout << std::endl;
    }
}

void Grid::Draw()
{
    for (int r = 0; r < numRows; r++)
    {
        for (int c = 0; c < numCols; c++)
        {
            int cellValue = grid[r][c];
            // Draw grid background
            DrawRectangle(c * cellSize + 11, r * cellSize + 11, cellSize - 1, cellSize - 1, {30, 30, 40, 255});

            if (cellValue != 0) {
                DrawRectangle(c * cellSize + 11, r * cellSize + 11, cellSize - 1, cellSize - 1, colors[cellValue]);
                // Add a small highlight for 3D effect
                DrawRectangle(c * cellSize + 11, r * cellSize + 11, cellSize - 1, 2, Fade(WHITE, 0.3f));
                DrawRectangle(c * cellSize + 11, r * cellSize + 11, 2, cellSize - 1, Fade(WHITE, 0.3f));
            }
        }
    }
}

void Grid::FillColors()
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

bool Grid::IsCellOutside(int row, int column)
{
    if (row >= 0 && row < numRows && column >= 0 && column < numCols)
    {
        return false;
    }
    return true;
}

bool Grid::IsCellEmpty(int row, int column)
{
    if (grid[row][column] == 0)
    {
        return true;
    }
    return false;
}

void Grid::SetCellValue(int row, int column, int value)
{
    if (row >= 0 && row < numRows && column >= 0 && column < numCols)
    {
        grid[row][column] = value;
    }
}

bool Grid::IsRowFull(int row)
{
    for (int c = 0; c < numCols; c++)
    {
        if (grid[row][c] == 0)
        {
            return false;
        }
    }
    return true;
}

void Grid::ClearRow(int row)
{
    for (int c = 0; c < numCols; c++)
    {
        grid[row][c] = 0;
    }
}

void Grid::MoveRowDown(int row, int numRows)
{
    for (int c = 0; c < numCols; c++)
    {
        grid[row + numRows][c] = grid[row][c];
        grid[row][c] = 0;
    }
}

int Grid::ClearFullRows()
{
    int cleared = 0;
    for (int r = numRows - 1; r >= 0; r--)
    {
        if (IsRowFull(r))
        {
            ClearRow(r);
            cleared++;
        }
        else if (cleared > 0)
        {
            MoveRowDown(r, cleared);
        }
    }
    return cleared;
}

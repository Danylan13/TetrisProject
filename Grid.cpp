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
            DrawRectangle(c * cellSize + 1, r * cellSize + 1, cellSize - 1, cellSize - 1, colors[cellValue]);
        }
    }
}

void Grid::FillColors()
{
    colors.push_back(BLACK); // 0: Empty
    colors.push_back(RED);   // 1: I-Block
    colors.push_back(BLUE);  // 2: J-Block
    colors.push_back(GREEN); // 3: L-Block
    colors.push_back(YELLOW);// 4: O-Block
    colors.push_back(PURPLE);// 5: S-Block
    colors.push_back(ORANGE);// 6: T-Block
    colors.push_back(PINK);  // 7: Z-Block
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

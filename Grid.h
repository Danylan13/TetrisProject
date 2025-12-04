#ifndef GRID_H_INCLUDED
#define GRID_H_INCLUDED

#pragma once

#include <vector>
#include <raylib.h>

class Grid
{
public:
    Grid();
    void Initialize();
    void Print();
    void Draw();
    bool IsCellOutside(int row, int column);
    bool IsCellEmpty(int row, int column);
    void SetCellValue(int row, int column, int value);
    bool IsRowFull(int row);
    void ClearRow(int row);
    void MoveRowDown(int row, int numRows);
    int ClearFullRows();
    int grid[20][10];

private:
    int numRows;
    int numCols;
    int cellSize;
    std::vector<Color> colors;
    void FillColors();
};

#endif // GRID_H_INCLUDED

#include "Game.h"
#include <random>
#include <algorithm>

Game::Game()
{
    blocks = GetAllBlocks();
    currentBlock = GetRandomBlock();
    nextBlock = GetRandomBlock();
}

void Game::Draw()
{
    grid.Draw();
    currentBlock.Draw();
}

Block Game::GetRandomBlock()
{
    if (blocks.empty())
    {
        blocks = GetAllBlocks();
    }
    int randomIndex = rand() % blocks.size();
    Block block = blocks[randomIndex];
    blocks.erase(blocks.begin() + randomIndex);
    return block;
}

std::vector<Block> Game::GetAllBlocks()
{
    return {IBlock(), JBlock(), LBlock(), OBlock(), SBlock(), TBlock(), ZBlock()};
}

bool Game::BlockFits()
{
    std::vector<Position> tiles = currentBlock.GetCellPositions();
    for (Position item : tiles)
    {
        if (grid.IsCellOutside(item.row, item.column) || !grid.IsCellEmpty(item.row, item.column))
        {
            return false;
        }
    }
    return true;
}

bool Game::BlockMovementValid(int rows, int columns)
{
    currentBlock.Move(rows, columns);
    bool fits = BlockFits();
    currentBlock.Move(-rows, -columns); // Revert the move
    return fits;
}

void Game::MoveBlockLeft()
{
    if (BlockMovementValid(0, -1))
    {
        currentBlock.Move(0, -1);
    }
}

void Game::MoveBlockRight()
{
    if (BlockMovementValid(0, 1))
    {
        currentBlock.Move(0, 1);
    }
}

void Game::MoveBlockDown()
{
    if (BlockMovementValid(1, 0))
    {
        currentBlock.Move(1, 0);
    }
    else
    {
        LockBlock();
    }
}

void Game::HandleInput()
{
    int keyPressed = GetKeyPressed();
    if (keyPressed != 0)
    {
        switch (keyPressed)
        {
        case KEY_LEFT:
            MoveBlockLeft();
            break;
        case KEY_RIGHT:
            MoveBlockRight();
            break;
        case KEY_DOWN:
            MoveBlockDown();
            break;
        case KEY_UP:
            RotateBlock();
            break;
        default:
            break;
        }
    }
}

void Game::RotateBlock()
{
    currentBlock.Rotate();
    if (BlockFits() == false)
    {
        currentBlock.UndoRotation();
    }
}

void Game::LockBlock()
{
    std::vector<Position> tiles = currentBlock.GetCellPositions();
    for (Position item : tiles)
    {
        grid.SetCellValue(item.row, item.column, currentBlock.id);
    }
    grid.ClearFullRows();
    ResetBlock();
}

void Game::ResetBlock()
{
    currentBlock = nextBlock;
    nextBlock = GetRandomBlock();
}

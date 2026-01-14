#include "Game.h"
#include <random>
#include <algorithm>
#include <fstream>

Game::Game()
{
    grid = Grid();
    blocks = GetAllBlocks();
    currentBlock = GetRandomBlock();
    nextBlock = GetRandomBlock();
    gameOver = false;
    paused = false;
    score = 0;
    linesClearedTotal = 0;
    level = 1;
    LoadHighScore();

    InitAudioDevice();
    music = LoadMusicStream("Sounds/music.mp3");
    PlayMusicStream(music);
    rotateSound = LoadSound("Sounds/rotate.mp3");
    clearSound = LoadSound("Sounds/clear.mp3");
}

Game::~Game()
{
    UnloadSound(rotateSound);
    UnloadSound(clearSound);
    UnloadMusicStream(music);
    CloseAudioDevice();
}

void Game::Draw()
{
    grid.Draw();
    if (!gameOver && !paused) {
        DrawGhostBlock();
    }
    currentBlock.Draw(11, 11);

    // Draw next block in the preview area
    // The box is at {320, 225, 170, 140} in main.cpp
    // We center the block inside this box
    switch (nextBlock.id)
    {
    case 1: // IBlock
        nextBlock.DrawPreview(335, 255);
        break;
    case 4: // OBlock
        nextBlock.DrawPreview(350, 265);
        break;
    default:
        nextBlock.DrawPreview(350, 255);
        break;
    }
}

void Game::DrawGhostBlock()
{
    Block ghostBlock = currentBlock;
    int offset = 0;
    while (BlockMovementValid(offset + 1, 0))
    {
        offset++;
    }

    std::vector<Position> tiles = ghostBlock.GetCellPositions();
    for (Position item : tiles)
    {
        DrawRectangle((item.column) * 30 + 11, (item.row + offset) * 30 + 11, 29, 29, Fade(ghostBlock.colors[ghostBlock.id], 0.3f));
    }
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
    currentBlock.Move(-rows, -columns);
    return fits;
}

void Game::MoveBlockLeft()
{
    if (!gameOver && !paused && BlockMovementValid(0, -1))
    {
        currentBlock.Move(0, -1);
    }
}

void Game::MoveBlockRight()
{
    if (!gameOver && !paused && BlockMovementValid(0, 1))
    {
        currentBlock.Move(0, 1);
    }
}

void Game::MoveBlockDown()
{
    if (gameOver || paused) return;

    if (BlockMovementValid(1, 0))
    {
        currentBlock.Move(1, 0);
        UpdateScore(0, 1);
    }
    else
    {
        LockBlock();
    }
}

void Game::HandleInput()
{
    int keyPressed = GetKeyPressed();

    if (keyPressed == KEY_P) {
        paused = !paused;
    }

    if (gameOver && keyPressed != 0)
    {
        Reset();
    }

    if (!paused && keyPressed != 0)
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
        case KEY_SPACE:
            // Hard drop
            while (BlockMovementValid(1, 0)) {
                currentBlock.Move(1, 0);
                UpdateScore(0, 2);
            }
            LockBlock();
            break;
        default:
            break;
        }
    }
}

void Game::RotateBlock()
{
    if (gameOver || paused) return;

    currentBlock.Rotate();
    if (BlockFits() == false)
    {
        currentBlock.UndoRotation();
    }
    else
    {
        PlaySound(rotateSound);
    }
}

void Game::LockBlock()
{
    std::vector<Position> tiles = currentBlock.GetCellPositions();
    for (Position item : tiles)
    {
        grid.SetCellValue(item.row, item.column, currentBlock.id);
    }
    int rowsCleared = grid.ClearFullRows();
    if (rowsCleared > 0)
    {
        PlaySound(clearSound);
    }
    UpdateScore(rowsCleared, 0);

    currentBlock = nextBlock;
    if (BlockFits() == false)
    {
        gameOver = true;
        if (score > highscore) {
            highscore = score;
            SaveHighScore();
        }
    }
    nextBlock = GetRandomBlock();
}

void Game::Reset()
{
    grid.Initialize();
    blocks = GetAllBlocks();
    currentBlock = GetRandomBlock();
    nextBlock = GetRandomBlock();
    score = 0;
    linesClearedTotal = 0;
    level = 1;
    gameOver = false;
    paused = false;
}

void Game::UpdateScore(int linesCleared, int moveDownPoints)
{
    switch (linesCleared)
    {
    case 1:
        score += 100 * level;
        break;
    case 2:
        score += 300 * level;
        break;
    case 3:
        score += 500 * level;
        break;
    case 4:
        score += 800 * level;
        break;
    default:
        break;
    }
    score += moveDownPoints;

    linesClearedTotal += linesCleared;
    level = 1 + (linesClearedTotal / 10);
}

void Game::SaveHighScore()
{
    std::ofstream file("highscore.txt");
    if (file.is_open()) {
        file << highscore;
        file.close();
    }
}

void Game::LoadHighScore()
{
    std::ifstream file("highscore.txt");
    if (file.is_open()) {
        file >> highscore;
        file.close();
    } else {
        highscore = 0;
    }
}

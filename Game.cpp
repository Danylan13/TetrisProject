#include "Game.h"
#include <random>
#include <algorithm>
#include <fstream>

Game::Game()
{
    for (int i = 0; i < 2; i++) {
        grid[i] = Grid();
        blocks[i] = GetAllBlocks();
        currentBlock[i] = GetRandomBlock(i);
        nextBlock[i] = GetRandomBlock(i);
        score[i] = 0;
        linesClearedTotal[i] = 0;
        level[i] = 1;
        canHold[i] = true;
        hasHoldBlock[i] = false;
    }

    gameOver = false;
    paused = false;
    isMultiplayer = false;
    isAIActive = false;
    lastAIMoveTime = 0;
    targetColumn = -1;
    targetRotation = -1;

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
    int offsetsX[2];
    if (isMultiplayer || isAIActive) {
        offsetsX[0] = 11;
        offsetsX[1] = 511;
    } else {
        offsetsX[0] = 11;
        offsetsX[1] = 0;
    }

    int count = (isMultiplayer || isAIActive) ? 2 : 1;

    for (int i = 0; i < count; i++) {
        grid[i].Draw(offsetsX[i], 11);
        if (!gameOver && !paused) {
            DrawGhostBlock(i);
        }
        currentBlock[i].Draw(offsetsX[i], 11);

        // Draw next block preview
        int nextX = offsetsX[i] + 320;
        int nextY = 225;

        int drawX = nextX + 50;
        int drawY = nextY + 45;
        if (nextBlock[i].id == 1) { // IBlock
            drawX = nextX + 35;
            drawY = nextY + 45;
        } else if (nextBlock[i].id == 4) { // OBlock
            drawX = nextX + 50;
            drawY = nextY + 55;
        }
        nextBlock[i].DrawPreview(drawX, drawY);

        // Draw hold block preview
        if (hasHoldBlock[i]) {
            int holdX = offsetsX[i] + 320;
            int holdY = 495;

            // Centering logic for HOLD block
            int hDrawX = holdX + 50;
            int hDrawY = holdY + 45;
            if (holdBlock[i].id == 1) { // IBlock
                hDrawX = holdX + 35;
                hDrawY = holdY + 45;
            } else if (holdBlock[i].id == 4) { // OBlock
                hDrawX = holdX + 50;
                hDrawY = holdY + 55;
            }
            holdBlock[i].DrawPreview(hDrawX, hDrawY);
        }
    }
}

void Game::DrawGhostBlock(int playerIdx)
{
    int offsetX = (isMultiplayer || isAIActive) ? ((playerIdx == 0) ? 11 : 511) : 11;
    Block ghostBlock = currentBlock[playerIdx];
    int offset = 0;
    while (BlockMovementValid(playerIdx, offset + 1, 0))
    {
        offset++;
    }

    std::vector<Position> tiles = ghostBlock.GetCellPositions();
    for (Position item : tiles)
    {
        DrawRectangle((item.column) * 30 + offsetX, (item.row + offset) * 30 + 11, 29, 29, Fade(ghostBlock.colors[ghostBlock.id], 0.3f));
    }
}

Block Game::GetRandomBlock(int playerIdx)
{
    if (blocks[playerIdx].empty())
    {
        blocks[playerIdx] = GetAllBlocks();
    }
    int randomIndex = rand() % blocks[playerIdx].size();
    Block block = blocks[playerIdx][randomIndex];
    blocks[playerIdx].erase(blocks[playerIdx].begin() + randomIndex);
    return block;
}

std::vector<Block> Game::GetAllBlocks()
{
    return {IBlock(), JBlock(), LBlock(), OBlock(), SBlock(), TBlock(), ZBlock()};
}

bool Game::BlockFits(int playerIdx)
{
    std::vector<Position> tiles = currentBlock[playerIdx].GetCellPositions();
    for (Position item : tiles)
    {
        if (grid[playerIdx].IsCellOutside(item.row, item.column) || !grid[playerIdx].IsCellEmpty(item.row, item.column))
        {
            return false;
        }
    }
    return true;
}

bool Game::BlockMovementValid(int playerIdx, int rows, int columns)
{
    currentBlock[playerIdx].Move(rows, columns);
    bool fits = BlockFits(playerIdx);
    currentBlock[playerIdx].Move(-rows, -columns);
    return fits;
}

void Game::MoveBlockLeft(int playerIdx)
{
    if (!gameOver && !paused && BlockMovementValid(playerIdx, 0, -1))
    {
        currentBlock[playerIdx].Move(0, -1);
    }
}

void Game::MoveBlockRight(int playerIdx)
{
    if (!gameOver && !paused && BlockMovementValid(playerIdx, 0, 1))
    {
        currentBlock[playerIdx].Move(0, 1);
    }
}

void Game::MoveBlockDown(int playerIdx)
{
    if (gameOver || paused) return;

    if (BlockMovementValid(playerIdx, 1, 0))
    {
        currentBlock[playerIdx].Move(1, 0);
        UpdateScore(playerIdx, 0, 1);
    }
    else
    {
        LockBlock(playerIdx);
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

    if (!paused)
    {
        // Player 1 Controls
        if (IsKeyPressed(KEY_LEFT)) MoveBlockLeft(0);
        if (IsKeyPressed(KEY_RIGHT)) MoveBlockRight(0);
        if (IsKeyPressed(KEY_DOWN)) MoveBlockDown(0);
        if (IsKeyPressed(KEY_UP)) RotateBlock(0);
        if (IsKeyPressed(KEY_C)) HoldBlock(0);
        if (IsKeyPressed(KEY_SPACE)) {
            while (BlockMovementValid(0, 1, 0)) {
                currentBlock[0].Move(1, 0);
                UpdateScore(0, 0, 2);
            }
            LockBlock(0);
        }

        // Player 2 Controls (only if multiplayer)
        if (isMultiplayer) {
            if (IsKeyPressed(KEY_A)) MoveBlockLeft(1);
            if (IsKeyPressed(KEY_D)) MoveBlockRight(1);
            if (IsKeyPressed(KEY_S)) MoveBlockDown(1);
            if (IsKeyPressed(KEY_W)) RotateBlock(1);
            if (IsKeyPressed(KEY_R)) HoldBlock(1);
            if (IsKeyPressed(KEY_F)) {
                while (BlockMovementValid(1, 1, 0)) {
                    currentBlock[1].Move(1, 0);
                    UpdateScore(1, 0, 2);
                }
                LockBlock(1);
            }
        }
    }
}

void Game::RotateBlock(int playerIdx)
{
    if (gameOver || paused) return;

    currentBlock[playerIdx].Rotate();
    if (BlockFits(playerIdx) == false)
    {
        currentBlock[playerIdx].UndoRotation();
    }
    else
    {
        PlaySound(rotateSound);
    }
}

void Game::LockBlock(int playerIdx)
{
    std::vector<Position> tiles = currentBlock[playerIdx].GetCellPositions();
    for (Position item : tiles)
    {
        grid[playerIdx].SetCellValue(item.row, item.column, currentBlock[playerIdx].id);
    }
    int rowsCleared = grid[playerIdx].ClearFullRows();
    if (rowsCleared > 0)
    {
        PlaySound(clearSound);
    }
    UpdateScore(playerIdx, rowsCleared, 0);

    currentBlock[playerIdx] = nextBlock[playerIdx];
    canHold[playerIdx] = true;
    if (BlockFits(playerIdx) == false)
    {
        gameOver = true;
        if (score[0] > highscore) {
            highscore = score[0];
            SaveHighScore();
        }
        if (isMultiplayer && score[1] > highscore) {
            highscore = score[1];
            SaveHighScore();
        }
    }
    nextBlock[playerIdx] = GetRandomBlock(playerIdx);

    if (playerIdx == 1 && isAIActive) {
        targetColumn = -1;
        targetRotation = -1;
    }
}

void Game::HoldBlock(int playerIdx)
{
    if (!canHold[playerIdx] || gameOver || paused) return;

    Block temp;
    bool hadHold = hasHoldBlock[playerIdx];

    if (!hadHold) {
        holdBlock[playerIdx] = currentBlock[playerIdx];
        holdBlock[playerIdx].Move(-holdBlock[playerIdx].GetCellPositions()[0].row + 0, -holdBlock[playerIdx].GetCellPositions()[0].column + 0);

        currentBlock[playerIdx] = nextBlock[playerIdx];
        nextBlock[playerIdx] = GetRandomBlock(playerIdx);
        hasHoldBlock[playerIdx] = true;
    } else {
        temp = currentBlock[playerIdx];
        currentBlock[playerIdx] = holdBlock[playerIdx];
        currentBlock[playerIdx].Move(-currentBlock[playerIdx].GetCellPositions()[0].row + 0, -currentBlock[playerIdx].GetCellPositions()[0].column + 3);
        holdBlock[playerIdx] = temp;
    }
    canHold[playerIdx] = false;

    if (playerIdx == 1 && isAIActive) {
        targetColumn = -1;
        targetRotation = -1;
    }
}

void Game::Reset()
{
    for (int i = 0; i < 2; i++) {
        grid[i].Initialize();
        blocks[i] = GetAllBlocks();
        currentBlock[i] = GetRandomBlock(i);
        nextBlock[i] = GetRandomBlock(i);
        score[i] = 0;
        linesClearedTotal[i] = 0;
        level[i] = 1;
        canHold[i] = true;
        hasHoldBlock[i] = false;
    }
    gameOver = false;
    paused = false;
    targetColumn = -1;
    targetRotation = -1;
}

void Game::UpdateScore(int playerIdx, int linesCleared, int moveDownPoints)
{
    int basePoints = 0;
    switch (linesCleared)
    {
    case 1: basePoints = 100; break;
    case 2: basePoints = 300; break;
    case 3: basePoints = 500; break;
    case 4: basePoints = 800; break;
    }

    score[playerIdx] += basePoints * level[playerIdx];
    score[playerIdx] += moveDownPoints;

    linesClearedTotal[playerIdx] += linesCleared;
    level[playerIdx] = 1 + (linesClearedTotal[playerIdx] / 10);
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

// --- AI Implementation ---

float Game::EvaluateBoard(const Grid& tempGrid) {
    int aggregateHeight = 0;
    int holes = 0;
    int bumpiness = 0;
    int completeLines = 0;

    int columnHeights[10] = {0};

    for (int c = 0; c < 10; c++) {
        for (int r = 0; r < 20; r++) {
            if (tempGrid.grid[r][c] != 0) {
                columnHeights[c] = 20 - r;
                break;
            }
        }
        aggregateHeight += columnHeights[c];
    }

    for (int c = 0; c < 10; c++) {
        bool blockFound = false;
        for (int r = 0; r < 20; r++) {
            if (tempGrid.grid[r][c] != 0) blockFound = true;
            else if (blockFound && tempGrid.grid[r][c] == 0) holes++;
        }
    }

    for (int c = 0; c < 9; c++) {
        bumpiness += abs(columnHeights[c] - columnHeights[c+1]);
    }

    for (int r = 0; r < 20; r++) {
        bool full = true;
        for (int c = 0; c < 10; c++) {
            if (tempGrid.grid[r][c] == 0) {
                full = false;
                break;
            }
        }
        if (full) completeLines++;
    }

    return -0.51f * aggregateHeight + 0.76f * completeLines - 0.35f * holes - 0.18f * bumpiness;
}

Game::AIMove Game::GetBestMove() {
    AIMove bestMove = {0, 0, -1000000.0f};
    Block originalBlock = currentBlock[1];

    for (int r = 0; r < 4; r++) {
        Block testBlock = originalBlock;
        for (int i = 0; i < r; i++) testBlock.Rotate();

        for (int c = -3; c < 10; c++) {
            Block moveBlock = testBlock;
            moveBlock.Move(0, c - 3);

            bool valid = true;
            for (auto p : moveBlock.GetCellPositions()) {
                if (grid[1].IsCellOutside(p.row, p.column) || !grid[1].IsCellEmpty(p.row, p.column)) {
                    valid = false;
                    break;
                }
            }
            if (!valid) continue;

            int drop = 0;
            while (true) {
                bool canDrop = true;
                for (auto p : moveBlock.GetCellPositions()) {
                    if (grid[1].IsCellOutside(p.row + drop + 1, p.column) || !grid[1].IsCellEmpty(p.row + drop + 1, p.column)) {
                        canDrop = false;
                        break;
                    }
                }
                if (canDrop) drop++;
                else break;
            }

            Grid tempGrid = grid[1];
            for (auto p : moveBlock.GetCellPositions()) {
                tempGrid.SetCellValue(p.row + drop, p.column, moveBlock.id);
            }

            float score = EvaluateBoard(tempGrid);
            if (score > bestMove.score) {
                bestMove.score = score;
                bestMove.rotation = r;
                bestMove.column = c;
            }
        }
    }
    return bestMove;
}

void Game::UpdateAI() {
    if (!isAIActive || gameOver || paused) return;

    double currentTime = GetTime();
    if (currentTime - lastAIMoveTime < 0.25) return;
    lastAIMoveTime = currentTime;

    if (targetColumn == -1) {
        AIMove move = GetBestMove();
        targetRotation = move.rotation;
        targetColumn = move.column;
    }

    if (targetRotation > 0) {
        RotateBlock(1);
        targetRotation--;
        return;
    }

    int currentMinCol = 10;
    for (auto p : currentBlock[1].GetCellPositions()) {
        if (p.column < currentMinCol) currentMinCol = p.column;
    }

    if (currentMinCol < targetColumn) {
        MoveBlockRight(1);
    } else if (currentMinCol > targetColumn) {
        MoveBlockLeft(1);
    } else {
        if (BlockMovementValid(1, 1, 0)) {
            currentBlock[1].Move(1, 0);
        } else {
            LockBlock(1);
        }
    }
}

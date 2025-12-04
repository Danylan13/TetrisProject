#ifndef BLOCKS_H_INCLUDED
#define BLOCKS_H_INCLUDED

#pragma once
#include "Block.h"

class LBlock : public Block
{
public:
    LBlock()
    {
        id = 1;
        cells[0] = { {0, 2}, {1, 0}, {1, 1}, {1, 2} };
        cells[1] = { {0, 1}, {1, 1}, {2, 1}, {2, 2} };
        cells[2] = { {1, 0}, {1, 1}, {1, 2}, {2, 0} };
        cells[3] = { {0, 0}, {0, 1}, {1, 1}, {2, 1} };
        Move(0, 3);
    }
};

class JBlock : public Block
{
public:
    JBlock()
    {
        id = 2;
        cells[0] = { {0, 0}, {1, 0}, {1, 1}, {1, 2} };
        cells[1] = { {0, 1}, {0, 2}, {1, 1}, {2, 1} };
        cells[2] = { {1, 0}, {1, 1}, {1, 2}, {2, 2} };
        cells[3] = { {0, 1}, {1, 1}, {2, 0}, {2, 1} };
        Move(0, 3);
    }
};

class IBlock : public Block
{
public:
    IBlock()
    {
        id = 3;
        cells[0] = { {1, 0}, {1, 1}, {1, 2}, {1, 3} };
        cells[1] = { {0, 2}, {1, 2}, {2, 2}, {3, 2} };
        cells[2] = { {2, 0}, {2, 1}, {2, 2}, {2, 3} };
        cells[3] = { {0, 1}, {1, 1}, {2, 1}, {3, 1} };
        Move(-1, 3);
    }
};

class OBlock : public Block
{
public:
    OBlock()
    {
        id = 4;
        cells[0] = { {0, 0}, {0, 1}, {1, 0}, {1, 1} };
        Move(0, 4);
    }
};

class SBlock : public Block
{
public:
    SBlock()
    {
        id = 5;
        cells[0] = { {0, 1}, {0, 2}, {1, 0}, {1, 1} };
        cells[1] = { {0, 1}, {1, 1}, {1, 2}, {2, 2} };
        cells[2] = { {1, 1}, {1, 2}, {2, 0}, {2, 1} };
        cells[3] = { {0, 0}, {1, 0}, {1, 1}, {2, 1} };
        Move(0, 3);
    }
};

class TBlock : public Block
{
public:
    TBlock()
    {
        id = 6;
        cells[0] = { {0, 1}, {1, 0}, {1, 1}, {1, 2} };
        cells[1] = { {0, 1}, {1, 1}, {1, 2}, {2, 1} };
        cells[2] = { {1, 0}, {1, 1}, {1, 2}, {2, 1} };
        cells[3] = { {0, 1}, {1, 0}, {1, 1}, {2, 1} };
        Move(0, 3);
    }
};

class ZBlock : public Block
{
public:
    ZBlock()
    {
        id = 7;
        cells[0] = { {0, 0}, {0, 1}, {1, 1}, {1, 2} };
        cells[1] = { {0, 2}, {1, 1}, {1, 2}, {2, 1} };
        cells[2] = { {1, 0}, {1, 1}, {2, 1}, {2, 2} };
        cells[3] = { {0, 1}, {1, 0}, {1, 1}, {2, 0} };
        Move(0, 3);
    }
};

#endif // BLOCKS_H_INCLUDED

#pragma once
#include "CObject.h"

class CBlock :
    public CObject
{
private:
    TILE_STATE      m_TileState;

public:
    CBlock();
    ~CBlock();

    CLONE(CBlock);

public:
    TILE_STATE RandomTile();

public:
    virtual void Update();
    virtual void Render(HDC hdc);
};


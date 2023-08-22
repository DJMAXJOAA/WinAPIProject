#pragma once
#include "CObject.h"

class CAnimation;

class CTile :
    public CObject
{
private:
    TILE_STATE      m_TileState;
    CAnimation*     m_TileTexture;

public:
    CTile();
    virtual ~CTile();
   
    CLONE(CTile);

public:
    TILE_STATE GetTileState() { return m_TileState; }

public:
    TILE_STATE RandomTile();

public:
    virtual void Update();
    virtual void Render(HDC hdc);
};


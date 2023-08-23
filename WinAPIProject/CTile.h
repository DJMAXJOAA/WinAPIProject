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
    virtual void AnimationFinishEvent() override {}

public:
    TILE_STATE GetTileState() { return m_TileState; }

public:
    void SetTileState(TILE_STATE _tileState) { m_TileState = _tileState; }

public:
    TILE_STATE RandomTile();

public:
    virtual void Update();
    virtual void Render(HDC hdc);
};


#pragma once
#include "struct.h"

class CTile;

struct TileState
{
    bool    bVisited;	// BFS 방문 여부
    CTile*  pTile;		// 타일 객체
    TileState(Vec2 vPos) : bVisited(false), pTile(nullptr) {};
};
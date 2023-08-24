#pragma once
#include "struct.h"

class CTile;

struct TileState
{
    bool    bVisited;	// BFS 规巩 咯何
    CTile*  pTile;		// 鸥老 按眉
    TileState(Vec2 vPos) : bVisited(false), pTile(nullptr) {};
};
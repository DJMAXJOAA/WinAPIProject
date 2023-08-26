#pragma once
#include "struct.h"

class CTile;

struct TileState
{
    bool     bVisited;	// BFS 방문 여부
    CTile*   pTile;		// 타일 객체
    CObject* pObj;      // 타일 위의 객체
    TileState() : bVisited(false), pTile(nullptr), pObj(nullptr) {};
};
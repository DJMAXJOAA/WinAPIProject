#pragma once
#include "struct.h"

class CTile;

struct TileState
{
    bool    bVisited;	// BFS �湮 ����
    CTile*  pTile;		// Ÿ�� ��ü
    TileState(Vec2 vPos) : bVisited(false), pTile(nullptr) {};
};
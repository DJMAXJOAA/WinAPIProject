#pragma once
#include "struct.h"

class CTile;

struct TileState
{
    bool    bVisited;	// BFS �湮 ����
    CTile*  pTile;		// Ÿ�� ��ü
    TileState() : bVisited(false), pTile(nullptr) {};
};
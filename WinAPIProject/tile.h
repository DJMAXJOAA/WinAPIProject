#pragma once

class CTile;

struct TileState
{
    bool     bVisited;	// BFS �湮 ����
    CTile*   pTile;		// Ÿ�� ��ü
    CObject* pObj;      // Ÿ�� ���� ��ü
    TileState() : bVisited(false), pTile(nullptr), pObj(nullptr) {};
};

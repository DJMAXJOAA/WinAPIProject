#pragma once
#include "tile.h"

class CScene_Battle;
using namespace battle;

struct TileState
{
	bool            bVisited;	// BFS ¹æ¹® ¿©ºÎ
	CTile*			pTile;		// Å¸ÀÏ °´Ã¼
	TileState(Vec2 vPos) : bVisited(false), pTile(nullptr) {};
};

class CTileManager
{
private:
	vector<vector<TileState>> m_vecTileState;	 // Å¸ÀÏÀÇ Á¤º¸
	map<Vec2, Vec2>			  m_mapRealPoint;    // ÁÂÇ¥°è ÁÂÇ¥ -> ½ÇÁ¦ ÁÂÇ¥ (ÀÌÁß¸Ê)
	map<Vec2, Vec2>			  m_mapGridPoint;    // ½ÇÁ¦ ÁÂÇ¥ -> ÁÂÇ¥°è ÁÂÇ¥ (ÀÌÁß¸Ê)

public:
	CTileManager(CScene_Battle* _pScene);
	~CTileManager();

public:
	void TileSelectInit();

public:
	Vec2 GridToReal(Vec2 vPos) { return m_mapRealPoint[vPos]; }
	Vec2 RealToGrid(Vec2 vPos) { return m_mapGridPoint[vPos]; }

public:
	vector<vector<TileState>>& GetTileState() { return m_vecTileState; }
	map<Vec2, Vec2>& GetMapRealPoint() { return m_mapRealPoint; }
	map<Vec2, Vec2>& GetMapGridPoint() { return m_mapGridPoint; }

public:
	void SetTileState(vector<vector<TileState>> vecTiles) { m_vecTileState = vecTiles; }
};


#pragma once
#include "tile.h"

class CScene_Battle;

class CTileManager
{
private:
	vector<vector<TileState>> m_vecTileState;	 // Å¸ÀÏÀÇ Á¤º¸
	map<Vec2, Vec2>			  m_mapRealPoint;    // ÁÂÇ¥°è ÁÂÇ¥ -> ½ÇÁ¦ ÁÂÇ¥ (ÀÌÁß¸Ê)
	map<Vec2, Vec2>			  m_mapGridPoint;    // ½ÇÁ¦ ÁÂÇ¥ -> ÁÂÇ¥°è ÁÂÇ¥ (ÀÌÁß¸Ê)

public:
	CTileManager();
	~CTileManager();

public:
	void TileInit();
	void TileRandomInit();

public:
	Vec2 GridToReal(Vec2 vPos) { return m_mapRealPoint[Vec2((int)vPos.x, (int)vPos.y)]; }
	Vec2 RealToGrid(Vec2 vPos) { return m_mapGridPoint[Vec2((int)vPos.x, (int)vPos.y)]; }

public:
	vector<vector<TileState>>& GetTileState() { return m_vecTileState; }
	map<Vec2, Vec2>& GetMapRealPoint() { return m_mapRealPoint; }
	map<Vec2, Vec2>& GetMapGridPoint() { return m_mapGridPoint; }

public:
	void SetTileState(vector<vector<TileState>> vecTiles) { m_vecTileState = vecTiles; }
};


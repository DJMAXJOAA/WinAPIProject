#pragma once
#include "tile.h"

class CTileCenter
{
private:
	vector<vector<TileState>> m_vecTileState;	 // Å¸ÀÏÀÇ Á¤º¸
	map<Vec2, Vec2>			  m_mapRealPoint;    // ÁÂÇ¥°è ÁÂÇ¥ -> ½ÇÁ¦ ÁÂÇ¥ (ÀÌÁß¸Ê)
	map<Vec2, Vec2>			  m_mapGridPoint;    // ½ÇÁ¦ ÁÂÇ¥ -> ÁÂÇ¥°è ÁÂÇ¥ (ÀÌÁß¸Ê)

public:
	CTileCenter();
	~CTileCenter();

public:
	void TileInit(list<Vec2>& _tileList);
	void TileRandomInit();

public:
	Vec2 GridToReal(Vec2 _vPos) { return m_mapRealPoint[Vec2((int)_vPos.x, (int)_vPos.y)]; }
	Vec2 RealToGrid(Vec2 _vPos) { return m_mapGridPoint[Vec2((int)_vPos.x, (int)_vPos.y)]; }

public:
	map<Vec2, Vec2>& GetMapRealPoint() { return m_mapRealPoint; }
	map<Vec2, Vec2>& GetMapGridPoint() { return m_mapGridPoint; }
	vector<vector<TileState>>& GetTiles() { return m_vecTileState; }
	CTile* GetTile(Vec2 _vGridPos) { return  m_vecTileState[(int)_vGridPos.y][(int)_vGridPos.x].pTile; }
	CObject* GetObj(Vec2 _vGridPos) { return  m_vecTileState[(int)_vGridPos.y][(int)_vGridPos.x].pObj; }

public:
	bool IsVisited(Vec2 _vGridPos) { return m_vecTileState[(int)_vGridPos.y][(int)_vGridPos.x].bVisited; }

public:
	void SetTileState(vector<vector<TileState>> _vecTiles) { m_vecTileState = _vecTiles; }
	void SetTileObject(Vec2 _vGridPos, CObject* _pObj) { m_vecTileState[(int)_vGridPos.y][(int)_vGridPos.x].pObj = _pObj; }
};


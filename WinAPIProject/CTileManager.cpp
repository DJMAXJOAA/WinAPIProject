#include "pch.h"
#include "CTileManager.h"

#include "CTile.h"

CTileCenter::CTileCenter()
	: m_vecTileState(BATTLE_SETTINGS::GRID_X, vector<TileState>(BATTLE_SETTINGS::GRID_Y))
	, m_mapRealPoint{}
	, m_mapGridPoint{}
{
}

CTileCenter::~CTileCenter()
{
}

void CTileCenter::Init()
{
	for (int y = 0; y < m_vecTileState.size(); y++)
	{
		for (int x = 0; x < m_vecTileState[y].size(); x++)
		{
			m_vecTileState[y][x].pObj = nullptr;
			m_vecTileState[y][x].bVisited = false;
		}
	}
}

void CTileCenter::TileRouteInit(list<Vec2>& _tileList)
{
	for (auto& tile : _tileList)
	{
		CTile* pTile = m_vecTileState[(int)tile.y][(int)tile.x].pTile;
		pTile->SetTileState((TILE_STATE)((int)pTile->GetTileState() - 4));
	}
}

void CTileCenter::TileVisitedInit()
{
	for (int y = 0; y < m_vecTileState.size(); y++)
	{
		for (int x = 0; x < m_vecTileState[y].size(); x++)
		{
			m_vecTileState[y][x].bVisited = false;
		}
	}
}

void CTileCenter::TileRandomInit()
{
	for (int y = 0; y < m_vecTileState.size(); y++)
	{
		for (int x = 0; x < m_vecTileState[y].size(); x++)
		{
			CTile* pTile = m_vecTileState[y][x].pTile;
			if (pTile->GetTileState() == TILE_STATE::BLACK)
			{
				pTile->SetTileState(pTile->RandomTile());
			}
		}
	}
}


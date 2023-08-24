#include "pch.h"
#include "CTileManager.h"

#include "CTile.h"

CTileManager::CTileManager()
	: m_vecTileState(GRID_X, vector<TileState>(GRID_Y))
	, m_mapRealPoint{}
	, m_mapGridPoint{}
{
}

CTileManager::~CTileManager()
{
}

void CTileManager::TileInit()
{
	for (int y = 0; y < m_vecTileState.size(); y++)
	{
		for (int x = 0; x < m_vecTileState[y].size(); x++)
		{
			CTile* pTile = m_vecTileState[y][x].pTile;
			pTile->SetTileState((TILE_STATE)((int)pTile->GetTileState() - 4));
		}
	}
}

void CTileManager::TileRandomInit()
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


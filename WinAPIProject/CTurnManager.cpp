#include "pch.h"
#include "CTurnManager.h"

#include "CKeyMgr.h"

#include "CTile.h"

CLogicCenter::CLogicCenter()
	: m_lstMoveRoute{}
	, m_TileColor(TILE_STATE::BLACK)
	, m_lstTarget{}
	, m_iCombo(0)
{
}

CLogicCenter::~CLogicCenter()
{
}

void CLogicCenter::Init()
{
	m_lstMoveRoute.clear();
	m_lstTarget.clear();
}

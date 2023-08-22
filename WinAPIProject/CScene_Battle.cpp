#include "pch.h"
#include "CScene_Battle.h"

#include "CCore.h"
#include "CCollisionMgr.h"
#include "CKeyMgr.h"

#include "CPlayer.h"
#include "CTile.h"
#include "CBlock.h"
#include "CMouse.h"

CScene_Battle::CScene_Battle()
	: m_vecTileState(9, vector<int>(9,0))
	, m_mapPoint{}
{
}

CScene_Battle::~CScene_Battle()
{
}

void CScene_Battle::Update()
{
	CScene::Update();

	if (KEY_TAP(KEY::ENTER))
	{
		ChangeScene(SCENE_TYPE::TOOL);
	}
}

void CScene_Battle::Enter()
{
	Vec2 vResolution = CCore::GetInstance()->GetResolution();

	int startX = (int)(vResolution.x / 2);
	int startY = (int)(vResolution.y / 4);

	// Player �߰�
	CPlayer* pObj = new CPlayer;
	pObj->SetPos(Vec2(startX, startY - (TILE_HEIGHT / 2)));
	AddObject(pObj, GROUP_TYPE::PLAYER);

	// ���콺 �߰�
	CMouse* pMouse = new CMouse;
	AddObject(pMouse, GROUP_TYPE::MOUSE);

	for (int y = 0; y < 9; ++y) {
		for (int x = 0; x < 9; ++x) {
			int drawX = startX + (x - y) * (TILE_WIDTH / 2);
			int drawY = startY + (x + y) * (TILE_HEIGHT / 2) - (TILE_HEIGHT / 2);

			// ��ǥ ����
			Vec2 Coordinate = Vec2(x, y);
			Vec2 actualCoord = Vec2(drawX, drawY);
			m_mapPoint.insert(make_pair(Coordinate, actualCoord));

			// Ÿ�� ����
			CTile* pTile = new CTile;
			pTile->SetPos(Vec2(drawX, drawY));
			m_vecTileState[y][x] = ((int)pTile->GetTileState());
			AddObject(pTile, GROUP_TYPE::TILE);

			// Ÿ�� �� ����
			CBlock* pBlcok = new CBlock(MAP_TYPE::SNOW);
			pBlcok->SetPos(Vec2(drawX, drawY));
			AddObject(pBlcok, GROUP_TYPE::BLOCK);
		}
	}

	// Ÿ�ϰ� ���콺�� �浹ó��
	CCollisionMgr::GetInstance()->CheckGroup(GROUP_TYPE::MOUSE, GROUP_TYPE::TILE);

	CCamera::GetInstance()->SetLookAt(vResolution / 2.f);
}

void CScene_Battle::Exit()
{
	DeleteAll();
	CCollisionMgr::GetInstance()->Reset();
}

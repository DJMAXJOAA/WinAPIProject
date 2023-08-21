#include "pch.h"
#include "CScene_Battle.h"

#include "CCore.h"
#include "CCollisionMgr.h"
#include "CKeyMgr.h"

#include "CPlayer.h"
#include "CTile.h"
#include "CBlock.h"

CScene_Battle::CScene_Battle()
	: m_tileState(9, vector<int>(9,0))
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

	// Player Ãß°¡

	for (int y = 0; y < 1; ++y) {
		for (int x = 0; x < 1; ++x) {
			int drawX = startX + (x - y) * (TILE_WIDTH / 2);
			int drawY = startY + (x + y) * (TILE_HEIGHT / 2) - (TILE_HEIGHT / 2);

			CPlayer* pObj = new CPlayer;
			pObj->SetPos(Vec2(drawX, drawY));
			AddObject(pObj, GROUP_TYPE::PLAYER);

			CTile* pTile = new CTile;
			pTile->SetPos(Vec2(drawX, drawY));

			m_tileState[y][x] = ((int)pTile->GetTileState());
			AddObject(pTile, GROUP_TYPE::TILE);


			CBlock* pBlcok = new CBlock;
			pBlcok->SetPos(Vec2(drawX, drawY));
			AddObject(pBlcok, GROUP_TYPE::BLOCK);
		}
	}

	CCamera::GetInstance()->SetLookAt(vResolution / 2.f);
}

void CScene_Battle::Exit()
{
	DeleteAll();
	CCollisionMgr::GetInstance()->Reset();
}

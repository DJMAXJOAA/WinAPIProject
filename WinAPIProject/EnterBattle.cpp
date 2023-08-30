#include "pch.h"
#include "EnterBattle.h"

#include "CCore.h"
#include "CDataMgr.h"
#include "CCamera.h"
#include "CCollisionMgr.h"

#include "CScene_Battle.h"
#include "CMouse.h"
#include "CTile.h"
#include "CBlock.h"
#include "CPlayer.h"
#include "CMonster.h"
#include "CEffect.h"
#include "CBackground.h"
#include "CPanelUI_Number.h"

#include "BFSSearch.h"
#include "CTurnManager.h"
#include "CTileManager.h"
#include "CMonsterSpawner.h"

#include "FieldData.h"

static std::random_device rd;
static std::mt19937 gen(rd());

void EnterBattle::Handle(CScene_Battle* _pScene)
{
	int level = _pScene->GetDifficulty();
	FIELD_TYPE fieldType = _pScene->GetFieldType();

	CTileCenter* m_TileCenter = _pScene->GetTileCenter();
	CMonsterSpawner* m_MonsterSpawner = _pScene->GetSpawner();
	CPlayer* m_pPlayer = _pScene->GetPlayer();

	Vec2 vResolution = CCore::GetInstance()->GetResolution();

	// 600번 대의 필드 데이터 중에 조건에 맞는 데이터들
	vector<CData*> allData = CDataMgr::GetInstance()->LoadAllData(600);
	vector<FieldData*> fieldData{};
	FieldData* randomFieldData = nullptr;

	// 랜덤하게 필드 ID 결정
	// 600번 대의 필드 데이터 중에 조건에 맞는 데이터들
	for (int i = 0; i < allData.size(); i++)
	{
		if (((FieldData*)allData[i])->m_iDifficulty == level
			&& ((FieldData*)allData[i])->m_MapType == fieldType)
			fieldData.push_back((FieldData*)allData[i]);
	}
	// 추려낸 데이터들중에 랜덤으로 1개 선택
	if (!fieldData.empty())
	{
		std::uniform_int_distribution<int> distr(0, int(fieldData.size() - 1));

		int randomIndex = distr(gen);
		randomFieldData = fieldData[randomIndex];
		_pScene->SetFieldType(randomFieldData->GetKey());
	}
	else
	{
		assert(0);	// 데이터 가져오기 실패 디버깅
	}

	// 마우스 추가
	CMouse* pMouse = new CMouse;
	CreateObj(pMouse, GROUP_TYPE::MOUSE);

	// 백그라운드 추가
	CBackground* pBackground = new CBackground(L"texture\\Background\\battle_background.png");
	CreateObj(pBackground, GROUP_TYPE::BACKGROUND);

	// 타일 + 블럭 추가
	int startX = (int)(vResolution.x / 2);
	int startY = (int)(vResolution.y / 4);
	map<Vec2, Vec2>& mapRealPoint = m_TileCenter->GetMapRealPoint();
	map<Vec2, Vec2>& mapGridPoint = m_TileCenter->GetMapGridPoint();
	vector<vector<TileState>>& vecTileState = m_TileCenter->GetTiles();

	for (int y = 0; y < 9; ++y) {
		for (int x = 0; x < 9; ++x) {
			int drawX = startX + (x - y) * (TILE_WIDTH / 2);
			int drawY = startY + (x + y) * (TILE_HEIGHT / 2) - (TILE_HEIGHT / 2);

			// 좌표 저장
			Vec2 gridPos = Vec2(x, y);
			Vec2 realPos = Vec2(drawX, drawY);

			// 이중맵으로 격자 좌표계 <-> 실제 좌표 연결
			mapRealPoint.insert(make_pair(gridPos, realPos));
			mapGridPoint.insert(make_pair(realPos, gridPos));

			// 타일 생성
			CTile* pTile = new CTile;
			pTile->SetPos(realPos);
			pTile->SetGridPos(gridPos);
			vecTileState[y][x].pTile = pTile;
			CreateObj(pTile, GROUP_TYPE::TILE);

			// 블럭 복사생성 -> 필드에 맞게
			CBlock* cBlcok = new CBlock(randomFieldData->m_BlockType);
			cBlcok->SetPos(realPos);
			cBlcok->SetGridPos(gridPos);
			CreateObj(cBlcok, GROUP_TYPE::BLOCK);
		}
	}

	// Player 추가
	CPlayer* pPlayer = new CPlayer;
	Vec2 PlayerStartPos(4, 2);
	pPlayer->SetPos(REAL(PlayerStartPos));
	pPlayer->SetGridPos(PlayerStartPos);
	_pScene->SetPlayer(pPlayer);
	m_TileCenter->SetTileObject(PlayerStartPos, pPlayer);
	CreateObj(pPlayer, GROUP_TYPE::UNIT);

	// 몬스터 생성
	m_MonsterSpawner->SpawnMonster(randomFieldData);
	list<CMonster*>& monsterList = m_MonsterSpawner->GetMonsterList();

	// 몬스터 리스트에 대한 반복자
	auto monsterIter = monsterList.begin();

	// i는 몬스터의 인덱스를 나타냅니다.
	for (int i = 0; monsterIter != monsterList.end(); )
	{
		// 몬스터들의 위치 랜덤 지정 (오브젝트 중복 안되게)
		Vec2 randomPos = GetRandomGridPos();

		if (m_TileCenter->GetObj(randomPos) == nullptr)
		{
			// 타일 위의 오브젝트가 nullptr이면, 오브젝트 좌표 설정
			(*monsterIter)->SetPos(REAL(randomPos));
			(*monsterIter)->SetGridPos(randomPos);
			m_TileCenter->SetTileObject(randomPos, *monsterIter);

			// 디버깅
			if (m_TileCenter->GetObj(randomPos) != nullptr)
			{
				cout << "타일 오브젝트 등록 " << i << "번째 :" << randomPos.x << "," << randomPos.y << ",\n";
			}

			// 다음 몬스터로 설정
			++monsterIter;
			++i;
		}
		else
		{
			// 좌표가 겹치면 반복자 증가 안하니까 다시 좌표 설정
		}
	}

	// 돈 표시 UI (임시)
	CPanelUI_Number* pNumber = new CPanelUI_Number(pPlayer->GetMoney(), Vec2(50, 650), 2.0f);
	_pScene->SetMoneyUI(pNumber);
	CreateObj(pNumber, GROUP_TYPE::UI);

	// GDI+ Test (Effect)
	//CEffect* pEffect = new CEffect;
	//pEffect->SetPos(REAL(PlayerStartPos));
	//CreateObj(pEffect, GROUP_TYPE::MISSILE_PLAYER);

	// 카메라 설정
	CCamera::GetInstance()->SetLookAt(REAL(PlayerStartPos));

	// 타일과 마우스의 충돌처리
	CCollisionMgr::GetInstance()->CheckGroup(GROUP_TYPE::MOUSE, GROUP_TYPE::TILE);
}

Vec2 EnterBattle::GetRandomGridPos()
{
	std::uniform_int_distribution<int> distr(0, 8);

	Vec2 result;
	result.x = float(distr(gen)); // x 좌표 랜덤 생성
	result.y = float(distr(gen)); // y 좌표 랜덤 생성

	return result;
}

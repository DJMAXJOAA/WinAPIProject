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

	// 600�� ���� �ʵ� ������ �߿� ���ǿ� �´� �����͵�
	vector<CData*> allData = CDataMgr::GetInstance()->LoadAllData(600);
	vector<FieldData*> fieldData{};
	FieldData* randomFieldData = nullptr;

	// �����ϰ� �ʵ� ID ����
	// 600�� ���� �ʵ� ������ �߿� ���ǿ� �´� �����͵�
	for (int i = 0; i < allData.size(); i++)
	{
		if (((FieldData*)allData[i])->m_iDifficulty == level
			&& ((FieldData*)allData[i])->m_MapType == fieldType)
			fieldData.push_back((FieldData*)allData[i]);
	}
	// �߷��� �����͵��߿� �������� 1�� ����
	if (!fieldData.empty())
	{
		std::uniform_int_distribution<int> distr(0, int(fieldData.size() - 1));

		int randomIndex = distr(gen);
		randomFieldData = fieldData[randomIndex];
		_pScene->SetFieldType(randomFieldData->GetKey());
	}
	else
	{
		assert(0);	// ������ �������� ���� �����
	}

	// ���콺 �߰�
	CMouse* pMouse = new CMouse;
	CreateObj(pMouse, GROUP_TYPE::MOUSE);

	// ��׶��� �߰�
	CBackground* pBackground = new CBackground(L"texture\\Background\\battle_background.png");
	CreateObj(pBackground, GROUP_TYPE::BACKGROUND);

	// Ÿ�� + �� �߰�
	int startX = (int)(vResolution.x / 2);
	int startY = (int)(vResolution.y / 4);
	map<Vec2, Vec2>& mapRealPoint = m_TileCenter->GetMapRealPoint();
	map<Vec2, Vec2>& mapGridPoint = m_TileCenter->GetMapGridPoint();
	vector<vector<TileState>>& vecTileState = m_TileCenter->GetTiles();

	for (int y = 0; y < 9; ++y) {
		for (int x = 0; x < 9; ++x) {
			int drawX = startX + (x - y) * (TILE_WIDTH / 2);
			int drawY = startY + (x + y) * (TILE_HEIGHT / 2) - (TILE_HEIGHT / 2);

			// ��ǥ ����
			Vec2 gridPos = Vec2(x, y);
			Vec2 realPos = Vec2(drawX, drawY);

			// ���߸����� ���� ��ǥ�� <-> ���� ��ǥ ����
			mapRealPoint.insert(make_pair(gridPos, realPos));
			mapGridPoint.insert(make_pair(realPos, gridPos));

			// Ÿ�� ����
			CTile* pTile = new CTile;
			pTile->SetPos(realPos);
			pTile->SetGridPos(gridPos);
			vecTileState[y][x].pTile = pTile;
			CreateObj(pTile, GROUP_TYPE::TILE);

			// �� ������� -> �ʵ忡 �°�
			CBlock* cBlcok = new CBlock(randomFieldData->m_BlockType);
			cBlcok->SetPos(realPos);
			cBlcok->SetGridPos(gridPos);
			CreateObj(cBlcok, GROUP_TYPE::BLOCK);
		}
	}

	// Player �߰�
	CPlayer* pPlayer = new CPlayer;
	Vec2 PlayerStartPos(4, 2);
	pPlayer->SetPos(REAL(PlayerStartPos));
	pPlayer->SetGridPos(PlayerStartPos);
	_pScene->SetPlayer(pPlayer);
	m_TileCenter->SetTileObject(PlayerStartPos, pPlayer);
	CreateObj(pPlayer, GROUP_TYPE::UNIT);

	// ���� ����
	m_MonsterSpawner->SpawnMonster(randomFieldData);
	list<CMonster*>& monsterList = m_MonsterSpawner->GetMonsterList();

	// ���� ����Ʈ�� ���� �ݺ���
	auto monsterIter = monsterList.begin();

	// i�� ������ �ε����� ��Ÿ���ϴ�.
	for (int i = 0; monsterIter != monsterList.end(); )
	{
		// ���͵��� ��ġ ���� ���� (������Ʈ �ߺ� �ȵǰ�)
		Vec2 randomPos = GetRandomGridPos();

		if (m_TileCenter->GetObj(randomPos) == nullptr)
		{
			// Ÿ�� ���� ������Ʈ�� nullptr�̸�, ������Ʈ ��ǥ ����
			(*monsterIter)->SetPos(REAL(randomPos));
			(*monsterIter)->SetGridPos(randomPos);
			m_TileCenter->SetTileObject(randomPos, *monsterIter);

			// �����
			if (m_TileCenter->GetObj(randomPos) != nullptr)
			{
				cout << "Ÿ�� ������Ʈ ��� " << i << "��° :" << randomPos.x << "," << randomPos.y << ",\n";
			}

			// ���� ���ͷ� ����
			++monsterIter;
			++i;
		}
		else
		{
			// ��ǥ�� ��ġ�� �ݺ��� ���� ���ϴϱ� �ٽ� ��ǥ ����
		}
	}

	// �� ǥ�� UI (�ӽ�)
	CPanelUI_Number* pNumber = new CPanelUI_Number(pPlayer->GetMoney(), Vec2(50, 650), 2.0f);
	_pScene->SetMoneyUI(pNumber);
	CreateObj(pNumber, GROUP_TYPE::UI);

	// GDI+ Test (Effect)
	//CEffect* pEffect = new CEffect;
	//pEffect->SetPos(REAL(PlayerStartPos));
	//CreateObj(pEffect, GROUP_TYPE::MISSILE_PLAYER);

	// ī�޶� ����
	CCamera::GetInstance()->SetLookAt(REAL(PlayerStartPos));

	// Ÿ�ϰ� ���콺�� �浹ó��
	CCollisionMgr::GetInstance()->CheckGroup(GROUP_TYPE::MOUSE, GROUP_TYPE::TILE);
}

Vec2 EnterBattle::GetRandomGridPos()
{
	std::uniform_int_distribution<int> distr(0, 8);

	Vec2 result;
	result.x = float(distr(gen)); // x ��ǥ ���� ����
	result.y = float(distr(gen)); // y ��ǥ ���� ����

	return result;
}

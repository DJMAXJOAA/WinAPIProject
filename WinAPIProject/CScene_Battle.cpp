#include "pch.h"
#include "CScene_Battle.h"

#include "CCore.h"
#include "CCollisionMgr.h"
#include "CKeyMgr.h"

#include "CPlayer.h"
#include "CTile.h"
#include "CBlock.h"
#include "CMouse.h"
#include "CEffect.h"
#include "CMonster.h"

#include "BFSSearch.h"
#include "CTurnManager.h"
#include "CTileManager.h"
#include "CMonsterSpawner.h"

using namespace battle;

CScene_Battle::CScene_Battle()
	: m_pPlayer(nullptr)
	, m_BFS(nullptr)
	, m_MonsterSpawner(nullptr)
	, m_TurnManager(nullptr)
	, m_TileManager(nullptr)
{
	// �Ŵ����� ��� -> ������Ʈ�� ����ϴ°� �ƴ϶� ���� ���� �����־�� �Ѵ�. Enter, Exit �Լ��� ���þ���
	// ���� ���� �ִ� �׷� ������Ʈ���� �����ؼ� �����ϴ� �����̶�, ���Ƿ� ������ ������Ű�� �ʰ� �����ؾ� �Ѵ�
	m_TurnManager = new CTurnManager;
	m_BFS = new BFSSearch;
	m_MonsterSpawner = new CMonsterSpawner;
	m_TileManager = new CTileManager;
}

CScene_Battle::~CScene_Battle()
{
	delete m_BFS;
	delete m_MonsterSpawner;
	delete m_TurnManager;
	delete m_TileManager;
}

void CScene_Battle::TurnInit(TURN_TYPE _type)
{
	switch (_type)
	{
	case TURN_TYPE::ENTER: break;
	case TURN_TYPE::PLAYER_START:
	{
		// ī�޶� ĳ���ͷ� �ʱ�ȭ
		CCamera::GetInstance()->SetTarget(nullptr);
		CCamera::GetInstance()->SetLookAt(m_pPlayer->GetPos());

		// ����Ʈ ���� �ִ� ��� Ÿ�ϵ��� ������ ���� �������� ������
		list<Vec2>& moveRoute = m_TurnManager->GetMoveRoute();
		m_TileManager->TileInit(moveRoute);

		// ����Ʈ �ʱ�ȭ
		m_TurnManager->RouteInit();
		break;
	}
	case TURN_TYPE::PLAYER_TILESELECT: break;
	case TURN_TYPE::PLAYER_MOVE:
	{
		CCamera::GetInstance()->SetTarget(m_pPlayer);

		// �÷��̾� �ִϸ��̼� ����
		m_pPlayer->SetState(PLAYER_STATE::MOVE);
		break;
	}
	case TURN_TYPE::PLAYER_ATTACK:
	{
		m_pPlayer->SetState(PLAYER_STATE::SWORD);
		break;
	}
	case TURN_TYPE::PLAYER_SKILL:
	{
		// ���� Ÿ�ϵ�(��� �����Դ� Ÿ�ϵ�) ���� Ÿ�ϵ�� ���½�Ű��
		CCamera::GetInstance()->SetTarget(nullptr);
		m_TileManager->TileRandomInit();

		// �÷��̾� �ִϸ��̼� ����
		m_pPlayer->SetState(PLAYER_STATE::ROLLING);
		break;
	}
	case TURN_TYPE::ENEMY_MOVE: break;
	case TURN_TYPE::ENEMY_ATTACK: break;
	case TURN_TYPE::EXIT: break;
	}
}

void CScene_Battle::TurnLogic()
{
	TURN_TYPE currentTurn = m_TurnManager->GetTurnState();
	switch (currentTurn)
	{
	case TURN_TYPE::ENTER: break;
	case TURN_TYPE::PLAYER_START: break;
	case TURN_TYPE::PLAYER_TILESELECT: break;
	case TURN_TYPE::PLAYER_MOVE:
	{
		PlayerMove();
		break;
	}
	case TURN_TYPE::PLAYER_ATTACK: break;
	case TURN_TYPE::PLAYER_SKILL: break;
	case TURN_TYPE::ENEMY_MOVE: break;
	case TURN_TYPE::ENEMY_ATTACK: break;
	case TURN_TYPE::EXIT: break;
	}
}

void CScene_Battle::PlayerMove()
{
	list<Vec2>& moveRoute = m_TurnManager->GetMoveRoute();
	if (moveRoute.empty())
	{
		// �� �Ŵ������� ���� üũ �ؼ�, ���� �����
		return;
	}

	Vec2 vDestination = REAL(moveRoute.front());
	Vec2 vCurrentPos = m_pPlayer->GetPos();

	// ��������(��ġ�� ��ġ) = �ֺ� ���� üũ -> ����
	if (vCurrentPos == vDestination)
	{
		// 1. ���� Ž�� ��, ������ ������ �Լ� ���� �� ���ݻ��� ��(lstTargetEnemy �迭�� ���� ���� ��)
		// Update���� �� ����Ʈ ��� üũ�ϰ�, �Ⱥ������ �������� ���ݻ��� ����
		/*BFS(m_mapGridPoint[vDestination], DIRECTION::FOUR_WAY, 1);*/

		// 2. Ÿ�� ���� ���� -> �̵��� ������ ������ ó��
		Vec2 gridDestination = moveRoute.front();
		vector<vector<TileState>>& tiles = m_TileManager->GetTileState();
		tiles[(int)gridDestination.y][(int)gridDestination.x].pTile->SetTileState(TILE_STATE::BLACK);

		// 3. Ÿ�� ���� ����(������, ������ġ ����), Ÿ�� ����Ʈ ��ĭ ����
		m_TurnManager->SetPlayerPos(moveRoute.front());
		moveRoute.pop_front();
		if (moveRoute.empty()) return;		// ������ Ÿ�� �����ϸ�, �Լ� Ż��

		vDestination = REAL(moveRoute.front());
	}

	// ĳ���Ϳ��� ������������ �����̰� ���
	m_pPlayer->Move(vDestination);
}

void CScene_Battle::TileSelectTrigger(CObject* _pObj)
{
	// ���� :: ���콺�� �� ���� ���¿��� Ÿ���� �ݶ��̴��� ���� ����
	// ���콺 �� ���� ���¿���, �ݶ��̴��� ������ -> �̺�Ʈ �Ŵ������� �� �Լ��� ȣ���Ŵ
	TURN_TYPE CurrnetTurn = m_TurnManager->GetTurnState();
	vector<vector<TileState>>& vecTiles = m_TileManager->GetTileState();

	switch (CurrnetTurn)
	{
	case TURN_TYPE::PLAYER_START:
	{
		Vec2 vPlayerPos = GRID(m_pPlayer->GetPos());
		Vec2 selectPos = GRID(_pObj->GetPos());

		// BFS�� 8���� Ž�� (�ֺ� 1ĭ)
		m_BFS->BFS(vPlayerPos, vecTiles, DIRECTION::EIGHT_WAY, 1);

		// BFS Ž�����, �湮 �߾�����(result �迭�� ������, ã����)
		if (vecTiles[(int)selectPos.y][(int)selectPos.x].bVisited)
		{
			// Ÿ�� ���� ����, ���� ��ġ ����, ����Ʈ �߰�, Ÿ�� ���õƴٰ� �Լ� �����ֱ�
			CTile* tile = (CTile*)_pObj;

			// Ÿ�� ���� ����, ���� ��ġ ����, ����Ʈ �߰�, Ÿ�� ���õƴٰ� �Լ� �����ֱ�, �� ��ȯ
			m_TurnManager->SetTileColor(tile->GetTileState());
			m_TurnManager->SetSelectTile(selectPos);
			m_TurnManager->GetMoveRoute().push_back(selectPos);
			tile->SetTileState((TILE_STATE)((int)tile->GetTileState() + 4));

			// �����
			DEBUG2(selectPos.x, selectPos.y);
			printf("Ÿ�� ����\n");

			// ī�޶� Ÿ�Ϸ� ����
			CCamera::GetInstance()->SetLookAt(REAL(selectPos));

			// �� ����
			m_TurnManager->SetTurnState(TURN_TYPE::PLAYER_TILESELECT);
		}
	}
		break;
	case TURN_TYPE::PLAYER_TILESELECT:
	{
		Vec2 selectPos = GRID(_pObj->GetPos());
		Vec2 currentPos = m_TurnManager->GetSelectTile();
		m_BFS->BFS(currentPos, vecTiles, DIRECTION::EIGHT_WAY, 1);

		// �ߺ��� ��ġ�� ����Ʈ�� ���� ���ϰ� ����
		list<Vec2>& moveRoute = m_TurnManager->GetMoveRoute();
		auto iter = std::find(moveRoute.begin(), moveRoute.end(), selectPos);
		if (vecTiles[(int)selectPos.y][(int)selectPos.x].bVisited && 
			vecTiles[(int)selectPos.y][(int)selectPos.x].pTile->GetTileState() == m_TurnManager->GetTileColor() &&
			iter == moveRoute.end())
		{
			CTile* tile = (CTile*)_pObj;
			
			// ī�޶� Ÿ�Ϸ� ����
			CCamera::GetInstance()->SetLookAt(REAL(selectPos));

			// ���� ��ġ ����, ����Ʈ �߰�, Ÿ�� ���õƴٰ� �Լ� �����ֱ�
			m_TurnManager->SetSelectTile(selectPos);
			moveRoute.push_back(selectPos);
			tile->SetTileState((TILE_STATE)((int)tile->GetTileState() + 4));

			// �����
			for (auto& lstPos : moveRoute) printf("%1.f, %1.f -> ", lstPos.x, lstPos.y);
			printf("\n");
		}
	}
		break;
	default:
		break;
	}

	// BFS �湮 �ʱ�ȭ
	m_BFS->BFS_Init(vecTiles);
}

void CScene_Battle::Update()
{
	if (KEY_TAP(KEY::ENTER))
	{
		ChangeScene(SCENE_TYPE::TOOL);
	}

	// ���� ����
	if (KEY_TAP(KEY::RBTN))
	{
		Vec2 vLookAt = CCamera::GetInstance()->GetRealPos(MOUSE_POS);
		CCamera::GetInstance()->SetLookAt(vLookAt);
	}

	// ���� �� ������Ʈ (���� ������Ʈ���� ������Ʈ)
	CScene::Update();

	// ���� ���� ó��
	TurnLogic();

	// �̺�Ʈ ó�����ִ°� ����
	m_TurnManager->Update();

}

void CScene_Battle::Enter()
{
	Vec2 vResolution = CCore::GetInstance()->GetResolution();

	// ���콺 �߰�
	CMouse* pMouse = new CMouse;
	AddObject(pMouse, GROUP_TYPE::MOUSE);

	// Ÿ�� + �� �߰�
	int startX = (int)(vResolution.x / 2);
	int startY = (int)(vResolution.y / 4);
	map<Vec2, Vec2>& mapRealPoint = m_TileManager->GetMapRealPoint();
	map<Vec2, Vec2>& mapGridPoint = m_TileManager->GetMapGridPoint();
	vector<vector<TileState>>& vecTileState = m_TileManager->GetTileState();

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
			pTile->SetPos(Vec2(drawX, drawY));
			vecTileState[y][x].pTile = pTile;
			AddObject(pTile, GROUP_TYPE::TILE);

			// �� �������
			CBlock* cBlcok = new CBlock(MAP_TYPE::SNOW);
			cBlcok->SetPos(Vec2(drawX, drawY));
			AddObject(cBlcok, GROUP_TYPE::BLOCK);
		}
	}

	// Player �߰�
	CPlayer* pPlayer = new CPlayer;
	Vec2 PlayerStartPos(4, 2);
	pPlayer->SetPos(REAL(PlayerStartPos));
	m_pPlayer = pPlayer;
	AddObject(pPlayer, GROUP_TYPE::PLAYER);

	 // GDI+ Test (Effect)
	CEffect* pEffect = new CEffect;
	pEffect->SetPos(REAL(PlayerStartPos));
	AddObject(pEffect, GROUP_TYPE::MISSILE_PLAYER);

	// Monster �߰�
	CMonster* pMonster = new CMonster;
	Vec2 MonsterStartPos(6, 3);
	pMonster->SetPos(REAL(MonsterStartPos));
	AddObject(pMonster, GROUP_TYPE::MONSTER);

	// Ÿ�ϰ� ���콺�� �浹ó��
	CCollisionMgr::GetInstance()->CheckGroup(GROUP_TYPE::MOUSE, GROUP_TYPE::TILE);
	
	// ī�޶� ����
	CCamera::GetInstance()->SetLookAt(REAL(PlayerStartPos));
}

void CScene_Battle::Exit()
{
	DeleteAll();
	m_BFS->BFS_Init(m_TileManager->GetTileState());
	m_TurnManager->Init();
	CCollisionMgr::GetInstance()->Reset();
}


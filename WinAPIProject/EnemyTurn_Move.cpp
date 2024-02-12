#include "pch.h"
#include "EnemyTurn_Move.h"

#include "CScene_Battle.h"
#include "CMonster.h"
#include "CPlayer.h"

#include "CMonsterSpawner.h"
#include "CTileManager.h"
#include "CTurnManager.h"
#include "BFSSearch.h"

#include "Monster_Trace.h"
#include "Monster_RandomMove.h"
#include "Monster_SelfHeal.h"
#include "Monster_Teleport.h"

#include "CDataUtils.h"

EnemyTurn::EnemyTurn()
	: m_vecStrategies((int)MONSTER_STRATEGY::NONE)
{
	m_vecStrategies[(int)MONSTER_STRATEGY::TRACE] = new Monster_Trace;
	m_vecStrategies[(int)MONSTER_STRATEGY::RANDOM_MOVE] = new Monster_RandomMove;
	m_vecStrategies[(int)MONSTER_STRATEGY::SELF_HEAL] = new Monster_SelfHeal;
	m_vecStrategies[(int)MONSTER_STRATEGY::RANDOM_TELEPORT] = new Monster_Teleport;
}

EnemyTurn::~EnemyTurn()
{
	SafeDeleteVec(m_vecStrategies);
}

void EnemyTurn::Init(CScene_Battle* _pScene)
{
	CTileCenter* m_TileCenter = _pScene->GetTileCenter();
	CMonsterSpawner* m_MonsterSpawner = _pScene->GetSpawner();
	list<CMonster*>& monsterList = _pScene->GetSpawner()->GetMonsterList();
	CPlayer* m_pPlayer = _pScene->GetPlayer();

	// �÷��̾� Ÿ�� ����
	m_pPlayer->SetTarget(nullptr);

	// ���Ͱ� �� �׾����� �¸�
	if (m_MonsterSpawner->GetMonsterList().empty())
	{
		_pScene->ChangeTurn(TURN_TYPE::WIN);
		return;
	}

	// ���� ���� ���� (����� �̸� �� �س���, �ѹ��� �̵�)
	for (auto& monster : monsterList)
	{
		printf("EnemyTurn::Handle :: ���� ���� ����� -> ");
		cout << wstring_to_string(monster->GetName()) << "�� ���� ����\n";
		// ���Ͱ� ������ �ִ� ���ϵ��� �������� ������ �̾Ƽ�, ������ �����Ŵ
		auto monsterStrategy = m_vecStrategies[(int)monster->RandomPattern()];
		if (monsterStrategy != nullptr)
		{
			monsterStrategy->Handle(_pScene, monster);
		}
	}

	// �÷��̾� �ʱ�ȭ
	m_pPlayer->SetState(PLAYER_STATE::IDLE);
	m_pPlayer->AnimationDirection(PLAYER_STATE::IDLE, true);

	printf("EnemyTurn::TurnInit :: �� ���� �ʱ�ȭ\n");
}

void EnemyTurn::Handle(CScene_Battle* _pScene)
{
	CTileCenter* m_TileCenter = _pScene->GetTileCenter();
	CLogicCenter* m_TurnCenter = _pScene->GetLogicCenter();
	list<CMonster*>& monsterList = _pScene->GetSpawner()->GetMonsterList();
	CPlayer* m_pPlayer = _pScene->GetPlayer();

	// �˻� �� Ÿ�� �ʱ�ȭ
	m_TileCenter->InitTileVisited();

	// ��Ʈ Ȯ�� �� ����Ʈ�� �� ������ �̵� ����
	for (auto& monster : monsterList)					
	{
		list<Vec2>& moveRoute = monster->GetRoute();
		// �̵� ��Ʈ�� ���� (Acting Done)
		if (moveRoute.empty())
		{
			continue;
		}
		Vec2 vDestination = REAL(moveRoute.front());	// ��ǥ Ÿ�� ������ǥ
		Vec2 vCurrentPos = monster->GetPos();			// ���� ���� ������ǥ

		// ��������(��ġ�� ��ġ) = �ֺ� ���� üũ -> ����
		if (vCurrentPos == vDestination)
		{
			// BFS Ž�� ��� ���� && Ž�� ��� �� �÷��̾� ����
			if (IsFoundObjects(_pScene, monster) && IsFoundResultPlayer(_pScene, monster))
			{
				AttackPlayer(_pScene, monster);
				return;
			}
			// �̵� ��Ʈ ��� �̵�
			if (moveRoute.empty())
			{
				SetMonsterActingDone(_pScene, monster);	// ���� Acting Done
				continue;
			}
			vDestination = REAL(moveRoute.front());		// ���� ��ǥ Ÿ�� ����
		}
		// ��ǥ Ÿ���� ���� �̵� ���
		GRID_DIRECTION gridDirection = GetGridDirection(monster->GetGridPos(), GRID(vDestination), vCurrentPos, vDestination);
		monster->Move(gridDirection, vDestination);
	}
	// ��� ������ �̵��� ��������, ���� �ٲ�
	if (IsAllMonstersActingDone(monsterList)) 
	{
		ChangeTurnToPlayer(_pScene);
	}
}

void EnemyTurn::InitMonster(CScene_Battle* _pScene, CMonster* _pMonster)
{
	CTileCenter* m_TileCenter = _pScene->GetTileCenter();
	CLogicCenter* m_TurnCenter = _pScene->GetLogicCenter();

	list<CObject*>& lstObj = m_TurnCenter->GetTargetList();
	vector<vector<TileState>>& vecTile = m_TileCenter->GetTiles();

	// Ÿ�� ����Ʈ �ʱ�ȭ
	lstObj.clear();
	m_TileCenter->InitTileVisited();

	// ���� ���� ��ġ ����
	auto lstPath = _pMonster->GetRoute();
	if (!lstPath.empty())
	{
		Vec2 lastPos = lstPath.back(); Vec2 curPos = _pMonster->GetGridPos();
		vecTile[(int)lastPos.y][(int)lastPos.x].pObj = nullptr;
		vecTile[(int)curPos.y][(int)curPos.x].pObj = _pMonster;
	}
	_pMonster->GetRoute().clear();
}

void EnemyTurn::AttackPlayer(CScene_Battle* _pScene, CMonster* _pMonster)
{
	CPlayer* m_pPlayer = _pScene->GetPlayer();

	GRID_DIRECTION gridDirection = GetGridDirection(_pMonster->GetGridPos(), m_pPlayer->GetGridPos(), _pMonster->GetPos(), m_pPlayer->GetPos());
	_pMonster->Attack(gridDirection, m_pPlayer);
	InitMonster(_pScene, _pMonster);

	printf("���� -> %1.f, %1.f\n", _pMonster->GetGridPos().x, _pMonster->GetGridPos().y);
}

void EnemyTurn::SetMonsterActingDone(CScene_Battle* _pScene, CMonster* _pMonster)
{
	// �˻��� �÷��̾ �������� �ٷ� Acting = true
	_pMonster->SetActing(true);
	_pMonster->AnimationDirection(L"idle", true);
	InitMonster(_pScene, _pMonster);

	printf("���� -> %1.f, %1.f\n", _pMonster->GetGridPos().x, _pMonster->GetGridPos().y);
}

void EnemyTurn::ChangeTurnToPlayer(CScene_Battle* _pScene)
{
	list<CMonster*>& monsterList = _pScene->GetSpawner()->GetMonsterList();

	for (auto& monster : monsterList)
	{
		monster->SetActing(false);
	}
	printf("EnemyTurn_Move::Handle :: �̵��� ������ ��� ������, ���������� �Ѿ�ϴ�.\n");
	_pScene->ChangeTurn(TURN_TYPE::PLAYER_START);
}

bool EnemyTurn::IsFoundObjects(CScene_Battle* _pScene, CMonster* _pMonster)
{
	CTileCenter* m_TileCenter = _pScene->GetTileCenter();
	CLogicCenter* m_TurnCenter = _pScene->GetLogicCenter();

	// ���� ������ �÷��̾ ������, ���� ���� ���� ���� üũ
	vector<vector<TileState>>& vecTile = m_TileCenter->GetTiles();
	list<CObject*>& lstObj = m_TurnCenter->GetTargetList();
	list<Vec2>& moveRoute = _pMonster->GetRoute();

	Vec2 vDestination = REAL(moveRoute.front());

	_pMonster->SetGridPos(moveRoute.front());
	moveRoute.pop_front();

	// BFS�� ��Ÿ���ŭ üũ
	BFSSearch::BFS(GRID(vDestination), vecTile, lstObj, DIRECTION::FOUR_WAY, _pMonster->GetRange());

	return !lstObj.empty();
}

bool EnemyTurn::IsFoundResultPlayer(CScene_Battle* _pScene, CMonster* _pMonster)
{
	list<CObject*>& lstObj = _pScene->GetLogicCenter()->GetTargetList();
	CPlayer* m_pPlayer = _pScene->GetPlayer();

	for (auto& obj : lstObj)
	{
		if (obj == (CObject*)m_pPlayer)
		{
			return true;	// �˻� ����� �÷��̾�� true
		}
	}
	return false;
}

bool EnemyTurn::IsAllMonstersActingDone(const list<CMonster*>& monsterList)
{
	for (const auto& monster : monsterList)
	{
		// ���� �ִϸ��̼� �̺�Ʈ�� ����Ǹ� true / ���� ���ϸ� �̵� ����� true
		if (!monster->IsActingDone())
		{
			return false;
		}
	}
	return true;
}
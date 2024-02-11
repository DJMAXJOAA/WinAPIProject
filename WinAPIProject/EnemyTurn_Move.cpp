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
	CPlayer* m_pPlayer = _pScene->GetPlayer();
	list<CMonster*>& monsterList = _pScene->GetSpawner()->GetMonsterList();

	// �÷��̾� Ÿ�� ����
	m_pPlayer->SetTarget(nullptr);

	// ���Ͱ� �� �׾�����, �� �Ѿ��
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

	// ��Ʈ Ȯ�� �� ���� �̵�
	for (auto& monster : monsterList)
	{
		list<Vec2>& moveRoute = monster->GetRoute();

		if (moveRoute.empty())
		{
			continue;
		}

		else
		{
			Vec2 vDestination = REAL(moveRoute.front());
			Vec2 vCurrentPos = monster->GetPos();

			// ��������(��ġ�� ��ġ) = �ֺ� ���� üũ -> ����
			if (vCurrentPos == vDestination)
			{
				monster->SetGridPos(moveRoute.front());
				moveRoute.pop_front();

				// ����Ʈ�� ���������

					// ���� ������ �÷��̾ ������, ���� ���� ���� ���� üũ
				vector<vector<TileState>>& vecTile = m_TileCenter->GetTiles();
				list<CObject*>& lstObj = m_TurnCenter->GetTargetList();

				// BFS�� ��Ÿ���ŭ üũ
				BFSSearch::BFS(GRID(vDestination), vecTile, lstObj, DIRECTION::FOUR_WAY, monster->GetRange());
				printf("EnemyTurn_Move::Handle :: BFS Ž�� ��� -> ");
				for (list<CObject*>::iterator iter = lstObj.begin(); iter != lstObj.end(); iter++)
				{
					cout << *iter << ", ";
				}
				printf("\n");

				// ������ ������, ���� ���� �� ��� �̵��� ������ �����ϰ� ���¼���
				for (auto& obj : lstObj)
				{
					// �˻� ����� �÷��̾��
					if (obj == (CObject*)m_pPlayer)
					{
						// ���� �̺�Ʈ
						GRID_DIRECTION gridDirection = GetGridDirection(monster->GetGridPos(), m_pPlayer->GetGridPos(), monster->GetPos(), m_pPlayer->GetPos());
						monster->Attack(gridDirection, m_pPlayer);

						// Ÿ�� ����Ʈ �ʱ�ȭ
						lstObj.clear();
						m_TileCenter->InitTileVisited();

						// �˻��� �÷��̾ �������� �ٷ� Acting = true
						moveRoute.clear();

						// ���� �ʱ�ȭ
						printf("���� -> %1.f, %1.f\n", monster->GetGridPos().x, monster->GetGridPos().y);
						return;
					}
				}

				if (moveRoute.empty())
				{
					// �˻��� �÷��̾ �������� �ٷ� Acting = true
					monster->SetActing(true);

					// BFS �ʱ�ȭ
					lstObj.clear();
					m_TileCenter->InitTileVisited();

					// ���� �ʱ�ȭ
					monster->AnimationDirection(L"idle", true);
					printf("���� -> %1.f, %1.f\n", monster->GetGridPos().x, monster->GetGridPos().y);
					continue;
				}

				vDestination = REAL(moveRoute.front());
			}

			GRID_DIRECTION gridDirection = GetGridDirection(monster->GetGridPos(), GRID(vDestination), vCurrentPos, vDestination);
			monster->Move(gridDirection, vDestination);
		}
	}

	// ��� ������ �̵��� ��������, ���� �ٲ�
	if (IsMonstersMovingDone(monsterList))
	{
		// �� �ٲٱ� ���� ���͵��� ��� ���� �ʱ�ȭ 
		for (auto& monster : monsterList)
		{
			monster->SetActing(false);
		}

		printf("EnemyTurn_Move::Handle :: �̵��� ������ ��� ������, ���������� �Ѿ�ϴ�.\n");
		_pScene->ChangeTurn(TURN_TYPE::PLAYER_START);
	}
}

bool EnemyTurn::IsMonstersMovingDone(const list<CMonster*>& monsterList)
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

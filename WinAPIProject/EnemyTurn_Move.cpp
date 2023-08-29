#include "pch.h"
#include "EnemyTurn_Move.h"

#include "CScene_Battle.h"
#include "CMonster.h"
#include "CPlayer.h"

#include "CAnimator.h"
#include "CAnimation.h"

#include "CMonsterSpawner.h"
#include "CTileManager.h"
#include "CTurnManager.h"
#include "BFSSearch.h"

#include "Monster_Trace.h"
#include "Monster_RandomMove.h"
#include "Monster_SelfHeal.h"
#include "Monster_Teleport.h"

EnemyTurn_Move::EnemyTurn_Move()
	: m_vecStrategies(6)
	, m_MonsterStrategy(nullptr)
	, m_bRouteCalculate(false)
{
	m_vecStrategies[(int)MONSTER_STRATEGY::TRACE] = new Monster_Trace;
	m_vecStrategies[(int)MONSTER_STRATEGY::RANDOM_MOVE] = new Monster_RandomMove;
	m_vecStrategies[(int)MONSTER_STRATEGY::SELF_HEAL] = new Monster_SelfHeal;
	m_vecStrategies[(int)MONSTER_STRATEGY::RANDOM_TELEPORT] = new Monster_Teleport;
}

EnemyTurn_Move::~EnemyTurn_Move()
{
	SafeDeleteVec(m_vecStrategies);
}

void EnemyTurn_Move::Handle(CScene_Battle* _pScene)
{
	CTileCenter* m_TileCenter = _pScene->GetTileCenter();
	CTurnCenter* m_TurnCenter = _pScene->GetTurnCenter();
	BFSSearch* m_BFS = _pScene->GetBFS();
	list<CMonster*>& monsterList = _pScene->GetSpawner()->GetMonsterList();
	CPlayer* m_pPlayer = _pScene->GetPlayer();

	// �� 1ȸ ���
	if (m_bRouteCalculate == false)
	{
		m_bRouteCalculate = true;

		// ���� ���� ���� (����� �̸� �� �س���, �ѹ��� �̵�)

		for (auto& monster : monsterList)
		{
			printf("EnemyTurn_Move::Handle :: ���� �̵� ����� -> ");
			cout << monster << "�� �̵� ���� ����\n";

			// ���Ͱ� ������ �ִ� ���ϵ��� �������� ������ �̾Ƽ�, ������ �����Ŵ
			m_MonsterStrategy = m_vecStrategies[(int)monster->RandomPattern()];
			if (m_MonsterStrategy != nullptr)
			{
				m_MonsterStrategy->Handle(_pScene, monster);
			}
		}
	}

	// �˻� �� Ÿ�� �ʱ�ȭ
	m_TileCenter->TileVisitedInit();

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
				if (moveRoute.empty())
				{
					// ���� ������ �÷��̾ ������, ���� ���� ���� ���� üũ
					vector<vector<TileState>>& vecTile = m_TileCenter->GetTiles();
					list<CObject*>& lstObj = m_TurnCenter->GetTargetList();

					// BFS�� ��Ÿ���ŭ üũ
					m_BFS->BFS(GRID(vDestination), vecTile, lstObj, DIRECTION::FOUR_WAY, monster->GetRange());
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
							m_TileCenter->TileVisitedInit();
							return;
						}
					}

					// �˻��� �÷��̾ �������� �ٷ� Acting = true
					monster->SetActing(true);

					// BFS �ʱ�ȭ
					lstObj.clear();
					m_TileCenter->TileVisitedInit();

					// ���� �ʱ�ȭ
					monster->AnimationDirection(L"idle", true);
					printf("���� -> %1.f, %1.f\n", monster->GetGridPos().x, monster->GetGridPos().y);
					continue;
				}

				if(!moveRoute.empty())
				{
					vDestination = REAL(moveRoute.front());
				}
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
		m_bRouteCalculate = false;
		m_TurnCenter->ChangeTurn(TURN_TYPE::PLAYER_START);
	}
}

bool EnemyTurn_Move::IsMonstersMovingDone(const list<CMonster*>& monsterList)
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
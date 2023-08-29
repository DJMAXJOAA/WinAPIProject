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

	// 단 1회 계산
	if (m_bRouteCalculate == false)
	{
		m_bRouteCalculate = true;

		// 몬스터 패턴 결정 (계산을 미리 다 해놓고, 한번에 이동)

		for (auto& monster : monsterList)
		{
			printf("EnemyTurn_Move::Handle :: 몬스터 이동 계산중 -> ");
			cout << monster << "의 이동 패턴 실행\n";

			// 몬스터가 가지고 있는 패턴들을 랜덤으로 패턴을 뽑아서, 로직을 실행시킴
			m_MonsterStrategy = m_vecStrategies[(int)monster->RandomPattern()];
			if (m_MonsterStrategy != nullptr)
			{
				m_MonsterStrategy->Handle(_pScene, monster);
			}
		}
	}

	// 검색 전 타일 초기화
	m_TileCenter->TileVisitedInit();

	// 루트 확정 후 적군 이동
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

			// 도착했음(위치가 일치) = 주변 적군 체크 -> 갱신
			if (vCurrentPos == vDestination)
			{
				monster->SetGridPos(moveRoute.front());
				moveRoute.pop_front();

				// 리스트가 비어있으면
				if (moveRoute.empty())
				{
					// 도착 지점에 플레이어가 있으면, 몬스터 공격 설정 여부 체크
					vector<vector<TileState>>& vecTile = m_TileCenter->GetTiles();
					list<CObject*>& lstObj = m_TurnCenter->GetTargetList();

					// BFS로 사거리만큼 체크
					m_BFS->BFS(GRID(vDestination), vecTile, lstObj, DIRECTION::FOUR_WAY, monster->GetRange());
					printf("EnemyTurn_Move::Handle :: BFS 탐색 결과 -> ");
					for (list<CObject*>::iterator iter = lstObj.begin(); iter != lstObj.end(); iter++)
					{
						cout << *iter << ", ";
					}
					printf("\n");

					// 적군이 있으면, 상태 변경 후 모두 이동이 끝나면 공격하게 상태설정
					for (auto& obj : lstObj)
					{
						// 검색 대상이 플레이어면
						if (obj == (CObject*)m_pPlayer)
						{
							// 어택 이벤트
							GRID_DIRECTION gridDirection = GetGridDirection(monster->GetGridPos(), m_pPlayer->GetGridPos(), monster->GetPos(), m_pPlayer->GetPos());
							monster->Attack(gridDirection, m_pPlayer);

							// 타겟 리스트 초기화
							lstObj.clear();
							m_TileCenter->TileVisitedInit();
							return;
						}
					}

					// 검색시 플레이어가 없었으면 바로 Acting = true
					monster->SetActing(true);

					// BFS 초기화
					lstObj.clear();
					m_TileCenter->TileVisitedInit();

					// 몬스터 초기화
					monster->AnimationDirection(L"idle", true);
					printf("도착 -> %1.f, %1.f\n", monster->GetGridPos().x, monster->GetGridPos().y);
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

	// 모든 적군의 이동이 끝났으면, 턴을 바꿈
	if (IsMonstersMovingDone(monsterList))
	{
		// 턴 바꾸기 전에 몬스터들의 모든 상태 초기화 
		for (auto& monster : monsterList)
		{
			monster->SetActing(false);
		}

		printf("EnemyTurn_Move::Handle :: 이동과 공격이 모두 끝나서, 공격턴으로 넘어갑니다.\n");
		m_bRouteCalculate = false;
		m_TurnCenter->ChangeTurn(TURN_TYPE::PLAYER_START);
	}
}

bool EnemyTurn_Move::IsMonstersMovingDone(const list<CMonster*>& monsterList)
{
	for (const auto& monster : monsterList)
	{
		// 공격 애니메이션 이벤트가 종료되면 true / 공격 안하면 이동 종료시 true
		if (!monster->IsActingDone())
		{
			return false;
		}
	}
	return true;
}
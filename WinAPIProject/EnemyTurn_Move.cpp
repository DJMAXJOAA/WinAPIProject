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

	// 플레이어 타겟 삭제
	m_pPlayer->SetTarget(nullptr);

	// 몬스터가 다 죽었으면 승리
	if (m_MonsterSpawner->GetMonsterList().empty())
	{
		_pScene->ChangeTurn(TURN_TYPE::WIN);
		return;
	}

	// 몬스터 패턴 결정 (계산을 미리 다 해놓고, 한번에 이동)
	for (auto& monster : monsterList)
	{
		printf("EnemyTurn::Handle :: 몬스터 패턴 계산중 -> ");
		cout << wstring_to_string(monster->GetName()) << "의 패턴 실행\n";
		// 몬스터가 가지고 있는 패턴들을 랜덤으로 패턴을 뽑아서, 로직을 실행시킴
		auto monsterStrategy = m_vecStrategies[(int)monster->RandomPattern()];
		if (monsterStrategy != nullptr)
		{
			monsterStrategy->Handle(_pScene, monster);
		}
	}

	// 플레이어 초기화
	m_pPlayer->SetState(PLAYER_STATE::IDLE);
	m_pPlayer->AnimationDirection(PLAYER_STATE::IDLE, true);

	printf("EnemyTurn::TurnInit :: 적 상태 초기화\n");
}

void EnemyTurn::Handle(CScene_Battle* _pScene)
{
	CTileCenter* m_TileCenter = _pScene->GetTileCenter();
	CLogicCenter* m_TurnCenter = _pScene->GetLogicCenter();
	list<CMonster*>& monsterList = _pScene->GetSpawner()->GetMonsterList();
	CPlayer* m_pPlayer = _pScene->GetPlayer();

	// 검색 전 타일 초기화
	m_TileCenter->InitTileVisited();

	// 루트 확정 후 리스트의 각 적군들 이동 수행
	for (auto& monster : monsterList)					
	{
		list<Vec2>& moveRoute = monster->GetRoute();
		// 이동 루트가 없음 (Acting Done)
		if (moveRoute.empty())
		{
			continue;
		}
		Vec2 vDestination = REAL(moveRoute.front());	// 목표 타일 실제좌표
		Vec2 vCurrentPos = monster->GetPos();			// 현재 몬스터 실제좌표

		// 도착했음(위치가 일치) = 주변 적군 체크 -> 갱신
		if (vCurrentPos == vDestination)
		{
			// BFS 탐색 결과 존재 && 탐색 결과 중 플레이어 존재
			if (IsFoundObjects(_pScene, monster) && IsFoundResultPlayer(_pScene, monster))
			{
				AttackPlayer(_pScene, monster);
				return;
			}
			// 이동 루트 모두 이동
			if (moveRoute.empty())
			{
				SetMonsterActingDone(_pScene, monster);	// 몬스터 Acting Done
				continue;
			}
			vDestination = REAL(moveRoute.front());		// 다음 목표 타일 설정
		}
		// 목표 타일을 향해 이동 명령
		GRID_DIRECTION gridDirection = GetGridDirection(monster->GetGridPos(), GRID(vDestination), vCurrentPos, vDestination);
		monster->Move(gridDirection, vDestination);
	}
	// 모든 적군의 이동이 끝났으면, 턴을 바꿈
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

	// 타겟 리스트 초기화
	lstObj.clear();
	m_TileCenter->InitTileVisited();

	// 몬스터 최종 위치 갱신
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

	printf("도착 -> %1.f, %1.f\n", _pMonster->GetGridPos().x, _pMonster->GetGridPos().y);
}

void EnemyTurn::SetMonsterActingDone(CScene_Battle* _pScene, CMonster* _pMonster)
{
	// 검색시 플레이어가 없었으면 바로 Acting = true
	_pMonster->SetActing(true);
	_pMonster->AnimationDirection(L"idle", true);
	InitMonster(_pScene, _pMonster);

	printf("도착 -> %1.f, %1.f\n", _pMonster->GetGridPos().x, _pMonster->GetGridPos().y);
}

void EnemyTurn::ChangeTurnToPlayer(CScene_Battle* _pScene)
{
	list<CMonster*>& monsterList = _pScene->GetSpawner()->GetMonsterList();

	for (auto& monster : monsterList)
	{
		monster->SetActing(false);
	}
	printf("EnemyTurn_Move::Handle :: 이동과 공격이 모두 끝나서, 공격턴으로 넘어갑니다.\n");
	_pScene->ChangeTurn(TURN_TYPE::PLAYER_START);
}

bool EnemyTurn::IsFoundObjects(CScene_Battle* _pScene, CMonster* _pMonster)
{
	CTileCenter* m_TileCenter = _pScene->GetTileCenter();
	CLogicCenter* m_TurnCenter = _pScene->GetLogicCenter();

	// 도착 지점에 플레이어가 있으면, 몬스터 공격 설정 여부 체크
	vector<vector<TileState>>& vecTile = m_TileCenter->GetTiles();
	list<CObject*>& lstObj = m_TurnCenter->GetTargetList();
	list<Vec2>& moveRoute = _pMonster->GetRoute();

	Vec2 vDestination = REAL(moveRoute.front());

	_pMonster->SetGridPos(moveRoute.front());
	moveRoute.pop_front();

	// BFS로 사거리만큼 체크
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
			return true;	// 검색 대상이 플레이어면 true
		}
	}
	return false;
}

bool EnemyTurn::IsAllMonstersActingDone(const list<CMonster*>& monsterList)
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
#include "pch.h"
#include "EnemyTurn_Move.h"

#include "CScene_Battle.h"
#include "CMonster.h"

#include "CMonsterSpawner.h"

#include "Monster_Trace.h"
#include "Monster_RandomMove.h"
#include "Monster_SelfHeal.h"
#include "Monster_Teleport.h"

EnemyTurn_Move::EnemyTurn_Move()
	: m_vecStrategies{}
	, m_MonsterStrategy(nullptr)
{
	m_vecStrategies.push_back(new Monster_Trace);
	m_vecStrategies.push_back(new Monster_RandomMove);
	m_vecStrategies.push_back(new Monster_SelfHeal);
	m_vecStrategies.push_back(new Monster_Teleport);
}

EnemyTurn_Move::~EnemyTurn_Move()
{
	SafeDeleteVec(m_vecStrategies);
}

void EnemyTurn_Move::Handle(CScene_Battle* _pScene)
{
	// 몬스터 패턴 결정 (계산을 미리 다 해놓고, 한번에 이동)
	vector<CMonster*>& monsterList = _pScene->GetSpawner()->GetMonsterList();

	for (auto& monster : monsterList)
	{
		printf("EnemyTurn_Move::Handle :: 몬스터 이동 계산중 -> ");
		cout << monster << "의 이동 패턴 실행\n";

		// 몬스터가 가지고 있는 패턴들을 랜덤으로 패턴을 뽑아서, 로직을 실행시킴
		m_MonsterStrategy = m_vecStrategies[(int)monster->RandomPattern()];
		m_MonsterStrategy->Handle(_pScene, monster);
	}
}

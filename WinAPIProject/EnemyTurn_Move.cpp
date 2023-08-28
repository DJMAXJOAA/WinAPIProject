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
	// ���� ���� ���� (����� �̸� �� �س���, �ѹ��� �̵�)
	vector<CMonster*>& monsterList = _pScene->GetSpawner()->GetMonsterList();

	for (auto& monster : monsterList)
	{
		printf("EnemyTurn_Move::Handle :: ���� �̵� ����� -> ");
		cout << monster << "�� �̵� ���� ����\n";

		// ���Ͱ� ������ �ִ� ���ϵ��� �������� ������ �̾Ƽ�, ������ �����Ŵ
		m_MonsterStrategy = m_vecStrategies[(int)monster->RandomPattern()];
		m_MonsterStrategy->Handle(_pScene, monster);
	}
}

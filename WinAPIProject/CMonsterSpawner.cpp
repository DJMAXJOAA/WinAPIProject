#include "pch.h"
#include "CMonsterSpawner.h"

#include "CEventMgr.h"

#include "CMonster.h"

#include "MonsterData.h"

#include "CMonsterFactory.h"
#include "CMonsterFactory_Stage1.h"
#include "CMonsterFactory_Stage2.h"

CMonsterSpawner::CMonsterSpawner()
	: m_mapMonsterData{}
	, m_vecSpawnPattern{}
{
}

CMonsterSpawner::~CMonsterSpawner()
{
	// �������� ���� ���� ��ü�� ������Ŵ
	// ���� ��ü�� ������ ������ ����������
	SafeDeleteVec(m_vecSpawnPattern);
}

void CMonsterSpawner::Init()
{
	// ���� ����
	CMonsterFactory* Stage1 = new CMonsterFactory_Stage1;
	m_vecSpawnPattern.push_back(Stage1);

	CMonsterFactory* Stage2 = new CMonsterFactory_Stage2;
	m_vecSpawnPattern.push_back(Stage2);
}

void CMonsterSpawner::Stage1()
{
	m_vecSpawnPattern[0]->SpawnMonster(m_mapMonsterData);
}

void CMonsterSpawner::Stage2()
{
}

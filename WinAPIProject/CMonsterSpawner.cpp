#include "pch.h"
#include "CMonsterSpawner.h"

#include "CDataMgr.h"

#include "CMonster.h"

#include "MonsterData.h"
#include "FieldData.h"

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

void CMonsterSpawner::SpawnMonster(int _level, FIELD_TYPE _type)
{
}

void CMonsterSpawner::SpawnMonsterPattern(int _level, FIELD_TYPE _type)
{
}

void CMonsterSpawner::SpawnMonster()
{
	FieldData* fieldData = CDataMgr::GetInstance()->FindData();
}

void CMonsterSpawner::SpawnMonster(map<wstring, CMonster*>& enemyDatas)
{

}
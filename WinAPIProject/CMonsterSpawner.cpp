#include "pch.h"
#include "CMonsterSpawner.h"

#include "CDataMgr.h"

#include "CMonster.h"

#include "MonsterData.h"
#include "FieldData.h"

#include "CData.h"
#include "AnimatorData.h"
#include "MonsterData.h"

CMonsterSpawner::CMonsterSpawner()
	: m_mapMonsterData{}
{
	vector<CData*> allData = CDataMgr::GetInstance()->LoadAllData(500);
	for (int i = 0; i < allData.size(); i++)
	{
		// �ʿ� Ŭ�п� ������Ÿ�� ��ü���� ���
		MonsterData* monData = ((MonsterData*)allData[i]);
		CMonster* pMonster = new CMonster(monData);
		m_mapMonsterData.insert(make_pair(monData->GetKey(), pMonster));
	}
}

CMonsterSpawner::~CMonsterSpawner()
{
	// ������Ÿ�� ��ü�� �����Ҵ� ����
	// ���� ��ü�� ������ ������ ����������
	SafeDeleteMap(m_mapMonsterData);
}

void CMonsterSpawner::Init()
{
	m_lstMonsters.clear();
}

void CMonsterSpawner::Update()
{
	// ���� ������ ��ü�� remove
	auto removeIter = remove_if(m_lstMonsters.begin(), m_lstMonsters.end(),
		[](CMonster* monster) { return monster->IsDead(); });

	m_lstMonsters.erase(removeIter, m_lstMonsters.end());
}

void CMonsterSpawner::SpawnMonster(FieldData* _data)
{
	assert(_data);
	vector<pair<int, int>> vecSpawn = _data->m_vecMonsters;
	for (int i = 0; i < vecSpawn.size(); i++)
	{
		for (int j = 0; j < vecSpawn[i].second; j++)
		{
			CMonster* pMonster = m_mapMonsterData.find(vecSpawn[i].first)->second->Clone();
			assert(pMonster);
			m_lstMonsters.push_back(pMonster);
			CreateObj(pMonster, GROUP_TYPE::UNIT);
		}
	}
}

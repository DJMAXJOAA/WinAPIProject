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
	Init();
}

CMonsterSpawner::~CMonsterSpawner()
{
	// 스테이지 생성 패턴 객체만 해제시킴
	// 몬스터 객체는 어차피 씬에서 해제시켜줌
	
}

void CMonsterSpawner::Init()
{
	vector<CData*> allData = CDataMgr::GetInstance()->LoadAllData(500);
	for (int i = 0; i < allData.size(); i++)
	{
		// 맵에 클론용 객체정보 등록
		MonsterData* monData = ((MonsterData*)allData[i]);
		CMonster* pMonster = new CMonster(monData);
		m_mapMonsterData.insert(make_pair(monData->GetKey(), pMonster));
	}
	
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
			CreateObj(pMonster, GROUP_TYPE::MONSTER);
		}
	}
}

#include "pch.h"
#include "CDataMgr.h"

#include "GameInfo.h"
#include "AnimationData.h"
#include "AnimatorData.h"
#include "ItemData.h"
#include "SkillData.h"
#include "MonsterData.h"

CDataMgr::CDataMgr()
	: m_mapDataList{}
{
}

CDataMgr::~CDataMgr()
{
}

CData* CDataMgr::CreateData(int _key)
{
	// 해시맵에 데이터 만들고 등록
	// 각 클래스의 생성자에서 키에 맞는 Init() 실행
	CData* AddData = nullptr;
	if (_key >= 500) AddData = new MonsterData(_key);
	else if (_key >= 400) AddData = new SkillData(_key);
	else if (_key >= 300) AddData = new ItemData(_key);
	else if (_key >= 200) AddData = new AnimatorData(_key);
	else if (_key >= 100) AddData = new AnimationData(_key);
	else if (_key >= 0)	 AddData = new GameInfo(_key);
	else assert(0);		// 오류검출
	
	m_mapDataList.insert(make_pair(_key, AddData));
	return AddData;
}

CData* CDataMgr::FindData(int _key)
{
	unordered_map<int, CData*>::iterator iter = m_mapDataList.find(_key);

	// 결과값 없으면 새로 생성하고 데이터 반환, 있으면 그대로 반환
	if (iter == m_mapDataList.end()) return CreateData(_key)->FindData();
	else return iter->second->FindData();
}

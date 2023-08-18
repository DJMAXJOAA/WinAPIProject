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
	// �ؽøʿ� ������ ����� ���
	// �� Ŭ������ �����ڿ��� Ű�� �´� Init() ����
	CData* AddData = nullptr;
	if (_key >= 500) AddData = new MonsterData(_key);
	else if (_key >= 400) AddData = new SkillData(_key);
	else if (_key >= 300) AddData = new ItemData(_key);
	else if (_key >= 200) AddData = new AnimatorData(_key);
	else if (_key >= 100) AddData = new AnimationData(_key);
	else if (_key >= 0)	 AddData = new GameInfo(_key);
	else assert(0);		// ��������
	
	m_mapDataList.insert(make_pair(_key, AddData));
	return AddData;
}

CData* CDataMgr::FindData(int _key)
{
	unordered_map<int, CData*>::iterator iter = m_mapDataList.find(_key);

	// ����� ������ ���� �����ϰ� ������ ��ȯ, ������ �״�� ��ȯ
	if (iter == m_mapDataList.end()) return CreateData(_key)->FindData();
	else return iter->second->FindData();
}

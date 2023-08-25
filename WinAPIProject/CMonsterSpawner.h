#pragma once

class CMonster;
class CMonsterFactory;

class CMonsterSpawner
{
private:
	map<wstring, CMonster*>		m_mapMonsterData;		// 클론용 몬스터 데이터(검색용)
	vector<CMonsterFactory*>	m_vecSpawnPattern;		// 스테이지마다 적의 생성 패턴
	
public:
	CMonsterSpawner();
	~CMonsterSpawner();

public:
	void Init();		// 몬스터 데이터 객체화시켜서 저장

public:
	void Stage1();
	void Stage2();
};


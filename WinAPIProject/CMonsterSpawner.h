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
	// 몬스터 생성 -> level에 따라 난이도 변경, 맵 소환 규칙은 동일한 레벨 중에 랜덤으로 결정
	// 레벨, 방 형태
	void SpawnMonster(int _level, FIELD_TYPE _type);	

public:
	void SpawnMonsterPattern(int _level, FIELD_TYPE _type);		// 데이터에 저장된 몬스터 생성 패턴을 불러와서, 데이터 양식에 맞게 몬스터 생성
};


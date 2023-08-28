#pragma once

class CMonster;
class FieldData;

class CMonsterSpawner
{
private:
	map<int, CMonster*>			m_mapMonsterData;	// 클론용 몬스터 데이터(검색용)
	vector<CMonster*>			m_vecMonsters;		// 필드에 있는 몬스터 배열
	
public:
	CMonsterSpawner();
	~CMonsterSpawner();

public:
	vector<CMonster*>& GetMonsterList() { return m_vecMonsters; }

public:
	// 몬스터 생성 -> level에 따라 난이도 변경, 맵 소환 규칙은 동일한 레벨 중에 랜덤으로 결정
	// 레벨, 방 형태
	void SpawnMonster(FieldData* _data);

public:
	void Init();		// 몬스터 데이터 객체화시켜서 저장
	void Update();		// 사망 정보가 있는 몬스터들을 배열에서 삭제

};


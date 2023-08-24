#pragma once

class CScene_Battle;
class CMonster;

class CMonsterFactory
{
private:
	// 프로토타입 패턴으로 몬스터 객체 정보 저장, 생성은 클론시키기
	vector<CMonster*>	m_vecMonsterList;

public:
	CMonsterFactory();
	~CMonsterFactory();

public:
	//virtual void SpawnMonster() = 0; // 소환 패턴은 하위 클래스에서 결정
};

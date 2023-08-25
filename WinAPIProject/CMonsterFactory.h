#pragma once

class CMonster;

class CMonsterFactory
{
public:
	CMonsterFactory();
	~CMonsterFactory();

public:
	// 적군의 객체 정보(Clone해서 씬에 생성), 적군이 등록될 벡터 -> 적 정보 알기 쉽게 스포너가 참조
	// 실제 몬스터들은 씬의 몬스터 그룹에 저장, 몬스터스포너는 참조만 함(정보만)
	// 소환 패턴은 자식 클래스들에서 결정
	virtual void SpawnMonster(map<wstring, CMonster*>& enemyDatas) = 0;
};

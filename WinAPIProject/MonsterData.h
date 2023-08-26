#pragma once
#include "CData.h"

class MonsterData :
	public CData
{
public:
	wstring						m_strName;				// 몬스터 이름
	int							m_iAnimatorKey;			// 애니메이션 이름
	vector<MONSTER_STRATEGY>	m_vecMosterStrategy;	// 가질 수 있는 패턴
	float						m_fHP;					// 체력
	float						m_fAtt;					// 공격력
	int							m_iMove;				// 한 턴에 이동할 수 있는 이동량
	int							m_iRange;				// 공격 사거리

public:
	MonsterData(int _key);
	~MonsterData();

public:
	virtual void ParseData(const json& item) override;
};


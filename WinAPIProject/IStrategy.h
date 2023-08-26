#pragma once

class CMonster;

class IMonsterStrategy
{
public:
	virtual ~IMonsterStrategy() {}

	virtual IMonsterStrategy* Clone() = 0;	// ����

public:
	virtual void MoveStrategy(CMonster* _pMon) = 0;
};


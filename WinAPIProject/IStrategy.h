#pragma once

class CMonster;

class IMonsterStrategy
{
public:
	virtual ~IMonsterStrategy() {}

	virtual IMonsterStrategy* Clone() = 0;	// º¹Á¦

public:
	virtual void MoveStrategy(CMonster* _pMon) = 0;
};


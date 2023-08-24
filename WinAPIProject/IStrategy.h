#pragma once

class CMonster;

class IMonsterStrategy
{
public:
	virtual ~IMonsterStrategy() {}

public:
	virtual void MoveStrategy(CMonster* _pMon) = 0;
};


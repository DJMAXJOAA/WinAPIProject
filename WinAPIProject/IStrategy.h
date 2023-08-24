#pragma once

class CMonster;

class IStrategy
{
public:
	virtual ~IStrategy() {}

public:
	virtual void MoveStrategy(CMonster* _pMon) = 0;
};


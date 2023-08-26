#pragma once
#include "IStrategy.h"
class Heal :
    public IMonsterStrategy
{
    CLONE(Heal)

public:
    virtual void MoveStrategy(CMonster* _pMon) {};
};


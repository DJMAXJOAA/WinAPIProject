#pragma once
#include "IStrategy.h"
class Trace :
    public IMonsterStrategy
{
    CLONE(Trace)

public:
    virtual void MoveStrategy(CMonster* _pMon) {};
};


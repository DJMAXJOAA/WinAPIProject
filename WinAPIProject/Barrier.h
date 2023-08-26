#pragma once
#include "IStrategy.h"
class Barrier :
    public IMonsterStrategy
{
    CLONE(Barrier)

public:
    virtual void MoveStrategy(CMonster* _pMon) {};
};


#pragma once
#include "IStrategy.h"
class RandomMove :
    public IMonsterStrategy
{
    CLONE(RandomMove)

public:
    virtual void MoveStrategy(CMonster* _pMon) {};
};


#pragma once
#include "IStrategy.h"
class Teleport :
    public IMonsterStrategy
{
    CLONE(Teleport)

public:
    virtual void MoveStrategy(CMonster* _pMon) {};
};


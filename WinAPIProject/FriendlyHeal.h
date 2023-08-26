#pragma once
#include "IStrategy.h"
class FriendlyHeal :
    public IMonsterStrategy
{
    CLONE(FriendlyHeal)

public:
    virtual void MoveStrategy(CMonster* _pMon) {};
};


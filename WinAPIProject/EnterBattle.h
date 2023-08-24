#pragma once
#include "IStategyTurn.h"

class EnterBattle :
    public IStategyTurn
{
public:
    virtual void TurnUpdate(CScene_Battle* _scene);
};


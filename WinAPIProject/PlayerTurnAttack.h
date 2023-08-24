#pragma once
#include "IStategyTurn.h"

class PlayerTurnAttack :
    public IStategyTurn
{
public:
    virtual void TurnUpdate(CScene_Battle* _scene);
};


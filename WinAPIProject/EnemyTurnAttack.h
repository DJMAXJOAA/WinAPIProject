#pragma once
#include "IStategyTurn.h"

class EnemyTurnAttack :
    public IStategyTurn
{
public:
    virtual void TurnUpdate(CScene_Battle* _scene);
};


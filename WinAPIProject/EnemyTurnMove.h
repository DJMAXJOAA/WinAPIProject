#pragma once
#include "IStategyTurn.h"

class EnemyTurnMove :
    public IStategyTurn
{
public:
    virtual void TurnUpdate(CScene_Battle* _scene);
};


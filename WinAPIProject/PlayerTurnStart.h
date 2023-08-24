#pragma once
#include "IStategyTurn.h"

class PlayerTurnStart :
    public IStategyTurn
{
public:
    virtual void TurnUpdate(CScene_Battle* _scene);
};


#pragma once
#include "IStategyTurn.h"

class PlayerTurnMove :
    public IStategyTurn
{
public:
    virtual void TurnUpdate(CScene_Battle* _scene);
};


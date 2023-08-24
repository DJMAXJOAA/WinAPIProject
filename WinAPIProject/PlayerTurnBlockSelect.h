#pragma once
#include "IStategyTurn.h"

class PlayerTurnBlockSelect :
    public IStategyTurn
{
public:
    virtual void TurnUpdate(CScene_Battle* _scene);
};


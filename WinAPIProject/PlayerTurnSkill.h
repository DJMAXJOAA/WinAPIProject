#pragma once
#include "IStategyTurn.h"

class PlayerTurnSkill :
    public IStategyTurn
{
public:
    virtual void TurnUpdate(CScene_Battle* _scene);
};


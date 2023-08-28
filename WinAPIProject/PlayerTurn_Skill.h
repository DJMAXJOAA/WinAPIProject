#pragma once
#include "CBattleState.h"

class PlayerTurn_Skill :
    public CBattleState
{
public:
    virtual void Handle(CScene_Battle* _pScene) override;
};


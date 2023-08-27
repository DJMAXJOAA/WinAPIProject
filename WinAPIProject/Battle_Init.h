#pragma once
#include "CBattleState.h"

class Battle_Init :
    public CBattleState
{
    virtual void Handle(CScene_Battle* _pScene) override;
};


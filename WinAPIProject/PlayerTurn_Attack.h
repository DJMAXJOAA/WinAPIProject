#pragma once
#include "CBattleState.h"

class PlayerTurn_Attack :
    public CBattleState
{
public:
    virtual void Handle(CScene_Battle* _pScene) override;
};



#pragma once
#include "CBattleState.h"

class PlayerTurn_Move :
    public CBattleState
{
public:
    virtual void Handle(CScene_Battle* _pScene) override;
};


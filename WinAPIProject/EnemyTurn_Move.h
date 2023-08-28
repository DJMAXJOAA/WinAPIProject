#pragma once
#include "CBattleState.h"

class EnemyTurn_Move :
    public CBattleState
{
public:
    virtual void Handle(CScene_Battle* _pScene) override;
};


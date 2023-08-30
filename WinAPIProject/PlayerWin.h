#pragma once
#include "CBattleState.h"

class PlayerWin :
    public CBattleState
{
public:
    virtual void Handle(CScene_Battle* _pScene) override;
};


#pragma once
#include "CBattleState.h"

class PlayerWin :
    public CBattleState
{
public:
    virtual void Init(CScene_Battle* _pScene) override;
    virtual void Handle(CScene_Battle* _pScene) override;
};


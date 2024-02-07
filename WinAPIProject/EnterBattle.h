#pragma once
#include "CBattleState.h"

class EnterBattle :
    public CBattleState
{
public:
    virtual void Init(CScene_Battle* _pScene) override;

public:
    Vec2 GetRandomGridPos();
};


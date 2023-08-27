#pragma once
#include "CBattleState.h"

class CScene_Battle;

class PlayerTurn_TileSelect :
    public CBattleState
{
public:
    virtual void Handle(CScene_Battle* _pScene) override;
};


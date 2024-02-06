#pragma once
#include "CBattleState.h"

class CScene_Battle;

class PlayerTurn_TileSelect :
    public CBattleState
{
public:
    virtual void OnTileSelect(CScene_Battle* _pScene, CObject* _pObj) override;
    virtual void Handle(CScene_Battle* _pScene) override;
};


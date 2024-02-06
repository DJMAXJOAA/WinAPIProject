#pragma once
#include "CBattleState.h"

class PlayerStart :
    public CBattleState
{
public:
    virtual void Init(CScene_Battle* _pScene) override;
    virtual void OnTileSelect(CScene_Battle* _pScene, CObject* _pObj) override;
    virtual void Handle(CScene_Battle* _pScene) override;
};


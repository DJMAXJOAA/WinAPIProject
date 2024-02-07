#pragma once
#include "CBattleState.h"

class PlayerTurn_Skill :
    public CBattleState
{
private:
    bool m_bUseSkill;

public:
    virtual void Init(CScene_Battle* _pScene) override;
    virtual void Handle(CScene_Battle* _pScene) override;
};


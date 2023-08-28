#pragma once
#include "CBattleState.h"

class CMonsterStrategy;

class EnemyTurn_Move :
    public CBattleState
{
private:
    vector<CMonsterStrategy*>   m_vecStrategies;    // 몬스터 패턴을 모아둔 배열
    CMonsterStrategy*           m_MonsterStrategy;  // 현재 몬스터의 패턴

public:
    EnemyTurn_Move();
    ~EnemyTurn_Move();

public:
    virtual void Handle(CScene_Battle* _pScene) override;
};


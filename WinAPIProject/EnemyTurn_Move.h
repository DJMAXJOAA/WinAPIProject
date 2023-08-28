#pragma once
#include "CBattleState.h"

class CMonsterStrategy;

class EnemyTurn_Move :
    public CBattleState
{
private:
    vector<CMonsterStrategy*>   m_vecStrategies;    // ���� ������ ��Ƶ� �迭
    CMonsterStrategy*           m_MonsterStrategy;  // ���� ������ ����

public:
    EnemyTurn_Move();
    ~EnemyTurn_Move();

public:
    virtual void Handle(CScene_Battle* _pScene) override;
};


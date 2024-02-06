#pragma once
#include "CBattleState.h"

class CMonsterStrategy;
class CMonster;
class CPlayer;

class EnemyTurn_Move :
    public CBattleState
{
private:
    vector<CMonsterStrategy*>   m_vecStrategies;    // ���� ������ ��Ƶ� �迭
    CMonsterStrategy*           m_MonsterStrategy;  // ���� ������ ����
    bool                        m_bRouteCalculate;  // ��ã�� ��Ʈ Ȯ���ߴ���

public:
    EnemyTurn_Move();
    virtual ~EnemyTurn_Move();

public:
    virtual void Init(CScene_Battle* _pScene) override;
    virtual void Handle(CScene_Battle* _pScene) override;
    bool IsMonstersMovingDone(const list<CMonster*>& monsterList);
};


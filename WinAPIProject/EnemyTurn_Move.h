#pragma once
#include "CBattleState.h"

class CMonsterStrategy;
class CMonster;
class CPlayer;

class EnemyTurn_Move :
    public CBattleState
{
private:
    vector<CMonsterStrategy*>   m_vecStrategies;    // 몬스터 패턴을 모아둔 배열
    CMonsterStrategy*           m_MonsterStrategy;  // 현재 몬스터의 패턴
    bool                        m_bRouteCalculate;  // 길찾기 루트 확정했는지

public:
    EnemyTurn_Move();
    virtual ~EnemyTurn_Move();

public:
    virtual void Init(CScene_Battle* _pScene) override;
    virtual void Handle(CScene_Battle* _pScene) override;
    bool IsMonstersMovingDone(const list<CMonster*>& monsterList);
};


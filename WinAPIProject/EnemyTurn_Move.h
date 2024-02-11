#pragma once
#include "CBattleState.h"

class IMonsterStrategy;
class CMonster;
class CPlayer;

class EnemyTurn :
    public CBattleState
{
private:
    vector<IMonsterStrategy*>   m_vecStrategies;    // ���� ������ ��Ƶ� �迭

public:
    EnemyTurn();
    virtual ~EnemyTurn();

public:
    virtual void Init(CScene_Battle* _pScene) override;
    virtual void Handle(CScene_Battle* _pScene) override;
    bool IsMonstersMovingDone(const list<CMonster*>& monsterList);
};


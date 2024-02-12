#pragma once
#include "CBattleState.h"

class IMonsterStrategy;
class CMonster;
class CScene_Battle;

class EnemyTurn :
    public CBattleState
{
private:
    vector<IMonsterStrategy*>   m_vecStrategies;    // 몬스터 패턴을 모아둔 배열

public:
    EnemyTurn();
    virtual ~EnemyTurn();

public:
    virtual void Init(CScene_Battle* _pScene) override;
    virtual void Handle(CScene_Battle* _pScene) override;

private:
    void InitMonster(CScene_Battle* _pScene, CMonster* _pMonster);

private:
    void AttackPlayer(CScene_Battle* _pScene, CMonster* _pMonster);
    void SetMonsterActingDone(CScene_Battle* _pScene, CMonster* _pMonster);
    void ChangeTurnToPlayer(CScene_Battle* _pScene);

private:
    bool IsFoundObjects(CScene_Battle* _pScene, CMonster* _pMonster);
    bool IsFoundResultPlayer(CScene_Battle* _pScene, CMonster* _pMonster);
    bool IsAllMonstersActingDone(const list<CMonster*>& monsterList);
};


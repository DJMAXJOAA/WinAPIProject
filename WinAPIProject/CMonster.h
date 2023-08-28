#pragma once
#include "node.h"
#include "CObject.h"

enum class MONSTER_STATE
{
    IDLE,
    MOVE,
    DAMAGED,
    ATTACK,
    SKILL,
};

class MonsterData;

class CMonster :
    public CObject
{
private:
    wstring                     m_strName;
    vector<MONSTER_STRATEGY>    m_vecStrategy;
    MONSTER_STRATEGY            m_currentStrategy;
    float                       m_fMaxHP;
    float                       m_fHP;
    float                       m_fAtt;
    int                         m_iMove;
    int                         m_iRange;

    vector<Node*>               m_nodeList;     // 길찾기 경로 저장

public:
    CMonster(int _key);
    CMonster(MonsterData* _data);
    CMonster(const CMonster& _origin);
    virtual ~CMonster();

    CLONE(CMonster)

public:
    MONSTER_STRATEGY GetStrategy() { return m_currentStrategy; }
    int GetMove() { return m_iMove; }

public:
    MONSTER_STRATEGY RandomPattern();

public:
    void GetDamaged(float _damaged);
    void Died(CObject* _pMonster);

public:
    virtual void Update();
    virtual void Render(HDC hdc);
};


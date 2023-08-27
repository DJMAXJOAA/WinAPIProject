#pragma once
#include "CObject.h"

enum class MONSTER_STATE
{
    IDEL,
    MOVE,
    DAMAGED,
    ATTACK,
    SKILL,
};

class MonsterData;
class IMonsterStrategy;

class CMonster :
    public CObject
{
private:
    wstring                     m_strName;
    vector<IMonsterStrategy*>   m_vecStrategy;
    float                       m_fMaxHP;
    float                       m_fHP;
    float                       m_fAtt;
    int                         m_iMove;
    int                         m_iRange;

public:
    CMonster(int _key);
    CMonster(MonsterData* _data);
    CMonster(const CMonster& _origin);
    virtual ~CMonster();

    CLONE(CMonster)

public:
    void MovePattern(IMonsterStrategy* _stratey);

public:
    void GetDamaged(float _damaged);
    void Died(CObject* _pMonster);

public:
    virtual void Update();
    virtual void Render(HDC hdc);
};


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
    int                         m_animationInterval;        // �ִϸ��̼� ����

    MONSTER_STRATEGY            m_currentStrategy;
    MONSTER_STATE               m_monsterState;
    GRID_DIRECTION              m_monsterDirection;
    float                       m_fMaxHP;
    float                       m_fHP;
    float                       m_fAtt;
    int                         m_iMove;
    int                         m_iRange;
    
    float                       m_fSpeed;       // ���� �̵��ӵ�(������ �̵��ӵ�)

    list <Vec2>                 m_lstRoute;     // �� �Ͽ� ��ã�� ��Ʈ ����

public:
    CMonster(int _key);
    CMonster(MonsterData* _data);
    CMonster(const CMonster& _origin);
    virtual ~CMonster();

    CLONE(CMonster)

public:
    MONSTER_STRATEGY GetStrategy() { return m_currentStrategy; }
    int GetMove() { return m_iMove; }
    MONSTER_STATE GetState() { return m_monsterState; }
    list<Vec2>& GetRoute() { return m_lstRoute; }

    void SetState(MONSTER_STATE _state) { m_monsterState = _state; }
    void SetRoute(list<Vec2> _route) { m_lstRoute = _route; }

public:
    void Move(GRID_DIRECTION _aniDirection, Vec2 _vDestination);
    void GetDamaged(float _damaged);
    void Died(CObject* _pMonster);

public:
    MONSTER_STRATEGY RandomPattern();

    void AnimationDirection(wstring _anim, bool _bRepeat, GRID_DIRECTION _aniDirection);
    void AnimationDirection(wstring _anim, bool _bRepeat);

public:
    virtual void Update();
    virtual void Render(HDC hdc);
};


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
class CPlayer;

class CMonster :
    public CObject
{
private:
    wstring                     m_strName;
    vector<MONSTER_STRATEGY>    m_vecStrategy;

    MONSTER_STRATEGY            m_currentStrategy;
    MONSTER_STATE               m_monsterState;
    GRID_DIRECTION              m_monsterDirection;
    float                       m_fMaxHP;
    float                       m_fHP;
    float                       m_fAtt;
    int                         m_iMove;
    int                         m_iRange;
    float                       m_fSpeed;       // 실제 이동속도(렌더링 이동속도)
    
    bool                        m_bActionFinish;    // 행동 종료 여부
    list <Vec2>                 m_lstRoute;         // 적 턴에 길찾기 루트 저장


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
    int GetRange() { return m_iRange; }

    void SetState(MONSTER_STATE _state) { m_monsterState = _state; }
    void SetRoute(list<Vec2> _route) { m_lstRoute = _route; }
    void SetActing(bool _bTF) { m_bActionFinish = _bTF; }

    bool IsActingDone() { return m_bActionFinish; }

public:
    void Move(GRID_DIRECTION _aniDirection, Vec2 _vDestination);
    void Attack(GRID_DIRECTION _aniDirection, CPlayer* _pPlayer);
    void Skill() {};

private:
    virtual void AnimationEvent() override;
    virtual void AnimationEnd() override;

    void AttackStartEvent(float _damage);
    void AttackDoneEvent();

    void Died();
public:
    void GetDamaged(float _damaged);

public:
    MONSTER_STRATEGY RandomPattern();

    void AnimationDirection(wstring _anim, bool _bRepeat, GRID_DIRECTION _aniDirection);
    void AnimationDirection(wstring _anim, bool _bRepeat);

public:
    virtual void Update();
    virtual void Render(HDC hdc);
};


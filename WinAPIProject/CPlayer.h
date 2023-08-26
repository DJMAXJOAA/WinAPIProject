#pragma once
#include "CObject.h"

enum class PLAYER_STATE
{
    IDEL = 100,
    MOVE,
    DAMAGED,
    ROLLING,
    LANDING,
    WIN,
    DASH,
    FIST,
    UPPERCUT,
    SWORD,
    LANCE,
    AXE,
    GUN,
    BOW,

    IDEL2,
    MOVE2,
    DAMAGED2,
    ROLLING2,
    LANDING2,
    WIN2,
    DASH2,
    FIST2,
    UPPERCUT2,
    SWORD2,
    LANCE2,
    AXE2,
    GUN2,
    BOW2,
};

class CPlayer :
    public CObject
{
private:
    PLAYER_STATE    m_playerState;  // 캐릭 상태

    float           m_fSpeed;       // 이동속도

public:
    CPlayer();
    virtual ~CPlayer();

private:
    CLONE(CPlayer)

public:
    PLAYER_STATE GetState() { return m_playerState; }

public:
    void SetState(PLAYER_STATE _state) { m_playerState = _state; }

public:
    void Move(Vec2 _vGridFront, Vec2 _vGridRear, Vec2 _vDestination);
    void Attack(Vec2 _vGridFront, Vec2 _vGridRear, Vec2 _vDestination);

public:
    virtual void Render(HDC hdc);
    virtual void Update();

private:
    virtual void AnimationEvent() override;
    virtual void AnimationEnd() override;
    void AnimationDirection(PLAYER_STATE _anim, bool _bRepeat, Vec2 _vGridFront, Vec2 _vGridRear, Vec2 _vDestination);     // 애니메이션 방향 지정
};


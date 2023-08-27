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
    PLAYER_STATE    m_playerState;  // ĳ�� ����

    CObject*       m_pTargetMonster;   // ���� Ÿ���� �� ����

    float           m_fSpeed;       // �̵��ӵ�
    float           m_fAtt;         // �÷��̾� ���ݷ�

public:
    CPlayer();
    virtual ~CPlayer();

private:
    CLONE(CPlayer)

public:
    PLAYER_STATE GetState() { return m_playerState; }
    CObject* GetTarget() { return m_pTargetMonster; }

public:
    void SetState(PLAYER_STATE _state) { m_playerState = _state; }
    void SetTarget(CObject* _pObj) { m_pTargetMonster = _pObj; }

public:
    void Move(GRID_DIRECTION _aniDirection, Vec2 _vDestination);
    void Attack(GRID_DIRECTION _aniDirection, CObject* pMon);

public:
    virtual void Render(HDC hdc);
    virtual void Update();

private:
    // �̺�Ʈ ȣ�� ����
    virtual void AnimationEvent() override;
    virtual void AnimationEnd() override;

    void PlayerAttackMonster(float _damage, CObject* _pMon);

    void AnimationDirection(PLAYER_STATE _anim, bool _bRepeat, GRID_DIRECTION _aniDirection);     // �ִϸ��̼� ���� ����
};


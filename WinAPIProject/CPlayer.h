#pragma once
#include "CObject.h"

enum class PLAYER_STATE
{
    IDLE = 100,
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
};

class CPlayer :
    public CObject
{
private:
    PLAYER_STATE    m_playerState;      // 캐릭 상태
    GRID_DIRECTION  m_playerDirection;  // 캐릭터 현재 방향

    CObject*       m_pTargetMonster;   // 현재 타게팅 된 몬스터

    float           m_fSpeed;       // 이동속도
    float           m_fAtt;         // 플레이어 공격력
    bool            m_bAttack;      // 공격 중 여부
    float           m_fCombo;       // 콤보 데미지(스킬 데미지에 가산)

public:
    CPlayer();
    virtual ~CPlayer();

private:
    CLONE(CPlayer)

public:
    PLAYER_STATE GetState() { return m_playerState; }
    CObject* GetTarget() { return m_pTargetMonster; }

    void SetState(PLAYER_STATE _state) { m_playerState = _state; }
    void SetTarget(CObject* _pObj) { m_pTargetMonster = _pObj; }
    void SetAttacking(bool _bTF) { m_bAttack = _bTF; }

    bool IsAttacking() { return m_bAttack; }

public:
    void Move(GRID_DIRECTION _aniDirection, Vec2 _vDestination);
    void PlayerAttackStart(GRID_DIRECTION _aniDirection, CObject* pMon);
    void PlayerSkillStart(int _iCombo);

private:
    // 이벤트 호출 받음
    virtual void AnimationEvent() override;
    virtual void AnimationEnd() override;

    void PlayerAttackMonster(float _damage, CObject* _pMon);
    void PlayerAttackDone();
    void PlayerSkillCast(float _value);
    void PlayerSkillDone();

    void AnimationDirection(PLAYER_STATE _anim, bool _bRepeat, GRID_DIRECTION _aniDirection);     // 애니메이션 방향 지정
    void AnimationDirection(PLAYER_STATE _anim, bool _bRepeat);

public:
    virtual void Render(HDC hdc);
    virtual void Update();

};


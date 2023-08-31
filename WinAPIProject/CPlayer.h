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



class CSound;

class CPlayer :
    public CObject
{
private:
    CSound*         m_pHitSound;     // 효과음

    PLAYER_STATE    m_playerState;      // 캐릭 상태
    GRID_DIRECTION  m_playerDirection;  // 캐릭터 현재 방향
    CObject*        m_pTargetMonster;   // 현재 타게팅 된 몬스터

    float           m_fSpeed;       // 이동속도
    float           m_fAtt;         // 플레이어 공격력
    float           m_fMaxHP;       // 플레이어 최대체력
    float           m_fHP;          // 플레이어 현재체력
    int             m_iMoney;       // 플레이어 현재 돈

    bool            m_bAttack;      // 공격 중 여부
    float           m_fCombo;       // 콤보 데미지(스킬 데미지에 가산)


public:
    CPlayer();
    virtual ~CPlayer();

private:
    CLONE(CPlayer)

public:
    virtual float GetHP() { return m_fHP; }
    virtual float GetMaxHP() { return m_fMaxHP; }
    PLAYER_STATE GetState() { return m_playerState; }
    CObject* GetTarget() { return m_pTargetMonster; }
    int GetMoney() { return m_iMoney; }

    void SetState(PLAYER_STATE _state) { m_playerState = _state; }
    void SetTarget(CObject* _pObj) { m_pTargetMonster = _pObj; }
    void SetAttacking(bool _bTF) { m_bAttack = _bTF; }
    void SetMoney(int _money) { m_iMoney = _money; }

    bool IsAttacking() { return m_bAttack; }

    void SetHP(float _hp) { m_fHP = _hp; }
    void SetAtt(float _att) { m_fAtt = _att; }  // 치트용

public:
    void Move(GRID_DIRECTION _aniDirection, Vec2 _vDestination);
    void Attack(GRID_DIRECTION _aniDirection, CObject* pMon);
    void Skill(int _iCombo);

private:
    // 이벤트 호출 받음
    virtual void AnimationEvent() override;
    virtual void AnimationEnd() override;

    void AttackStartEvent(float _damage, CObject* _pMon);
    void AttackDoneEvent();
    void SkillCastEvent(float _value);
    void SkillDoneEvent();

    void Died();

public:
    void GetDamaged(float _damaged);

public:
    void AnimationDirection(PLAYER_STATE _anim, bool _bRepeat, GRID_DIRECTION _aniDirection);     // 애니메이션 방향 지정
    void AnimationDirection(PLAYER_STATE _anim, bool _bRepeat);

public:
    virtual void Render(HDC hdc);
    virtual void Update();

};


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
    CSound*         m_pHitSound;     // ȿ����

    PLAYER_STATE    m_playerState;      // ĳ�� ����
    GRID_DIRECTION  m_playerDirection;  // ĳ���� ���� ����
    CObject*        m_pTargetMonster;   // ���� Ÿ���� �� ����

    float           m_fSpeed;       // �̵��ӵ�
    float           m_fAtt;         // �÷��̾� ���ݷ�
    float           m_fMaxHP;       // �÷��̾� �ִ�ü��
    float           m_fHP;          // �÷��̾� ����ü��
    int             m_iMoney;       // �÷��̾� ���� ��

    bool            m_bAttack;      // ���� �� ����
    float           m_fCombo;       // �޺� ������(��ų �������� ����)


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
    void SetAtt(float _att) { m_fAtt = _att; }  // ġƮ��

public:
    void Move(GRID_DIRECTION _aniDirection, Vec2 _vDestination);
    void Attack(GRID_DIRECTION _aniDirection, CObject* pMon);
    void Skill(int _iCombo);

private:
    // �̺�Ʈ ȣ�� ����
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
    void AnimationDirection(PLAYER_STATE _anim, bool _bRepeat, GRID_DIRECTION _aniDirection);     // �ִϸ��̼� ���� ����
    void AnimationDirection(PLAYER_STATE _anim, bool _bRepeat);

public:
    virtual void Render(HDC hdc);
    virtual void Update();

};


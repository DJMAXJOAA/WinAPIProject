#pragma once
#include "CObject.h"

class IMonsterStrategy;

class CMonster :
    public CObject
{
private:
    int     m_iHP;

public:
    CMonster();
    virtual ~CMonster();

    CLONE(CMonster);

public:
    void MovePattern(IMonsterStrategy* _stratey);

public:
    virtual void AnimationFinishEvent() override {}

public:
    virtual void OnCollisionEnter(CCollider* _pOther);

public:
    virtual void Update();
    virtual void Render(HDC hdc);
};


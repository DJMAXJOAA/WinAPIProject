#pragma once
#include "CObject.h"

class AI;

class CMonster :
    public CObject
{
private:
    float   m_fSpeed;
    int     m_iHP;

    AI*     m_pAI;

public:
    CMonster();
    ~CMonster();

public:
    float GetSpeed() { return m_fSpeed; }

public:
    void SetSpeed(float _f) { m_fSpeed = _f; }
    /*void SetAI(AI* _AI) { m_pAI = _AI; m_pAI->m_pOwner = this; }*/

public:
    virtual void OnCollisionEnter(CCollider* _pOther);

public:
    virtual void Update();

    CLONE(CMonster);
};


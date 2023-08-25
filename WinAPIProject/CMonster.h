#pragma once
#include "CObject.h"

class IMonsterStrategy;

class CMonster :
    public CObject
{
private:
    float   m_fHP;
    float   m_fAtt;
    int     m_iMove;
    int     m_iRange;

public:
    CMonster();
    virtual ~CMonster();

    CLONE(CMonster);

public:
    void MovePattern(IMonsterStrategy* _stratey);

public:
    virtual void Update();
    virtual void Render(HDC hdc);
};


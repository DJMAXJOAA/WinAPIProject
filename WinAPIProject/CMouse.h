#pragma once
#include "CObject.h"
class CMouse :
    public CObject
{
public:
    CMouse();
    virtual ~CMouse();

private:
    CLONE(CMouse);

public:
    virtual void AnimationFinishEvent() override {}

public:
    virtual void OnCollisionEnter(CCollider* _pOther);

public:
    virtual void Render(HDC hdc);
    virtual void Update();
};


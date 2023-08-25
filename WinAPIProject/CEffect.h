#pragma once
#include "CObject.h"

class CEffect :
    public CObject
{
public:
    CEffect();
    virtual ~CEffect();

public:
    virtual void Render(HDC hdc);
    virtual void Update();

    CLONE(CEffect);

public:
    virtual void AnimationEvent() override { DeleteObj(this); }
};


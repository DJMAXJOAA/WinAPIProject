#pragma once
#include "CObject.h"

class CEffect :
    public CObject
{
private:
    bool    m_bRendered;

public:
    CEffect(int _animator, Vec2 _vPos);
    virtual ~CEffect();

public:
    virtual void Render(HDC hdc);
    virtual void Update();

    CLONE(CEffect)

public:
    virtual void AnimationEnd() override { m_bRendered = false; DeleteObj(this); }
};


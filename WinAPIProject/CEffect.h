#pragma once
#include "CObject.h"

class CEffect :
    public CObject
{
public:
    CEffect();
    ~CEffect();

public:
    virtual void Render(HDC hdc);
    virtual void Update();

    CLONE(CEffect);
};


#pragma once
#include "CObject.h"
class CMouse :
    public CObject
{
public:
    CMouse();
    virtual ~CMouse();

public:
    virtual void Render(HDC hdc);
    virtual void Update();

private:
    CLONE(CMouse);
};


#pragma once
#include "CObject.h"
class CMouse :
    public CObject
{
public:
    CMouse();
    ~CMouse();

public:
    virtual void Render(HDC hdc);
    virtual void Update();

private:
    CLONE(CMouse);
};


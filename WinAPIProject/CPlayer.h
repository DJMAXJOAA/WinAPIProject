#pragma once
#include "CObject.h"

class CTexture;

class CPlayer :
    public CObject
{
private:

public:
    CPlayer();
    ~CPlayer();

public:
    virtual void Render(HDC hdc);
    virtual void Update();

private:
    CLONE(CPlayer);

private:
    void CreateMissile();   // 酒公唱 积己 给窍霸 private
};


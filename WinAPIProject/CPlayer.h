#pragma once
#include "CObject.h"

class CPlayer :
    public CObject
{
public:
    CPlayer();
    virtual ~CPlayer();

public:
    virtual void Render(HDC hdc);
    virtual void Update();

private:
    CLONE(CPlayer);

private:
    void CreateMissile();   // �ƹ��� ���� ���ϰ� private
};


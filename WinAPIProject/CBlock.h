#pragma once
#include "CObject.h"

class CBlock :
    public CObject
{
private:
    MAP_TYPE    m_MapType;
    int         m_BlockState;

public:
    CBlock(MAP_TYPE _type);
    virtual ~CBlock();

    CLONE(CBlock);

public:
    int RandomBlock();

public:
    virtual void Update();
    virtual void Render(HDC hdc);
};


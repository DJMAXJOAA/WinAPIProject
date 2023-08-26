#pragma once
#include "CObject.h"

class CBlock :
    public CObject
{
private:
    BLOCK_TYPE    m_MapType;
    int         m_BlockState;

public:
    CBlock(BLOCK_TYPE _type);
    virtual ~CBlock();

    CLONE(CBlock)

public:
    int RandomBlock();

public:
    virtual void Update();
    virtual void Render(HDC hdc);
};



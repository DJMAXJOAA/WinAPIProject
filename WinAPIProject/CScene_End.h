#pragma once
#include "CScene.h"

class CScene_End :
    public CScene
{
public:
    CScene_End();
    ~CScene_End();

public:
    virtual void Update();
    virtual void Enter();
    virtual void Exit();
};


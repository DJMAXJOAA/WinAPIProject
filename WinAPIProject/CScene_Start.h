#pragma once
#include "CScene.h"

class CSound;

class CScene_Start :
    public CScene
{
private:
    CSound*     m_pBGM; // 노래 실행

public:
    CScene_Start();
    virtual ~CScene_Start();

public:
    virtual void Update();
    virtual void Enter();
    virtual void Exit();
};


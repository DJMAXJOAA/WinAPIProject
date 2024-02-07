#pragma once
#include "CScene.h"

class CSound;

class CScene_Start :
    public CScene
{
private:
    CSound*     m_pBGM; // �뷡 ����

public:
    CScene_Start();
    virtual ~CScene_Start();

public:
    virtual void Update();
    virtual void Enter();
    virtual void Exit();
};


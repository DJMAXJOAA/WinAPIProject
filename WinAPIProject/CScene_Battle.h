#pragma once
#include "CScene.h"

class CScene_Battle :
    public CScene
{
private:
    vector<vector<int>>     m_vecTileState;
    map<Vec2, Vec2>        m_mapPoint;

public:
    CScene_Battle();
    ~CScene_Battle();

public:
    virtual void Update();
    virtual void Enter();
    virtual void Exit();
};


#pragma once
#include "CScene.h"

class CUI;

class CScene_Tool :
    public CScene
{
private:
    CUI*        m_pUI;
 
public:
    CScene_Tool();
    ~CScene_Tool();

public:
    void SaveTileData();
    void LoadTileData();
    void SaveTile(const wstring& _strRelativePath);

public:
    virtual void Update();
    virtual void Enter();
    virtual void Exit();
};


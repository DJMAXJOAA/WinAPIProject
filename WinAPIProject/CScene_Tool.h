#pragma once
#include "CScene.h"

class CUI;
class CToolTest;

class CScene_Tool :
    public CScene
{
private:
    CToolTest*  m_pDisplay;
    CUI*        m_pUI;
 
public:
    CScene_Tool();
    ~CScene_Tool();

public:
    void SetOffset(Vec2 _value);
    void AddOffset(Vec2 _value);
    void PrevFrame();
    void NextFrame();
    void SaveAnimation();

public:
    void SaveTileData();
    void LoadTileData();
    void SaveTile(const wstring& _strRelativePath);

public:
    virtual void Update();
    virtual void Enter();
    virtual void Exit();
};


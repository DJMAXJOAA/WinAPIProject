#pragma once
#include "CScene.h"

class CUI;
class CToolTest1;
class CToolTest2;

class CScene_Tool :
    public CScene
{
private:
    int         m_iCurrentAnimator;
    int         m_iCurrentAnimation;

private:
    CToolTest1* m_pDisplay;
    CToolTest2* m_pObj;
    CUI*        m_pUI;
 
public:
    CScene_Tool();
    ~CScene_Tool();

public:
    void SetCurrentAnimator(int _iValue) { m_iCurrentAnimator = _iValue; }
    void SetCurrentAnimation(int _iValue) { m_iCurrentAnimation = _iValue; }
    void SetOffset(Vec2 _value);
    void AddOffset(Vec2 _value);
    void PrevFrame();
    void NextFrame();
    void SaveAnimation();

public:
    virtual void Update();
    virtual void Enter();
    virtual void Exit();
};


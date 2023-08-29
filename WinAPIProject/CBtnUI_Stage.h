#pragma once
#include "CBtnUI.h"

class CTexture;

class CBtnUI_Stage :
    public CBtnUI
{
private:
    int                     m_iValue;
    vector<CBtnUI_Stage*>   m_vecChildren;

public:
    CBtnUI_Stage();
    ~CBtnUI_Stage();

    CLONE(CBtnUI)

public:
    vector<CBtnUI_Stage*>& GetChildren() { return m_vecChildren; }
    int GetValue() { return m_iValue; }

    void SetValue(int _value) { m_iValue = _value; }

    void AddChild(CBtnUI_Stage* child) { m_vecChildren.push_back(child); }

public:
    virtual void MouseOn();
    virtual void MouseLbtnDown();
    virtual void MouseLbtnUp();
    virtual void MouseLbtnClicked();

public:
    virtual void Render(HDC hdc) override;
    virtual void Update() override;
};


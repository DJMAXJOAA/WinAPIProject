#pragma once

#include "CUI.h"
class CPanelUI :
    public CUI
{
private:
    Vec2        m_vDragStart;
    bool        m_bDragPossible;

public:
    CPanelUI(bool _bCamAff, bool _bDrag = false);
    ~CPanelUI();

    CLONE(CPanelUI)

public:
    virtual void Update();
    virtual void Render(HDC hdc);

public:
    virtual void MouseOn();
    virtual void MouseLbtnDown();
    virtual void MouseLbtnUp();
}; 


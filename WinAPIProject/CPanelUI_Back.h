#pragma once
#include "CPanelUI.h"

class CTexture;

class CPanelUI_Back :
    public CPanelUI
{
private:
    CTexture*       m_pTexture;

public:
    CPanelUI_Back();
    ~CPanelUI_Back();

    CLONE(CPanelUI_Back)

public:
    virtual void Update() override;
    virtual void Render(HDC hdc) override;
};


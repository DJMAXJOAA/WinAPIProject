#pragma once
#include "CBtnUI.h"

class CBtnUI_Start :
    public CBtnUI
{
private:
    vector<Image*>  m_vecImage;
    Image*          m_pImage;
    
    CLONE(CBtnUI_Start)

public:
    CBtnUI_Start(vector<wstring> _filenames);
    ~CBtnUI_Start();

public:
    virtual void MouseOn() override;
    virtual void MouseLbtnDown() override;
    virtual void MouseLbtnUp() override;
    virtual void MouseLbtnClicked() override;

public:
    virtual void MouseOnCheck() override;

public:
    virtual void Render(HDC hdc) override;
    virtual void Update() override;
};


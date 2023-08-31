#pragma once
#include "CPanelUI.h"

class CPanelUI_Number :
    public CPanelUI
{
private:
    int                 m_iNumber;      // 현재 숫자
    float               m_fScale;       // 숫자 이미지 배율
    vector<Image*>      m_vecNumbers;   // 0~9 이미지 배열

    float		        m_fLifeTime;	// 숫자 오브젝트 수명
    float               m_fAlpha;       // 수명이 줄어들수록 투명화됨

public:
    CPanelUI_Number(int _number, Vec2 _vPos, float _scale = 1.0f, float _lifetime = 10000.f);
    ~CPanelUI_Number();

    CLONE(CPanelUI_Number)

private:
    void InitImages();

public:
    void SetNumber(int _number) { m_iNumber = _number; }

public:
    virtual void Update() override;
    virtual void Render(HDC hdc) override;
};


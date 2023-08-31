#pragma once
#include "CPanelUI.h"

class CPanelUI_Number :
    public CPanelUI
{
private:
    int                 m_iNumber;      // ���� ����
    float               m_fScale;       // ���� �̹��� ����
    vector<Image*>      m_vecNumbers;   // 0~9 �̹��� �迭

    float		        m_fLifeTime;	// ���� ������Ʈ ����
    float               m_fAlpha;       // ������ �پ����� ����ȭ��

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


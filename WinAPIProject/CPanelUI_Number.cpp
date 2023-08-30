#include "pch.h"
#include "CPanelUI_Number.h"

#include "CPathMgr.h"
#include "CTimeMgr.h"

CPanelUI_Number::CPanelUI_Number(int _number, Vec2 _vPos, float _scale, float _lifetime)
    : CPanelUI(false)
    , m_vecNumbers(10)
    , m_iNumber(_number)
    , m_fScale(_scale)
    , m_fLifeTime(_lifetime)
    , m_fAlpha(255.f)
{
    SetPos(_vPos);
    InitImages();
}

CPanelUI_Number::~CPanelUI_Number()
{
    SafeDeleteVec(m_vecNumbers);
}

void CPanelUI_Number::InitImages()
{
    wstring strFilePath = CPathMgr::GetInstance()->GetContentPath();
    wstring base = L"texture\\UI\\Number\\";

    // 0~9���� ���� �̹����� �迭�� �־���
    for (int i = 0; i <= 9; ++i) 
    {
        wstring temp = strFilePath + base + to_wstring(i) + L".png";
        m_vecNumbers[i] = new Image(temp.c_str());
    }
}

void CPanelUI_Number::Update()
{   
    // ������� ���� ���ڵ��� �� ����
    if (m_fLifeTime >= 10000.f)
        return;

    float fAlphaDropRate = 255.0f / m_fLifeTime;
    m_fAlpha -= fAlphaDropRate * fDT;

    // ���İ��� 0 �̸��� �Ǹ� 0���� ����
    if (m_fAlpha < 0)
    {
        m_fAlpha = 0;
    }

    // ���� ����
    m_fLifeTime -= fDT;
    if (m_fLifeTime <= 0)
    {
        // ������ ���ϸ� ���� �̺�Ʈ�� �߻�
        DeleteObj(this);
    }
}

void CPanelUI_Number::Render(HDC hdc)
{
    Graphics graphics(hdc);

    Vec2 vPos = GetPos();
    string strNum = to_string(m_iNumber);
    int xOffset = 0;

    ImageAttributes imgAttr;
    ColorMatrix colorMatrix = {
        1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 0.0f, m_fAlpha / 255.0f, 0.0f,  // Alpha�� ����
        0.0f, 0.0f, 0.0f, 0.0f, 1.0f
    };
    imgAttr.SetColorMatrix(&colorMatrix);

    // �������� ���� �̹����� �ٲ���
    for (char c : strNum) 
    {
        int digit = c - '0';
        if (digit >= 0 && digit <= 9) 
        {
            Image* img = m_vecNumbers[digit];

            float scaledWidth = img->GetWidth() * m_fScale;
            float scaledHeight = img->GetHeight() * m_fScale;

            RectF destRect(vPos.x + xOffset, vPos.y, scaledWidth, scaledHeight);
            graphics.DrawImage(img, destRect, 0, 0, img->GetWidth(), img->GetHeight(), UnitPixel, &imgAttr);

            xOffset += (int)scaledWidth;  // ���� ���� �̹����� �׸� ��ġ�� ������Ʈ
        }
    }
}

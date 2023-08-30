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

    // 0~9까지 숫자 이미지를 배열에 넣어줌
    for (int i = 0; i <= 9; ++i) 
    {
        wstring temp = strFilePath + base + to_wstring(i) + L".png";
        m_vecNumbers[i] = new Image(temp.c_str());
    }
}

void CPanelUI_Number::Update()
{   
    // 사라질일 없는 숫자들은 걍 무시
    if (m_fLifeTime >= 10000.f)
        return;

    float fAlphaDropRate = 255.0f / m_fLifeTime;
    m_fAlpha -= fAlphaDropRate * fDT;

    // 알파값이 0 미만이 되면 0으로 고정
    if (m_fAlpha < 0)
    {
        m_fAlpha = 0;
    }

    // 수명 감소
    m_fLifeTime -= fDT;
    if (m_fLifeTime <= 0)
    {
        // 수명이 다하면 삭제 이벤트를 발생
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
        0.0f, 0.0f, 0.0f, m_fAlpha / 255.0f, 0.0f,  // Alpha값 설정
        0.0f, 0.0f, 0.0f, 0.0f, 1.0f
    };
    imgAttr.SetColorMatrix(&colorMatrix);

    // 정수값을 숫자 이미지로 바꿔줌
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

            xOffset += (int)scaledWidth;  // 다음 숫자 이미지를 그릴 위치를 업데이트
        }
    }
}

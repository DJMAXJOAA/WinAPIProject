#include "pch.h"
#include "CHPBar.h"

#include "CPlayer.h"
#include "CMonster.h"

CHPBar::CHPBar(int _width, int _height, Vec2 _offset)
	: m_pOwner(nullptr)
	, m_vOffsetPos(_offset)
	, m_iWidth(_width)
	, m_iHeight(_height)
{
}

CHPBar::CHPBar(const CHPBar& _origin)
	: m_pOwner(nullptr)
	, m_vOffsetPos(_origin.m_vOffsetPos)
	, m_iWidth(_origin.m_iWidth)
	, m_iHeight(_origin.m_iHeight)
	, m_vFinalPos(_origin.m_vFinalPos)
{
}

CHPBar::~CHPBar()
{
}

void CHPBar::DrawTextWithBorder(Graphics& graphics, const std::wstring& text, const Font& font, const PointF& point)
{
    RectF boundingBox;
    StringFormat stringFormat;
    stringFormat.SetAlignment(StringAlignmentNear);
    graphics.MeasureString(text.c_str(), -1, &font, point, &stringFormat, &boundingBox);

    // 중앙 피벗 조절
    PointF adjustedPoint = PointF(
        point.X - boundingBox.Width / 2,
        point.Y - boundingBox.Height / 2
    );

    // 테두리 -> 검정
    SolidBrush borderBrush(Color(255, 0, 0, 0));

    // 테두리 -> 흰색 
    SolidBrush textBrush(Color(255, 255, 255, 255));

    // 테두리 그리기
    for (int x = -1; x <= 1; ++x)
    {
        for (int y = -1; y <= 1; ++y)
        {
            PointF newPoint = PointF(adjustedPoint.X + x, adjustedPoint.Y + y);
            graphics.DrawString(text.c_str(), -1, &font, newPoint, &stringFormat, &borderBrush);
        }
    }

    // 텍스트 그리기
    graphics.DrawString(text.c_str(), -1, &font, adjustedPoint, &stringFormat, &textBrush);
}

void CHPBar::FinalUpdate()
{
	Vec2 vObjectPos = m_pOwner->GetPos();
	m_vFinalPos = vObjectPos + m_vOffsetPos;
}

void CHPBar::Render(HDC hdc)
{
    Graphics graphics(hdc);

    Vec2 vRenderPos = CCamera::GetInstance()->GetRenderPos(m_vFinalPos);

    // 체력바의 최종 위치 계산
    int x = int(vRenderPos.x + m_vOffsetPos.x - m_iWidth / 2);
    int y = int(vRenderPos.y + m_vOffsetPos.y - m_iHeight / 2);

    // 체력바 위의 글씨 그리기
    float MaxHP, currentHP;
    if (m_bMonster)
    {
        MaxHP = ((CMonster*)m_pOwner)->GetMaxHP();
        currentHP = ((CMonster*)m_pOwner)->GetHP();
    }
    else
    {
        MaxHP = ((CPlayer*)m_pOwner)->GetMaxHP();
        currentHP = ((CPlayer*)m_pOwner)->GetHP();
    }
    wstring healthStr = to_wstring((int)currentHP);

    // 체력바 그리기
    float healthPercent = currentHP / MaxHP; // 이 값은 실제 게임 로직에서 가져와야 함

    // 바탕 그리기
    SolidBrush bgBrush(Color(255, 0, 0));  // 빨간색
    graphics.FillRectangle(&bgBrush, x, y, m_iWidth, m_iHeight);

    // 실제 체력 그리기
    SolidBrush healthBrush(Color(0, 255, 0));  // 초록색
    int healthWidth = static_cast<int>(m_iWidth * healthPercent);
    graphics.FillRectangle(&healthBrush, x, y, healthWidth, m_iHeight);

    // 테두리 그리기 (옵션)
    Pen borderPen(Color(255, 255, 255));  // 흰색
    graphics.DrawRectangle(&borderPen, x, y, m_iWidth, m_iHeight);

    // 체력 숫자를 그릴 폰트와 형식 지정
    FontFamily fontFamily(L"Arial");
    Font font(&fontFamily, 12);
    StringFormat format;
    format.SetAlignment(StringAlignmentCenter);

    // 체력 숫자 그리기
    SolidBrush textBrush(Color(255, 255, 255)); // 흰색
    PointF pointF(static_cast<REAL>(vRenderPos.x), static_cast<REAL>(y - 10));  // 체력 바 위에 20픽셀 올려서 그림
    DrawTextWithBorder(graphics, healthStr.c_str(), font, pointF);
}


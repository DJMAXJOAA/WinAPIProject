#include "pch.h"
#include "CHPBar.h"

#include "CCore.h"

#include "CObject.h"

CHPBar::CHPBar(int _width, int _height, Vec2 _offset)
	: m_pOwner(nullptr)
	, m_vOffsetPos(_offset)
	, m_iWidth(_width)
	, m_iHeight(_height)
	, m_vFinalPos{}
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
    int x = vRenderPos.x + m_vOffsetPos.x;
    int y = vRenderPos.y + m_vOffsetPos.y;

    float healthPercent = 0.5; // 이 값은 실제 게임 로직에서 가져와야 함

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
}


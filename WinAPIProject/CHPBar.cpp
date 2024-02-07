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

    // �߾� �ǹ� ����
    PointF adjustedPoint = PointF(
        point.X - boundingBox.Width / 2,
        point.Y - boundingBox.Height / 2
    );

    // �׵θ� -> ����
    SolidBrush borderBrush(Color(255, 0, 0, 0));

    // �׵θ� -> ��� 
    SolidBrush textBrush(Color(255, 255, 255, 255));

    // �׵θ� �׸���
    for (int x = -1; x <= 1; ++x)
    {
        for (int y = -1; y <= 1; ++y)
        {
            PointF newPoint = PointF(adjustedPoint.X + x, adjustedPoint.Y + y);
            graphics.DrawString(text.c_str(), -1, &font, newPoint, &stringFormat, &borderBrush);
        }
    }

    // �ؽ�Ʈ �׸���
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

    // ü�¹��� ���� ��ġ ���
    int x = int(vRenderPos.x + m_vOffsetPos.x - m_iWidth / 2);
    int y = int(vRenderPos.y + m_vOffsetPos.y - m_iHeight / 2);

    // ü�¹� ���� �۾� �׸���
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

    // ü�¹� �׸���
    float healthPercent = currentHP / MaxHP; // �� ���� ���� ���� �������� �����;� ��

    // ���� �׸���
    SolidBrush bgBrush(Color(255, 0, 0));  // ������
    graphics.FillRectangle(&bgBrush, x, y, m_iWidth, m_iHeight);

    // ���� ü�� �׸���
    SolidBrush healthBrush(Color(0, 255, 0));  // �ʷϻ�
    int healthWidth = static_cast<int>(m_iWidth * healthPercent);
    graphics.FillRectangle(&healthBrush, x, y, healthWidth, m_iHeight);

    // �׵θ� �׸��� (�ɼ�)
    Pen borderPen(Color(255, 255, 255));  // ���
    graphics.DrawRectangle(&borderPen, x, y, m_iWidth, m_iHeight);

    // ü�� ���ڸ� �׸� ��Ʈ�� ���� ����
    FontFamily fontFamily(L"Arial");
    Font font(&fontFamily, 12);
    StringFormat format;
    format.SetAlignment(StringAlignmentCenter);

    // ü�� ���� �׸���
    SolidBrush textBrush(Color(255, 255, 255)); // ���
    PointF pointF(static_cast<REAL>(vRenderPos.x), static_cast<REAL>(y - 10));  // ü�� �� ���� 20�ȼ� �÷��� �׸�
    DrawTextWithBorder(graphics, healthStr.c_str(), font, pointF);
}


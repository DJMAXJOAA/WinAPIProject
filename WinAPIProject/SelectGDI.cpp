#include "pch.h"
#include "SelectGDI.h"

#include "CCore.h"

SelectGDI::SelectGDI(HDC hdc, PEN_TYPE _ePenType)
	: m_hdc(hdc)
	, m_hDefaultBrush(nullptr)
	, m_hDefaultPen(nullptr)
{
	HPEN hPen = CCore::GetInstance()->GetPen(_ePenType);
	m_hDefaultPen = (HPEN)SelectObject(hdc, hPen);
}

SelectGDI::SelectGDI(HDC hdc, BRUSH_TYPE _eBrushType)
	: m_hdc(hdc)
	, m_hDefaultBrush(nullptr)
	, m_hDefaultPen(nullptr)
{
	HBRUSH hBrush = CCore::GetInstance()->GetBrush(_eBrushType);
	m_hDefaultBrush = (HBRUSH)SelectObject(hdc, hBrush);
}

SelectGDI::~SelectGDI()
{
	// Ư�� Ŭ������ �Լ��� ���� ����� ��, �� ��ü�� ������� ���� ��ü���� SelectGDI�� ������� ��
	// �׷��� �ڿ������� �Ҹ��ڰ� ȣ��Ǿ �Ҵ��ߴ� ��� �귯�ð� dc�� ��ȯ�ȴ�
	SelectObject(m_hdc, m_hDefaultBrush);
	SelectObject(m_hdc, m_hDefaultPen);
}

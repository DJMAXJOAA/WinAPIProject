#include "pch.h"
#include "CPanelUI.h"

#include "CKeyMgr.h"

CPanelUI::CPanelUI()
	: CUI(false)
{
	
}

CPanelUI::~CPanelUI()
{

}

void CPanelUI::Update()
{
	
}

void CPanelUI::Render(HDC hdc)
{
	CUI::Render(hdc);	// ����� �θ��� Render�� ���, ����� Ȯ���ϱ� �����ϱ�
}

void CPanelUI::MouseOn()
{
	// �г�UI�� �巡�� ����� �߰������ش�
	if (IsLbtnDown())
	{
		Vec2 vDiff = MOUSE_POS - m_vDragStart;

		Vec2 vCurPos = GetPos();
		vCurPos += vDiff;
		SetPos(vCurPos);

		m_vDragStart = MOUSE_POS;
	}
}

void CPanelUI::MouseLbtnDown()
{
	m_vDragStart = MOUSE_POS;
}

void CPanelUI::MouseLbtnUp()
{
}

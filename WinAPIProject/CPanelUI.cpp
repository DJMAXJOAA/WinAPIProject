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
	CUI::Render(hdc);	// 사용은 부모의 Render를 사용, 디버깅 확인하기 어려우니까
}

void CPanelUI::MouseOn()
{
	// 패널UI는 드래그 기능을 추가시켜준다
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

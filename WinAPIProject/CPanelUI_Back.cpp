#include "pch.h"
#include "CPanelUI_Back.h"

CPanelUI_Back::CPanelUI_Back()
	: CPanelUI(true)
	, m_pTexture(nullptr)
{
}

CPanelUI_Back::~CPanelUI_Back()
{
}

void CPanelUI_Back::Update()
{
}

void CPanelUI_Back::Render(HDC hdc)
{
	CUI::Render(hdc);
}
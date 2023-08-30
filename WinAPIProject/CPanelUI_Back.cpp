#include "pch.h"
#include "CPanelUI_Back.h"

#include "CResMgr.h"
#include "CPathMgr.h"

#include "CTexture.h"

CPanelUI_Back::CPanelUI_Back()
	: CPanelUI(true)
	, m_pTexture(nullptr)
{
	m_pTexture = CResMgr::GetInstance()->LoadTexture(L"BackPanel", L"texture\\Panel.bmp");
}

CPanelUI_Back::~CPanelUI_Back()
{
}

void CPanelUI_Back::Update()
{
	CUI::Update();
}

void CPanelUI_Back::Render(HDC hdc)
{
	/*CUI::Render(hdc);*/
	Vec2 vRenderPos = CCamera::GetInstance()->GetRenderPos(GetPos());

	TransparentBlt(hdc, (int)vRenderPos.x, (int)vRenderPos.y, m_pTexture->Width(), m_pTexture->Height(), m_pTexture->GetDC(), 0, 0, m_pTexture->Width(), m_pTexture->Height(), RGB(255, 0, 255));

	Render_Child(hdc);
}
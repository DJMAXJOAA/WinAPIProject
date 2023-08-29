#include "pch.h"
#include "CBtnUI_Stage.h"

#include "CCore.h"

CBtnUI_Stage::CBtnUI_Stage()
	: CBtnUI(true)
	, m_iValue(0)
	, m_vecChildren{}
{

}

CBtnUI_Stage::~CBtnUI_Stage()
{
}

void CBtnUI_Stage::MouseOn()
{
}

void CBtnUI_Stage::MouseLbtnDown()
{
}

void CBtnUI_Stage::MouseLbtnUp()
{
}

void CBtnUI_Stage::MouseLbtnClicked()
{
}

void CBtnUI_Stage::Render(HDC hdc)
{
	Graphics* graphics = CCore::GetInstance()->GetGraphics();
	Pen pen(Color(255, 0, 0, 255));

	Vec2 StartPos = CCamera::GetInstance()->GetRenderPos(GetPos());
	Vec2 EndPos;

	for (auto& btn : m_vecChildren)
	{
		EndPos = CCamera::GetInstance()->GetRenderPos(btn->GetPos());
		graphics->DrawLine(&pen, (INT)StartPos.x, (INT)StartPos.y, (INT)EndPos.x, (INT)EndPos.y);
	}

	CUI::Render(hdc);
}

void CBtnUI_Stage::Update()
{
}

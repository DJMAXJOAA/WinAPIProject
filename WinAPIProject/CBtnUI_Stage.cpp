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
	CUI::Render(hdc);
}

void CBtnUI_Stage::Update()
{
}

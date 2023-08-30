#include "pch.h"
#include "CBtnUI_Stage.h"

#include "CCore.h"
#include "CKeyMgr.h"

#include "CAnimator.h"
#include "CAnimation.h"

CBtnUI_Stage::CBtnUI_Stage()
	: CBtnUI(true)
	, m_FieldType(FIELD_TYPE::NONE)
	, m_bCleared(true)
	, m_bSelect(false)
{

}

CBtnUI_Stage::CBtnUI_Stage(int _value)
	: CBtnUI(true)
	, m_FieldType((FIELD_TYPE)_value)
	, m_bCleared(false)
	, m_bSelect(false)
	, m_pBtnSelect(0)
	, m_pSceneRobby(nullptr)
{
	SetScale(Vec2(30, 30));

	if (_value < 4)
	{
		SetAnimator(207, true);
	}
	else
	{
		SetAnimator(208, true);
	}
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
	if (1)
	{
		if (m_pBtnSelect && m_pSceneRobby)
		{
			// 원형 객체와 원형 함수에 접근 -> ((*객체).*함수())(인자있으면 인자);
			((*m_pSceneRobby).*m_pBtnSelect)(GetGridPos());
		}
	}
}

void CBtnUI_Stage::MouseOnCheck()
{
	Vec2 vScale = GetScale() / 2;
	Vec2 vMousePos = MOUSE_POS;

	Vec2 vFinalPos = GetFinalPos();

	if (IsCamAffected())
	{
		vMousePos = CCamera::GetInstance()->GetRealPos(vMousePos);
	}

	if (vFinalPos.x - vScale.x <= vMousePos.x && vMousePos.x <= vFinalPos.x + vScale.x
		&& vFinalPos.y - vScale.y <= vMousePos.y && vMousePos.y <= vFinalPos.y + vScale.y)
	{
		SetMouseOn(true);
	}
	else
	{
		SetMouseOn(false);
	}
}

void CBtnUI_Stage::Render(HDC hdc)
{
	CUI::Render(hdc);

	ComponetRender(hdc);
}

void CBtnUI_Stage::Update()
{
	CAnimation* animation = GetAnimator()->GetAnimation();
	switch (m_FieldType)
	{
	case FIELD_TYPE::UP:
		animation->SetFrame(0);
		break;
	case FIELD_TYPE::MID:
		animation->SetFrame(1);
		break;
	case FIELD_TYPE::DOWN:
		animation->SetFrame(2);
		break;

	case FIELD_TYPE::COMMON:
		animation->SetFrame(4);
		break;
	case FIELD_TYPE::ELITE:
		animation->SetFrame(5);
		break;
	case FIELD_TYPE::SHOP:
		animation->SetFrame(6);
		break;
	case FIELD_TYPE::SHELTER:
		animation->SetFrame(7);
		break;
	case FIELD_TYPE::BOSS:
		animation->SetFrame(8);
		break;
	case FIELD_TYPE::MESSAGE:
		animation->SetFrame(9);
		break;
	}

	//if (m_bSelect)
	//{
	//	if ((int)m_FieldType < 4) animation->SetFrame(animation->GetCurrentFrame() - 3);
	//	else animation->SetFrame(animation->GetCurrentFrame() - 7);
	//}

	CUI::Update();
	GetAnimator()->Update();
}

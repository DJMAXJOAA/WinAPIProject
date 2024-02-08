#include "pch.h"
#include "CBtnUI_Stage.h"

#include "CKeyMgr.h"
#include "CEventMgr.h"
#include "CResMgr.h"

#include "CTexture.h"

#include "CAnimator.h"
#include "CAnimation.h"

CBtnUI_Stage::CBtnUI_Stage(int _value)
	: CBtnUI(true)
	, m_pCircle(nullptr)
	, m_FieldType((FIELD_TYPE)_value)
	, m_bCleared(false)
	, m_bSelect(false)
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

	// 클리어용
	m_pCircle = CResMgr::GetInstance()->LoadTexture(L"Clear_Circle", L"texture\\UI\\circle.bmp");
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
	CBtnUI::MouseLbtnClicked();

	if (m_bSelect && !m_bCleared)
	{
		EnterStageEvent(GetGridPos());
	}
}

void CBtnUI_Stage::EnterStageEvent(Vec2 _vPos)
{
	tEvent evn = {  };
	evn.eEvent = EVENT_TYPE::FIELD_ENTER_STAGE;
	evn.lParam = (DWORD_PTR)_vPos.x;
	evn.wParam = (DWORD_PTR)_vPos.y;

	CEventMgr::GetInstance()->AddEvent(evn);
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
	/*CUI::Render(hdc);*/

	ComponetRender(hdc);

	if(m_bCleared)
	{
		Vec2 vRenderPos = CCamera::GetInstance()->GetRenderPos(GetPos());
		TransparentBlt(hdc, (int)(vRenderPos.x - m_pCircle->Width()/2.f), (int)(vRenderPos.y - m_pCircle->Height() / 2.f), m_pCircle->Width(), m_pCircle->Height(), m_pCircle->GetDC(), 0, 0, m_pCircle->Width(), m_pCircle->Height(), RGB(255, 0, 255));
	}
}

void CBtnUI_Stage::Update()
{
	CAnimation* animation = GetAnimator()->GetAnimation();
	switch (m_FieldType)
	{
	case FIELD_TYPE::UP:
		animation->SetFrame(3);
		break;
	case FIELD_TYPE::MID:
		animation->SetFrame(4);
		break;
	case FIELD_TYPE::DOWN:
		animation->SetFrame(5);
		break;

	case FIELD_TYPE::COMMON:
		animation->SetFrame(8);
		break;
	case FIELD_TYPE::ELITE:
		animation->SetFrame(9);
		break;
	case FIELD_TYPE::SHOP:
		animation->SetFrame(10);
		break;
	case FIELD_TYPE::SHELTER:
		animation->SetFrame(11);
		break;
	case FIELD_TYPE::BOSS:
		animation->SetFrame(12);
		break;
	case FIELD_TYPE::MESSAGE:
		animation->SetFrame(13);
		break;
	}

	if (m_bSelect || m_bCleared)
	{
		if ((int)m_FieldType < 4) animation->SetFrame(animation->GetCurrentFrame() - 3);
		else animation->SetFrame(animation->GetCurrentFrame() - 7);
	}

	CUI::Update();
	GetAnimator()->Update();
}

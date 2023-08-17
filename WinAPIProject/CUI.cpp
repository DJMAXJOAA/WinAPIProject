#include "pch.h"
#include "CUI.h"

#include "CKeyMgr.h"

#include "SelectGDI.h"

CUI::CUI(bool _bCamAff)
	: m_pParentUI(nullptr)
	, m_bCamAffected(_bCamAff)
	, m_bMouseOn(false)
	, m_bLbtnDown(false)
{
}

CUI::CUI(const CUI& _origin)
	: CObject(_origin)
	, m_pParentUI(nullptr)
	, m_bCamAffected(_origin.m_bCamAffected)
	, m_bMouseOn(false)
	, m_bLbtnDown(false)
{
	// 자식UI를 깊은 복사로 생성한다
	for (size_t i = 0; i < _origin.m_vecChildUI.size(); i++)
	{
		AddChild(_origin.m_vecChildUI[i]->Clone());
	}
}

CUI::~CUI()
{
	SafeDeleteVec(m_vecChildUI);
}

void CUI::MouseOn()
{
}

void CUI::MouseLbtnDown()
{
}

void CUI::MouseLbtnUp()
{
}

void CUI::MouseLbtnClicked()
{
}

void CUI::MouseOnCheck()
{
	Vec2 vMousePos = MOUSE_POS;
	Vec2 vScale = GetScale();

	if (m_bCamAffected)
	{
		vMousePos = CCamera::GetInstance()->GetRealPos(vMousePos);
	}

	if (m_vFinalPos.x <= vMousePos.x && vMousePos.x <= m_vFinalPos.x + vScale.x
		&& m_vFinalPos.y <= vMousePos.y && vMousePos.y <= m_vFinalPos.y + vScale.y)
	{
		m_bMouseOn = true;
	}
	else
	{
		m_bMouseOn = false;
	}
}

void CUI::Update()
{
	Update_Child();
}

void CUI::FinalUpdate()
{
	CObject::FinalUpdate();

	m_vFinalPos = GetPos();

	if (GetParent()) // 부모 클래스가 있으면, 부모 위치만큼 더해줌
	{
		Vec2 vParentPos = GetParent()->GetFinalPos();
		m_vFinalPos += vParentPos;
	}
	// UI 마우스 체크
	MouseOnCheck();

	// 자식 클래스의 finalupdate도 돌려주기
	FinalUpdate_Child();
}

void CUI::Render(HDC hdc)
{
	Vec2 vPos = GetFinalPos();
	Vec2 vScale = GetScale();

	if (m_bCamAffected)
	{
		vPos = CCamera::GetInstance()->GetRenderPos(vPos);
	}

	if (m_bLbtnDown)
	{
		SelectGDI select(hdc, PEN_TYPE::GREEN);

		Rectangle(hdc
			, (int)vPos.x
			, (int)vPos.y
			, (int)(vPos.x + vScale.x)
			, (int)(vPos.y + vScale.y));
	}
	else
	{
		Rectangle(hdc
			, (int)vPos.x
			, (int)vPos.y
			, (int)(vPos.x + vScale.x)
			, (int)(vPos.y + vScale.y));
	}

	Render_Child(hdc);
}

void CUI::Update_Child()
{
	for (size_t i = 0; i < m_vecChildUI.size(); i++)
	{
		m_vecChildUI[i]->Update();
	}
}

void CUI::FinalUpdate_Child()
{
	for (size_t i = 0; i < m_vecChildUI.size(); i++)
	{
		m_vecChildUI[i]->FinalUpdate();
	}
}

void CUI::Render_Child(HDC hdc)
{
	for (size_t i = 0; i < m_vecChildUI.size(); i++)
	{
		m_vecChildUI[i]->Render(hdc);
	}
}

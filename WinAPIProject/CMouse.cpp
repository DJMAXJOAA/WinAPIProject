#include "pch.h"
#include "CMouse.h"

#include "CKeyMgr.h"
#include "CEventMgr.h"

#include "CCollider.h"

CMouse::CMouse()
	: m_pOtherCollider(nullptr)
{
	CreateCollider();
	GetCollider()->SetScale(Vec2(10.f, 10.f));
	GetCollider()->SetFinalPos(GetPos());
}

CMouse::~CMouse()
{
}

void CMouse::OnCollisionEnter(CCollider* _pOther)
{
	// �÷��̾� ��ó�� �־��� ������ �ƴ��� üũ�ϱ�
	// �ٷ� ���� üũ�� �� ������, �̺�Ʈ �ٽ� ������ ����
	if (KEY_HOLD(KEY::LBTN) && (m_pOtherCollider != _pOther))
	{
		PlayerTileSelectTrigger(_pOther->GetObj());
	}
	m_pOtherCollider = _pOther;
}

void CMouse::PlayerTileSelectTrigger(CObject* _pObj)
{
	tEvent evn = {  };
	evn.eEvent = EVENT_TYPE::TILESELECT_TRIGGER;
	evn.lParam = (DWORD_PTR)_pObj;

	CEventMgr::GetInstance()->AddEvent(evn);
}

void CMouse::Render(HDC hdc)
{
	ComponetRender(hdc);
}

void CMouse::Update()
{
	Vec2 vPos = CCamera::GetInstance()->GetRealPos(MOUSE_POS);
	SetPos(vPos);
}

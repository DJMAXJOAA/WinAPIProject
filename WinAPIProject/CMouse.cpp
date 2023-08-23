#include "pch.h"
#include "CMouse.h"

#include "CKeyMgr.h"

#include "CCollider.h"

CMouse::CMouse()
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
	if (KEY_HOLD(KEY::LBTN))
	{
		PlayerTileSelect(_pOther->GetObj());
	}
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

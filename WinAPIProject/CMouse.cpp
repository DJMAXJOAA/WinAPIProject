#include "pch.h"
#include "CMouse.h"

#include "CKeyMgr.h"

#include "CCollider.h"

CMouse::CMouse()
{
	CreateCollider();
	GetCollider()->SetScale(Vec2(15.f, 15.f));
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
	Vec2 vPos = GetPos();

	vPos = MOUSE_POS;

	SetPos(vPos);
}

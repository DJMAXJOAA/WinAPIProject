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
	// 플레이어 근처에 있었던 블럭인지 아닌지 체크하기
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

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

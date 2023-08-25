#include "pch.h"
#include "CToolTest2.h"

#include "CCollider.h"
#include "CAnimator.h"
#include "CAnimation.h"

CToolTest2::CToolTest2()
	: m_iID(200)
{
	SetAnimator(200);

	SetAnimator(m_iID);
	GetAnimator()->Play(113, true);

	CreateCollider();
	GetCollider()->SetOffsetPos(Vec2(0.f, 0.f));
	GetCollider()->SetScale(GetAnimator()->GetAnimation()->GetFrame(0).vSlice);
}

CToolTest2::CToolTest2(int iAnimator, int iAnimation)
	: m_iID(iAnimator)
{
	SetAnimator(iAnimator);

	SetAnimator(m_iID);
	GetAnimator()->Play(iAnimation, true);

	CreateCollider();
	GetCollider()->SetOffsetPos(Vec2(0.f, 0.f));
	GetCollider()->SetScale(GetAnimator()->GetAnimation()->GetFrame(0).vSlice);
}

CToolTest2::~CToolTest2()
{
}

void CToolTest2::ChangeAnimation(int _key)
{
	DEBUG1(_key);

	GetAnimator()->Play(_key, true);

	GetCollider()->SetScale(GetAnimator()->GetAnimation()->GetFrame(0).vSlice);
}

void CToolTest2::ChangeAnimator(int _key)
{
	DEBUG1(_key);
	SetAnimator(_key);
	GetCollider()->SetScale(GetAnimator()->GetAnimation()->GetFrame(0).vSlice);
}

void CToolTest2::Render(HDC hdc)
{
	ComponetRender(hdc);
}

void CToolTest2::Update()
{
	GetAnimator()->Update();
}

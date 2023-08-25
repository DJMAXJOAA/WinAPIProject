#include "pch.h"
#include "CToolTest.h"

#include "CDataMgr.h"

#include "CCollider.h"
#include "CAnimator.h"
#include "CAnimation.h"

#include "SelectGDI.h"
#include "CTexture.h"

#include "AnimationData.h"

CToolTest1::CToolTest1()
	: m_iID(200)
{
	SetAnimator(m_iID);
	GetAnimator()->Play(113, true);

	CreateCollider();
	GetCollider()->SetOffsetPos(Vec2(0.f, 0.f));
	GetCollider()->SetScale(GetAnimator()->GetAnimation()->GetFrame(0).vSlice);
}

CToolTest1::CToolTest1(int iAnimator, int iAnimation)
	: m_iID(iAnimator)
{
	SetAnimator(iAnimator);

	SetAnimator(m_iID);
	GetAnimator()->Play(iAnimation, true);

	CreateCollider();
	GetCollider()->SetOffsetPos(Vec2(0.f, 0.f));
	GetCollider()->SetScale(GetAnimator()->GetAnimation()->GetFrame(0).vSlice);
}

CToolTest1::~CToolTest1()
{

}

void CToolTest1::ChangeAnimation(int _key)
{
	DEBUG1(_key);

	GetAnimator()->Play(_key, true);

	GetCollider()->SetScale(GetAnimator()->GetAnimation()->GetFrame(0).vSlice);
}

void CToolTest1::ChangeAnimator(int _key)
{
	DEBUG1(_key);
	SetAnimator(_key);
	GetCollider()->SetScale(GetAnimator()->GetAnimation()->GetFrame(0).vSlice);
}

void CToolTest1::Render(HDC hdc)
{
	CTexture* pTex = GetAnimator()->GetAnimation()->GetTexture();
	tAnimFrame frame = GetAnimator()->GetAnimation()->GetFrame(0);
	Vec2 vOffset = frame.vOffset;

	Vec2 vPos = CCamera::GetInstance()->GetRenderPos(GetPos());

	Vec2 vScale = GetCollider()->GetScale();
	Vec2 vRenderPos = CCamera::GetInstance()->GetRenderPos(GetCollider()->GetFinalPos());

	Rectangle(hdc
		, (int)(vRenderPos.x - vScale.x / 2.f)
		, (int)(vRenderPos.y - vScale.y / 2.f)
		, (int)(vRenderPos.x + vScale.x / 2.f)
		, (int)(vRenderPos.y + vScale.y / 2.f));

	TransparentBlt(hdc
		, (int)(vPos.x - frame.vSlice.x / 2.f)
		, (int)(vPos.y - frame.vSlice.y / 2.f)
		, (int)frame.vSlice.x
		, (int)frame.vSlice.y
		, pTex->GetDC()
		, (int)frame.vLeftTop.x
		, (int)frame.vLeftTop.y
		, (int)frame.vSlice.x
		, (int)frame.vSlice.y
		, RGB(255, 0, 255)
	);

	vPos = CCamera::GetInstance()->GetRenderPos(GetPos() + vOffset);
	vScale = Vec2(10.f, 10.f);
	SelectGDI gdi_pen(hdc, PEN_TYPE::RED);
	SelectGDI gdi_brush(hdc, BRUSH_TYPE::HOLLOW);
	Rectangle(hdc	,(int)(vPos.x - vScale.x / 2.f)
					,(int)(vPos.y - vScale.y / 2.f)
					,(int)(vPos.x + vScale.x / 2.f)
					,(int)(vPos.y + vScale.y / 2.f));
}

void CToolTest1::Update()
{
	GetAnimator()->Update();
}

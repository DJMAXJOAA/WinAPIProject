#include "pch.h"
#include "CEffect.h"

#include "CAnimatorGdiPlus.h"

CEffect::CEffect(int _animator, Vec2 _vPos)
	: m_bRendered(true)
{
	SetAnimatorGdiPlus(_animator, false);

	_vPos.y -= 50.f;
	SetPos(_vPos);
}

CEffect::~CEffect()
{
}

void CEffect::Render(HDC hdc)
{
	if (!m_bRendered)
		return;

	ComponetRender(hdc);
}

void CEffect::Update()
{
	if (!m_bRendered)
		return;

	GetAnimatorGdiPlus()->Update();
}
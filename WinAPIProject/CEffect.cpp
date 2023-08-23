#include "pch.h"
#include "CEffect.h"

#include "CAnimatorGdiPlus.h"

CEffect::CEffect()
{
	SetAnimatorGdiPlus(203, false);
}

CEffect::~CEffect()
{
}

void CEffect::Render(HDC hdc)
{
	ComponetRender(hdc);
}

void CEffect::Update()
{
	GetAnimatorGdiPlus()->Update();
}
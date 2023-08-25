#include "pch.h"
#include "CMonster.h"

#include "CTimeMgr.h"

#include "CAnimator.h"
#include "CAnimation.h"

#include "IStrategy.h"

CMonster::CMonster()
{
	SetAnimator(204);
}

CMonster::~CMonster()
{
}

void CMonster::MovePattern(IMonsterStrategy* _stratey)
{
	_stratey->MoveStrategy(this);
}

void CMonster::Update()
{
	GetAnimator()->Update();
}

void CMonster::Render(HDC hdc)
{
	ComponetRender(hdc);
}

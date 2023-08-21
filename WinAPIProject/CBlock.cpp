#include "pch.h"
#include "CBlock.h"

#include "CDataMgr.h"d

#include "CCollider.h"
#include "CAnimator.h"
#include "CAnimation.h"

static std::random_device rd;
static std::mt19937 rng(rd());

CBlock::CBlock(MAP_TYPE _type)
	: m_BlockState(0)
{
	
	SetAnimator(202);
	
}

CBlock::~CBlock()
{
}

int CBlock::RandomBlock()
{
	GetAnimator()->GetAnimation()
	std::uniform_int_distribution<int> dist(0, 2);
	return dist(rng);
}

void CBlock::Update()
{
	GetAnimator()->Update();
}

void CBlock::Render(HDC hdc)
{
	ComponetRender(hdc);
}

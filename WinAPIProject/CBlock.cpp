#include "pch.h"
#include "CBlock.h"

#include "CCollider.h"
#include "CAnimator.h"
#include "CAnimation.h"

static std::random_device rd;
static std::mt19937 rng(rd());

CBlock::CBlock()
	: m_TileState(RandomTile())
{
	SetAnimator(202);
}

CBlock::~CBlock()
{
}

TILE_STATE CBlock::RandomTile()
{
	std::uniform_int_distribution<int> dist(0, 2);
	return (TILE_STATE)dist(rng);
}

void CBlock::Update()
{
	CAnimation* animation = GetAnimator()->GetAnimation();

	switch (m_TileState)
	{
	case TILE_STATE::BLACK:
		animation->SetFrame(0);
		break;
	case TILE_STATE::RED:
		animation->SetFrame(1);
		break;
	case TILE_STATE::GREEN:
		animation->SetFrame(2);
		break;
	}

	GetAnimator()->Update();
}

void CBlock::Render(HDC hdc)
{
	ComponetRender(hdc);
}

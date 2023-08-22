#include "pch.h"
#include "CTile.h"

#include "CCollider.h"
#include "CAnimator.h"
#include "CAnimation.h"

static std::random_device rd;
static std::mt19937 rng(rd());

CTile::CTile()
	: m_TileState(RandomTile())
{
	CreateCollider();
	GetCollider()->SetScale(Vec2(20, 20));
	SetAnimator(201);
}

CTile::~CTile()
{
}

TILE_STATE CTile::RandomTile()
{
	std::uniform_int_distribution<int> dist(5, 9);
	return (TILE_STATE)dist(rng);
}

void CTile::Update()
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
	case TILE_STATE::BLUE:
		animation->SetFrame(3);
		break;
	case TILE_STATE::PURPLE:
		animation->SetFrame(4);
		break;
	}

	GetAnimator()->Update();
}

void CTile::Render(HDC hdc)
{
	ComponetRender(hdc);
}

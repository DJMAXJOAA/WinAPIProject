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

	m_TileTexture = GetAnimator()->GetAnimation();
	m_TileTexture->SetFrame((int)m_TileState);
}

CTile::~CTile()
{
}

TILE_STATE CTile::RandomTile()
{
	std::uniform_int_distribution<int> dist(1, 4);
	return (TILE_STATE)dist(rng);
}

void CTile::Update()
{
	m_TileTexture->SetFrame((int)m_TileState);

	GetAnimator()->Update();
}

void CTile::Render(HDC hdc)
{
	ComponetRender(hdc);
}

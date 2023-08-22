#include "pch.h"
#include "CBlock.h"

#include "CDataMgr.h"

#include "CCollider.h"
#include "CAnimator.h"
#include "CAnimation.h"

static std::random_device rd;
static std::mt19937 rng(rd());

CBlock::CBlock(MAP_TYPE _type)
	: m_BlockState(0)
{
	SetAnimator(202);
	switch (_type)
	{
	case MAP_TYPE::BRICK:
		GetAnimator()->Play(129, true);
		break;
	case MAP_TYPE::DIRT:
		GetAnimator()->Play(130, true);
		break;
	case MAP_TYPE::GRASS:
		GetAnimator()->Play(131, true);
		break;
	case MAP_TYPE::STONE:
		GetAnimator()->Play(132, true);
		break;
	case MAP_TYPE::SNOW:
		GetAnimator()->Play(133, true);map ¼³Á¤
		break;
	default:
		break;
	}
	m_BlockState = RandomBlock();
}

CBlock::~CBlock()
{
}

int CBlock::RandomBlock()
{
	int range = GetAnimator()->GetAnimation()->GetMaxFrame();
	std::uniform_int_distribution<int> dist(0, range);
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

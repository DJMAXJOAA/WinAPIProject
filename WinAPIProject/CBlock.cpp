#include "pch.h"
#include "CBlock.h"

#include "CAnimator.h"
#include "CAnimation.h"

static std::random_device rd;
static std::mt19937 rng(rd());

CBlock::CBlock(BLOCK_TYPE _type)
	: m_BlockState(0)
{
	SetAnimator(202);
	switch (_type)
	{
	case BLOCK_TYPE::BRICK:
		GetAnimator()->Play(129, true);
		break;
	case BLOCK_TYPE::DIRT:
		GetAnimator()->Play(130, true);
		break;
	case BLOCK_TYPE::GRASS:
		GetAnimator()->Play(131, true);
		break;
	case BLOCK_TYPE::STONE:
		GetAnimator()->Play(132, true);
		break;
	case BLOCK_TYPE::SNOW:
		GetAnimator()->Play(133, true);
		break;
	default:
		break;
	}
	m_BlockState = RandomBlock();

	CAnimation* animation = GetAnimator()->GetAnimation();
	animation->SetFrame(m_BlockState);
}

CBlock::~CBlock()
{
}

int CBlock::RandomBlock()
{
	int range = GetAnimator()->GetAnimation()->GetMaxFrame() - 1;
	std::uniform_int_distribution<int> dist(0, range);
	return dist(rng);
}

void CBlock::Update()
{
}

void CBlock::Render(HDC hdc)
{
	ComponetRender(hdc);
}

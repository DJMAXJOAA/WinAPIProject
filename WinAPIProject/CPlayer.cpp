#include "pch.h"
#include "CPlayer.h"

#include "CTimeMgr.h"
#include "CKeyMgr.h"
#include "CSceneMgr.h"
#include "CPathMgr.h"
#include "CResMgr.h"

#include "CTexture.h"
#include "CScene.h"
#include "CMissile.h"

#include "CCollider.h"
#include "CAnimator.h"
#include "CAnimation.h"

CPlayer::CPlayer()
	: m_fSpeed(200.f)
	, m_playerState(PLAYER_STATE::IDEL)
{
	CreateCollider();
	GetCollider()->SetScale(Vec2(15.f, 15.f));

	// �ؽ��� �ε� (�ִϸ��̼� ����)
	SetAnimator(200);
	GetAnimator()->Play((int)PLAYER_STATE::IDEL, true);
}

CPlayer::~CPlayer()
{

}

void CPlayer::Move(Vec2 _vGridFront, Vec2 _vGridRear, Vec2 _vDestination)
{
	Vec2 vPos = GetPos();

	AnimationFrontBack(PLAYER_STATE::MOVE, _vGridFront, _vGridRear, _vDestination);

	// �Ҽ��� ������ ��ȯ�ǰ�
	if(vPos.DistanceTo(_vDestination) <= m_fSpeed * fDT)
		return SetPos(_vDestination);

	Vec2 vDirection = (vPos - _vDestination).Normalize() * -1;

	vPos.x += m_fSpeed * vDirection.x * fDT;
	vPos.y += m_fSpeed * vDirection.y * fDT;

	SetPos(vPos);
}

void CPlayer::Attack(Vec2 _vGridFront, Vec2 _vGridRear, Vec2 _vDestination)
{
	AnimationFrontBack(PLAYER_STATE::UPPERCUT, _vGridFront, _vGridRear, _vDestination);
}

void CPlayer::Render(HDC hdc)
{
	ComponetRender(hdc);
}

void CPlayer::Update()
{
	GetAnimator()->Update();
}

void CPlayer::AnimationEvent()
{
	printf("�ִϸ��̼� �̺�Ʈ ȣ��\n");
}

void CPlayer::AnimationFrontBack(PLAYER_STATE _anim, Vec2 _vGridFront, Vec2 _vGridRear, Vec2 _vDestination)
{
	Vec2 vPos = GetPos();
	const int animationInterval = 14;	// �տ����� ��ǰ� �ڿ����� ����� �� ���̰� 14

	// �ڵ��� ��� ����
	if (_vGridFront.x >= _vGridRear.x && _vGridFront.y >= _vGridRear.y)
	{
		GetAnimator()->Play((int)_anim + animationInterval, true);

		// �̹��� �¿� ����
		if (vPos.x < _vDestination.x) GetAnimator()->GetAnimation()->SetFlip(false);
		else GetAnimator()->GetAnimation()->SetFlip(true);
	}
	// �� �ȵ�
	else 
	{
		GetAnimator()->Play((int)_anim, true);

		// �̹��� �¿� ����
		if (vPos.x < _vDestination.x) GetAnimator()->GetAnimation()->SetFlip(true);
		else GetAnimator()->GetAnimation()->SetFlip(false);
	}
}

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

	// 텍스쳐 로딩 (애니메이션 설정)
	SetAnimator(200);
	GetAnimator()->Play((int)PLAYER_STATE::IDEL, true);
}

CPlayer::~CPlayer()
{

}

void CPlayer::Move(Vec2 _vGridFront, Vec2 _vGridRear, Vec2 _vDestination)
{
	Vec2 vPos = GetPos();

	AnimationDirection(PLAYER_STATE::MOVE, false, _vGridFront, _vGridRear, _vDestination);

	// 소수점 버리고 반환되게
	if(vPos.DistanceTo(_vDestination) <= m_fSpeed * fDT)
		return SetPos(_vDestination);

	Vec2 vDirection = (vPos - _vDestination).Normalize() * -1;

	vPos.x += m_fSpeed * vDirection.x * fDT;
	vPos.y += m_fSpeed * vDirection.y * fDT;

	SetPos(vPos);
}

void CPlayer::Attack(Vec2 _vGridFront, Vec2 _vGridRear, Vec2 _vDestination)
{
	AnimationDirection(PLAYER_STATE::UPPERCUT, false, _vGridFront, _vGridRear, _vDestination);
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
	CCamera::GetInstance()->SetVibrateCamera(10.f, 1, 0.02f);
	printf("애니메이션 이벤트 호출\n");
}

void CPlayer::AnimationEnd()
{
	GetAnimator()->Play(100, true);
	printf("애니메이션 종료 호출\n");
}

void CPlayer::AnimationDirection(PLAYER_STATE _anim, bool _bRepeat, Vec2 _vGridFront, Vec2 _vGridRear, Vec2 _vDestination)
{
	Vec2 vPos = GetPos();
	const int animationInterval = 14;	// 앞에보는 모션과 뒤에보는 모션의 값 차이가 14

	// 뒤도는 모션 설정
	if (_vGridFront.x >= _vGridRear.x && _vGridFront.y >= _vGridRear.y)
	{
		GetAnimator()->Play((int)_anim + animationInterval, _bRepeat);

		// 이미지 좌우 반전
		if (vPos.x < _vDestination.x) GetAnimator()->GetAnimation()->SetFlip(false);
		else GetAnimator()->GetAnimation()->SetFlip(true);
	}
	// 뒤 안돔
	else 
	{
		GetAnimator()->Play((int)_anim, _bRepeat);

		// 이미지 좌우 반전
		if (vPos.x < _vDestination.x) GetAnimator()->GetAnimation()->SetFlip(true);
		else GetAnimator()->GetAnimation()->SetFlip(false);
	}
}

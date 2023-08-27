#include "pch.h"
#include "CPlayer.h"

#include "CTimeMgr.h"
#include "CKeyMgr.h"
#include "CSceneMgr.h"
#include "CPathMgr.h"
#include "CResMgr.h"
#include "CEventMgr.h"

#include "CTexture.h"
#include "CScene.h"

#include "CCollider.h"
#include "CAnimator.h"
#include "CAnimation.h"

CPlayer::CPlayer()
	: m_fSpeed(200.f)
	, m_playerState(PLAYER_STATE::IDEL)
	, m_pTargetMonster(nullptr)
	, m_fAtt(50.f)
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

void CPlayer::Move(GRID_DIRECTION _aniDirection, Vec2 _vDestination)
{
	Vec2 vPos = GetPos();

	AnimationDirection(PLAYER_STATE::MOVE, true, _aniDirection);

	// 소수점 버리고 반환되게
	if(vPos.DistanceTo(_vDestination) <= m_fSpeed * fDT)
		return SetPos(_vDestination);

	Vec2 vDirection = (vPos - _vDestination).Normalize() * -1;

	vPos.x += m_fSpeed * vDirection.x * fDT;
	vPos.y += m_fSpeed * vDirection.y * fDT;

	SetPos(vPos);
}

void CPlayer::Attack(GRID_DIRECTION _aniDirection, CObject* pMon)
{
	// 애니메이션 이벤트 프레임에 공격 들어가게
	// 여기선 애니메이션 설정
	SetTarget(pMon);
	AnimationDirection(PLAYER_STATE::UPPERCUT, false, _aniDirection);
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
	PlayerAttackMonster(m_fAtt, m_pTargetMonster);
	CCamera::GetInstance()->SetVibrateCamera(10.f, 1, 0.02f);
	printf("애니메이션 이벤트 호출\n");
}

void CPlayer::AnimationEnd()
{
	GetAnimator()->PlayType(L"Idle", true);

	m_pTargetMonster = nullptr;
	printf("애니메이션 종료 호출\n");
}

void CPlayer::PlayerAttackMonster(float _damage, CObject* _pMon)
{
	tEvent evn = {  };
	evn.eEvent = EVENT_TYPE::PLAYER_ATTACK;
	evn.lParam = (DWORD_PTR)_damage;
	evn.wParam = (DWORD_PTR)_pMon;

	CEventMgr::GetInstance()->AddEvent(evn);
}

void CPlayer::AnimationDirection(PLAYER_STATE _anim, bool _bRepeat, GRID_DIRECTION _aniDirection)
{
	const int animationInterval = 14;	// 앞에보는 모션과 뒤에보는 모션의 값 차이가 14

	switch (_aniDirection)
	{
	case GRID_DIRECTION::UP:
	{
		GetAnimator()->Play((int)_anim + animationInterval, _bRepeat);
		GetAnimator()->GetAnimation()->SetFlip(false);
		break;
	}
	case GRID_DIRECTION::LEFT:
	{
		GetAnimator()->Play((int)_anim + animationInterval, _bRepeat);
		GetAnimator()->GetAnimation()->SetFlip(true);
		break;
	}
	case GRID_DIRECTION::RIGHT:
	{
		GetAnimator()->Play((int)_anim, _bRepeat);
		GetAnimator()->GetAnimation()->SetFlip(true);
		break;
	}
	case GRID_DIRECTION::DOWN:
	{
		GetAnimator()->Play((int)_anim, _bRepeat);
		GetAnimator()->GetAnimation()->SetFlip(false);
		break;
	}
	}
}

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
	, m_playerState(PLAYER_STATE::IDLE)
	, m_playerDirection(GRID_DIRECTION::DOWN)
	, m_pTargetMonster(nullptr)
	, m_fAtt(50.f)
	, m_bAttack(false)
	, m_fCombo(0)
	, m_fHP(1000)
	, m_fMaxHP(m_fHP)
{
	CreateCollider();
	GetCollider()->SetScale(Vec2(15.f, 15.f));

	// 텍스쳐 로딩 (애니메이션 설정)
	SetAnimator(200);
	GetAnimator()->Play((int)PLAYER_STATE::IDLE, true);
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
	m_playerState = PLAYER_STATE::UPPERCUT;
	AnimationDirection(PLAYER_STATE::UPPERCUT, false, _aniDirection);

	printf("CPlayer::PlayerAttackDone :: 공격 시작\n");
}

void CPlayer::Skill(int _iCombo)
{
	// 콤보 * 0.1이 콤보 데미지
	m_fCombo = float(_iCombo * 0.1);

	m_playerState = PLAYER_STATE::AXE;
	AnimationDirection(PLAYER_STATE::AXE, false);

	printf("CPlayer::PlayerAttackDone :: 스킬 시작\n");
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
	switch (m_playerState)
	{
	case PLAYER_STATE::UPPERCUT:
	{
		CCamera::GetInstance()->SetVibrateCamera(10.f, 1, 0.02f);
		if (m_pTargetMonster)
		{
			AttackStartEvent(m_fAtt, m_pTargetMonster);
		}

		printf("CPlayer::AnimationEvent :: 캐릭터 공격 애니메이션 이벤트 시작\n");

		break;
	}
	case PLAYER_STATE::AXE:
	{
		// 스킬 데미지 : 플레이어의 공격력 + 콤보수 * 0.1
		float fSkillAtt = m_fAtt + float(m_fAtt * m_fCombo);

		CCamera::GetInstance()->SetVibrateCamera(30.f, 1, 0.02f);
		SkillCastEvent(fSkillAtt);
		printf("CPlayer::AnimationEvent :: 캐릭터 스킬 애니메이션 이벤트 시작\n");
		break;
	}
	}
	
}

void CPlayer::AnimationEnd()
{
	cout << (int)m_playerState << "\n";
	switch (m_playerState)
	{
	case PLAYER_STATE::UPPERCUT:
	{
		AttackDoneEvent();
		printf("CPlayer::AnimationEnd :: 캐릭터 공격 애니메이션 종료 시작\n");

		break;
	}
	case PLAYER_STATE::AXE:
	{
		// 콤보 초기화
		m_fCombo = 0;

		SkillDoneEvent();
		printf("CPlayer::AnimationEnd :: 캐릭터 스킬 애니메이션 종료 시작\n");
		break;
	}
	case PLAYER_STATE::DAMAGED:
	{
		m_playerState = PLAYER_STATE::IDLE;
		AnimationDirection(m_playerState, false);
		printf("CPlayer::AnimationEnd :: 캐릭터 피격 애니메이션 종료 시작\n");
		break;
	}
	}
}

void CPlayer::AttackStartEvent(float _damage, CObject* _pMon)
{
	tEvent evn = {  };
	evn.eEvent = EVENT_TYPE::PLAYER_ATTACK;
	evn.lParam = (DWORD_PTR)_damage;
	evn.wParam = (DWORD_PTR)_pMon;

	CEventMgr::GetInstance()->AddEvent(evn);
}

void CPlayer::AttackDoneEvent()
{
	tEvent evn = {  };
	evn.eEvent = EVENT_TYPE::PLAYER_ATTACK_DONE;

	CEventMgr::GetInstance()->AddEventEarly(evn);
}

void CPlayer::SkillCastEvent(float _value)
{
	tEvent evn = {  };
	evn.eEvent = EVENT_TYPE::PLAYER_SKILL_CAST;
	evn.lParam = (DWORD_PTR)_value;

	CEventMgr::GetInstance()->AddEvent(evn);
}

void CPlayer::SkillDoneEvent()
{
	tEvent evn = {  };
	evn.eEvent = EVENT_TYPE::PLAYER_SKILL_DONE;

	CEventMgr::GetInstance()->AddEventEarly(evn);
}

void CPlayer::GetDamaged(float _damaged)
{
	if (this == nullptr || m_fHP <= 0)
	{
		return;
	}

	m_fHP -= _damaged;
	if (m_fHP <= 0)
	{
		m_fHP = 0;
		Died();

		// 디버깅
		printf("캐릭터 사망\n");

		return;
	}
	else
	{
		m_playerState = PLAYER_STATE::DAMAGED;
		AnimationDirection(m_playerState, false);

		// 디버깅
		printf("캐릭터 데미지 받음 : %1.f, 남은 체력 : %1.f\n", _damaged, m_fHP);
	}
}

void CPlayer::Died()
{
	tEvent evn = {  };
	evn.eEvent = EVENT_TYPE::PLAYER_DIED;
	evn.lParam = (DWORD_PTR)this;

	CEventMgr::GetInstance()->AddEvent(evn);
	DeleteObj(this);
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
		m_playerDirection = GRID_DIRECTION::UP;
		break;
	}
	case GRID_DIRECTION::LEFT:
	{
		GetAnimator()->Play((int)_anim + animationInterval, _bRepeat);
		GetAnimator()->GetAnimation()->SetFlip(true);
		m_playerDirection = GRID_DIRECTION::LEFT;
		break;
	}
	case GRID_DIRECTION::RIGHT:
	{
		GetAnimator()->Play((int)_anim, _bRepeat);
		GetAnimator()->GetAnimation()->SetFlip(true);
		m_playerDirection = GRID_DIRECTION::RIGHT;
		break;
	}
	case GRID_DIRECTION::DOWN:
	{
		GetAnimator()->Play((int)_anim, _bRepeat);
		GetAnimator()->GetAnimation()->SetFlip(false);
		m_playerDirection = GRID_DIRECTION::DOWN;
		break;
	}
	}
}

void CPlayer::AnimationDirection(PLAYER_STATE _anim, bool _bRepeat)
{
	const int animationInterval = 14;	// 앞에보는 모션과 뒤에보는 모션의 값 차이가 14

	switch (m_playerDirection)
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

#include "pch.h"
#include "CMonster.h"

#include "CTimeMgr.h"
#include "CDataMgr.h"
#include "CEventMgr.h"

#include "CAnimator.h"
#include "CAnimation.h"

#include "MonsterData.h"

static std::random_device rd;
static std::mt19937 gen(rd());

CMonster::CMonster(int _key)
	: m_vecStrategy{}
	, m_lstRoute{}
	, m_fSpeed(30.f)
	, m_currentStrategy(MONSTER_STRATEGY::NONE)
{
	MonsterData* monData = (MonsterData*)CDataMgr::GetInstance()->FindData(_key);
	m_strName = monData->m_strName;
	
	m_fHP = monData->m_fHP;
	m_fMaxHP = monData->m_fHP;
	m_fAtt = monData->m_fAtt;
	m_iMove = monData->m_iMove;
	m_iRange = monData->m_iRange;
	m_vecStrategy = monData->m_vecMosterStrategy;
	SetAnimator(monData->m_iAnimatorKey);
}

CMonster::CMonster(MonsterData* _data)
	: m_strName(_data->m_strName)
	, m_vecStrategy(_data->m_vecMosterStrategy)
	, m_fHP(_data->m_fHP)
	, m_fMaxHP(_data->m_fHP)
	, m_fAtt(_data->m_fAtt)
	, m_iMove(_data->m_iMove)
	, m_iRange(_data->m_iRange)
	, m_fSpeed(200.f)
	, m_lstRoute{}
	, m_bActionFinish(false)
	, m_monsterState(MONSTER_STATE::IDLE)
	, m_monsterDirection(GRID_DIRECTION::DOWN)
	, m_currentStrategy(MONSTER_STRATEGY::NONE)
{
	SetAnimator(_data->m_iAnimatorKey);
}

CMonster::CMonster(const CMonster& _origin)
	: CObject(_origin)
	, m_strName(_origin.m_strName)
	, m_vecStrategy(_origin.m_vecStrategy)
	, m_fHP(_origin.m_fHP)
	, m_fMaxHP(_origin.m_fMaxHP)
	, m_fAtt(_origin.m_fAtt)
	, m_iMove(_origin.m_iMove)
	, m_iRange(_origin.m_iRange)
	, m_fSpeed(_origin.m_fSpeed)
	, m_lstRoute{}
	, m_bActionFinish(_origin.m_bActionFinish)
	, m_monsterState(_origin.m_monsterState)
	, m_monsterDirection(_origin.m_monsterDirection)
	, m_currentStrategy(_origin.m_currentStrategy)
{
}

CMonster::~CMonster()
{
}

MONSTER_STRATEGY CMonster::RandomPattern()
{
	// 배열 내 랜덤 패턴을 반환, 패턴 설정
	std::uniform_int_distribution<int> distr(0, (int)m_vecStrategy.size() - 1);
	MONSTER_STRATEGY pattern = m_vecStrategy[distr(gen)];
	m_currentStrategy = pattern;

	return pattern;
}

void CMonster::AnimationDirection(wstring _anim, bool _bRepeat, GRID_DIRECTION _aniDirection)
{
	switch (_aniDirection)
	{
	case GRID_DIRECTION::UP:
	{
		_anim = L"back_" + _anim;
		GetAnimator()->PlayType(_anim, _bRepeat);
		GetAnimator()->GetAnimation()->SetFlip(false);
		m_monsterDirection = GRID_DIRECTION::UP;
		break;
	}
	case GRID_DIRECTION::LEFT:
	{
		_anim = L"back_" + _anim;
		GetAnimator()->PlayType(_anim, _bRepeat);
		GetAnimator()->GetAnimation()->SetFlip(true);
		m_monsterDirection = GRID_DIRECTION::LEFT;
		break;
	}
	case GRID_DIRECTION::RIGHT:
	{
		_anim = L"front_" + _anim;
		GetAnimator()->PlayType(_anim, _bRepeat);
		GetAnimator()->GetAnimation()->SetFlip(true);
		m_monsterDirection = GRID_DIRECTION::RIGHT;
		break;
	}
	case GRID_DIRECTION::DOWN:
	{
		_anim = L"front_" + _anim;
		GetAnimator()->PlayType(_anim, _bRepeat);
		GetAnimator()->GetAnimation()->SetFlip(false);
		m_monsterDirection = GRID_DIRECTION::DOWN;
		break;
	}
	}
}

void CMonster::AnimationDirection(wstring _anim, bool _bRepeat)
{
	switch (m_monsterDirection)
	{
	case GRID_DIRECTION::UP:
	{
		_anim = L"back_" + _anim;
		GetAnimator()->PlayType(_anim, _bRepeat);
		GetAnimator()->GetAnimation()->SetFlip(false);
		m_monsterDirection = GRID_DIRECTION::UP;
		break;
	}
	case GRID_DIRECTION::LEFT:
	{
		_anim = L"back_" + _anim;
		GetAnimator()->PlayType(_anim, _bRepeat);
		GetAnimator()->GetAnimation()->SetFlip(true);
		m_monsterDirection = GRID_DIRECTION::LEFT;
		break;
	}
	case GRID_DIRECTION::RIGHT:
	{
		_anim = L"front_" + _anim;
		GetAnimator()->PlayType(_anim, _bRepeat);
		GetAnimator()->GetAnimation()->SetFlip(true);
		m_monsterDirection = GRID_DIRECTION::RIGHT;
		break;
	}
	case GRID_DIRECTION::DOWN:
	{
		_anim = L"front_" + _anim;
		GetAnimator()->PlayType(_anim, _bRepeat);
		GetAnimator()->GetAnimation()->SetFlip(false);
		m_monsterDirection = GRID_DIRECTION::DOWN;
		break;
	}
	}
}

void CMonster::Move(GRID_DIRECTION _aniDirection, Vec2 _vDestination)
{
	Vec2 vPos = GetPos();

	AnimationDirection(L"move", true, _aniDirection);

	// 소수점 버리고 반환되게
	if (vPos.DistanceTo(_vDestination) <= m_fSpeed * fDT)
		return SetPos(_vDestination);

	Vec2 vDirection = (vPos - _vDestination).Normalize() * -1;

	vPos.x += m_fSpeed * vDirection.x * fDT;
	vPos.y += m_fSpeed * vDirection.y * fDT;

	SetPos(vPos);
}

void CMonster::Attack(GRID_DIRECTION _aniDirection, CPlayer* _pPlayer)
{
	m_monsterState = MONSTER_STATE::ATTACK;
	AnimationDirection(L"attack", false, _aniDirection);

	printf("CMonster::AttackPlayer :: 공격 시작\n");
}

void CMonster::AnimationEvent()
{
	switch (m_monsterState)
	{
	case MONSTER_STATE::ATTACK:
	{
		CCamera::GetInstance()->SetVibrateCamera(10.f, 1, 0.02f);
		AttackStartEvent(m_fAtt);
		printf("CMonster::AnimationEvent :: 몬스터 공격 애니메이션 이벤트 시작\n");

		break;
	}
	case MONSTER_STATE::SKILL:
	{
		printf("CMonster::AnimationEvent :: 몬스터 스킬 애니메이션 이벤트 시작\n");

		break;
	}
	}
}

void CMonster::AnimationEnd()
{
	switch (m_monsterState)
	{
	case MONSTER_STATE::ATTACK:
	{
		AttackDoneEvent();
		printf("CMonster::AnimationEvent :: 몬스터 공격 애니메이션 종료 시작\n");

		break;
	}
	case MONSTER_STATE::SKILL:
	{
		printf("CMonster::AnimationEvent :: 몬스터 스킬 애니메이션 종료 시작\n");

		break;
	}
	case MONSTER_STATE::DAMAGED:
	{
		m_monsterState = MONSTER_STATE::IDLE;
		AnimationDirection(L"idle", true);
		printf("CPlayer::AnimationEnd :: 몬스터 피격 애니메이션 종료 시작\n");
		break;
	}
	}
}

void CMonster::AttackStartEvent(float _damage)
{
	tEvent evn = {  };
	evn.eEvent = EVENT_TYPE::MONSTER_ATTACK;
	evn.lParam = (DWORD_PTR)_damage;

	CEventMgr::GetInstance()->AddEvent(evn);
}

void CMonster::AttackDoneEvent()
{
	tEvent evn = {  };
	evn.eEvent = EVENT_TYPE::MONSTER_ATTACK_DONE;
	evn.lParam = (DWORD_PTR)this;

	CEventMgr::GetInstance()->AddEventEarly(evn);
}


void CMonster::GetDamaged(float _damaged)
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
		printf("몬스터 사망\n");

		return;
	}
	else
	{
		m_monsterState = MONSTER_STATE::DAMAGED;
		AnimationDirection(L"damaged", false);

		// 디버깅
		printf("몬스터 데미지 받음 : %1.f, 남은 체력 : %1.f\n", _damaged, m_fHP);
	}
}

void CMonster::Died()
{
	tEvent evn = {  };
	evn.eEvent = EVENT_TYPE::MONSTER_DIED;
	evn.lParam = (DWORD_PTR)this;

	CEventMgr::GetInstance()->AddEvent(evn);
	DeleteObj(this);
}

void CMonster::Update()
{
	cout << GetAnimator()->GetAnimation()->GetEventFrame();
	GetAnimator()->Update();
}

void CMonster::Render(HDC hdc)
{
	ComponetRender(hdc);
}

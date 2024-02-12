#include "pch.h"
#include "CMonster.h"

#include "CTimeMgr.h"
#include "CDataMgr.h"
#include "CEventMgr.h"
#include "CResMgr.h"

#include "CPanelUI_Number.h"

#include "CSound.h"
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
	, m_pHitSound(nullptr)
{
	MonsterData* monData = (MonsterData*)CDataMgr::GetInstance()->FindData(_key);
	m_strName = monData->m_strName;
	
	m_fHP = monData->m_fHP;
	m_fMaxHP = monData->m_fHP;
	m_fAtt = monData->m_fAtt;
	m_iMove = monData->m_iMove;
	m_iRange = monData->m_iRange;
	m_vecStrategy = monData->m_vecMosterStrategy;
	m_iMoney = monData->m_iMoney;
	SetAnimator(monData->m_iAnimatorKey);

	CreateHealthBar(true);

	// ���� ����
	m_pHitSound = CResMgr::GetInstance()->LoadSound(L"hitSound", L"sound\\attack.wav");
}

CMonster::CMonster(MonsterData* _data)
	: m_strName(_data->m_strName)
	, m_vecStrategy(_data->m_vecMosterStrategy)
	, m_fHP(_data->m_fHP)
	, m_fMaxHP(_data->m_fHP)
	, m_fAtt(_data->m_fAtt)
	, m_iMove(_data->m_iMove)
	, m_iRange(_data->m_iRange)
	, m_iMoney(_data->m_iMoney)
	, m_fSpeed(200.f)
	, m_lstRoute{}
	, m_bActionFinish(false)
	, m_monsterState(MONSTER_STATE::IDLE)
	, m_monsterDirection(GRID_DIRECTION::DOWN)
	, m_currentStrategy(MONSTER_STRATEGY::NONE)
	, m_pHitSound(nullptr)
{
	SetAnimator(_data->m_iAnimatorKey);

	CreateHealthBar(true);

	// ���� ����
	m_pHitSound = CResMgr::GetInstance()->LoadSound(L"hitSound", L"sound\\attack.wav");
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
	, m_iMoney(_origin.m_iMoney)
	, m_lstRoute{}
	, m_bActionFinish(_origin.m_bActionFinish)
	, m_monsterState(_origin.m_monsterState)
	, m_monsterDirection(_origin.m_monsterDirection)
	, m_currentStrategy(_origin.m_currentStrategy)
	, m_pHitSound(_origin.m_pHitSound)
{
}

CMonster::~CMonster()
{
}

MONSTER_STRATEGY CMonster::RandomPattern()
{
	// �迭 �� ���� ������ ��ȯ, ���� ����
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

	m_monsterState = MONSTER_STATE::MOVE;
	AnimationDirection(L"move", true, _aniDirection);

	// �Ҽ��� ������ ��ȯ�ǰ�
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

	printf("CMonster::Attack :: ���� ����\n");
}

void CMonster::Skill()
{
	m_monsterState = MONSTER_STATE::SKILL;
	AnimationDirection(L"magic", false);

	printf("CMonster::Skill :: ��ų ����\n");
}

void CMonster::AnimationEvent()
{
	switch (m_monsterState)
	{
	case MONSTER_STATE::ATTACK:
	{
		CCamera::GetInstance()->SetVibrateCamera(10.f, 1, 0.02f);
		AttackStartEvent(m_fAtt);
		printf("CMonster::AnimationEvent :: ���� ���� �ִϸ��̼� �̺�Ʈ ����\n");

		break;
	}
	case MONSTER_STATE::SKILL:
	{
		printf("CMonster::AnimationEvent :: ���� ��ų �ִϸ��̼� �̺�Ʈ ����\n");
		m_fHP += m_fAtt;
		if (m_fHP > m_fMaxHP) m_fHP = m_fMaxHP;
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
		printf("CMonster::AnimationEvent :: ���� ���� �ִϸ��̼� ���� ����\n");

		break;
	}
	case MONSTER_STATE::SKILL:
	{
		printf("CMonster::AnimationEvent :: ���� ��ų �ִϸ��̼� ���� ����\n");
		AnimationDirection(L"idle", true);
		SetActing(true);
		break;
	}
	case MONSTER_STATE::DAMAGED:
	{
		m_monsterState = MONSTER_STATE::IDLE;
		AnimationDirection(L"idle", true);
		printf("CPlayer::AnimationEnd :: ���� �ǰ� �ִϸ��̼� ���� ����\n");
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

	// ������ ���
	Vec2 vRenderPos = CCamera::GetInstance()->GetRenderPos(GetPos());
	vRenderPos.y -= 100.f;

	CPanelUI_Number* pNumber = new CPanelUI_Number((int)_damaged, vRenderPos, 1.0, 1.f);
	CreateObj(pNumber, GROUP_TYPE::NUMBER);

	// ���� ���
	m_pHitSound->Play(false);

	if (m_fHP <= 0)
	{
		m_fHP = 0;
		Died();

		// �����
		printf("���� ���\n");

		return;
	}
	else
	{
		m_monsterState = MONSTER_STATE::DAMAGED;
		AnimationDirection(L"damaged", false);

		// �����
		printf("���� ������ ���� : %1.f, ���� ü�� : %1.f\n", _damaged, m_fHP);
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
	GetAnimator()->Update();
}

void CMonster::Render(HDC hdc)
{
	ComponetRender(hdc);
}

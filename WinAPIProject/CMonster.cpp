#include "pch.h"
#include "CMonster.h"

#include "CTimeMgr.h"
#include "CDataMgr.h"
#include "CEventMgr.h"

#include "CAnimator.h"
#include "CAnimation.h"

#include "IStrategy.h"
#include "Barrier.h"
#include "Heal.h"
#include "RandomMove.h"
#include "Teleport.h"
#include "Trace.h"
#include "SelfHeal.h"
#include "FriendlyHeal.h"

#include "MonsterData.h"


CMonster::CMonster(int _key)
	: m_vecStrategy{}
{
	MonsterData* monData = (MonsterData*)CDataMgr::GetInstance()->FindData(_key);
	m_strName = monData->m_strName;
	
	m_fHP = monData->m_fHP;
	m_fMaxHP = monData->m_fHP;
	m_fAtt = monData->m_fAtt;
	m_iMove = monData->m_iMove;
	m_iRange = monData->m_iRange;

	for (auto& strategy : monData->m_vecMosterStrategy)
	{
		switch (strategy)
		{
		case MONSTER_STRATEGY::TRACE:
		{
			IMonsterStrategy* monStrategy = new Trace;
			m_vecStrategy.push_back(monStrategy);
		}
		case MONSTER_STRATEGY::RANDOM_MOVE: break;
		{
			IMonsterStrategy* monStrategy = new RandomMove;
			m_vecStrategy.push_back(monStrategy);
		}
		case MONSTER_STRATEGY::SELF_HEAL: break;
		{
			IMonsterStrategy* monStrategy = new SelfHeal;
			m_vecStrategy.push_back(monStrategy);
		}
		case MONSTER_STRATEGY::RANDOM_TELEPORT: break;
		{
			IMonsterStrategy* monStrategy = new Teleport;
			m_vecStrategy.push_back(monStrategy);
		}
		case MONSTER_STRATEGY::FRIENDLY_HEAL: break;
		{
			IMonsterStrategy* monStrategy = new FriendlyHeal;
			m_vecStrategy.push_back(monStrategy);
		}
		default:
			assert(0);	// ���� ����
		}
	}
	SetAnimator(monData->m_iAnimatorKey);
}

CMonster::CMonster(MonsterData* _data)
	: m_strName(_data->m_strName)
	, m_vecStrategy{}
	, m_fHP(_data->m_fHP)
	, m_fMaxHP(_data->m_fHP)
	, m_fAtt(_data->m_fAtt)
	, m_iMove(_data->m_iMove)
	, m_iRange(_data->m_iRange)
{
	for (auto& strategy : _data->m_vecMosterStrategy)
	{
		switch (strategy)
		{
		case MONSTER_STRATEGY::TRACE:
		{
			IMonsterStrategy* monStrategy = new Trace;
			m_vecStrategy.push_back(monStrategy);
			break;
		}
		case MONSTER_STRATEGY::RANDOM_MOVE:
		{
			IMonsterStrategy* monStrategy = new RandomMove;
			m_vecStrategy.push_back(monStrategy);
			break;
		}
		case MONSTER_STRATEGY::SELF_HEAL:
		{
			IMonsterStrategy* monStrategy = new SelfHeal;
			m_vecStrategy.push_back(monStrategy);
			break;
		}
		case MONSTER_STRATEGY::RANDOM_TELEPORT:
		{
			IMonsterStrategy* monStrategy = new Teleport;
			m_vecStrategy.push_back(monStrategy);
			break;
		}
		case MONSTER_STRATEGY::FRIENDLY_HEAL:
		{
			IMonsterStrategy* monStrategy = new FriendlyHeal;
			m_vecStrategy.push_back(monStrategy);
			break;
		}
		case MONSTER_STRATEGY::SELF_BARRIER:
		{
			IMonsterStrategy* monStrategy = new Barrier;
			m_vecStrategy.push_back(monStrategy);
			break;
		}
		default:
			assert(0);	// ���� ����
		}
	}
	SetAnimator(_data->m_iAnimatorKey);
}

CMonster::CMonster(const CMonster& _origin)
	: CObject(_origin)
	, m_strName(_origin.m_strName)
	, m_fHP(_origin.m_fHP)
	, m_fMaxHP(_origin.m_fMaxHP)
	, m_fAtt(_origin.m_fAtt)
	, m_iMove(_origin.m_iMove)
	, m_iRange(_origin.m_iRange)
{
	for (auto& strategy : _origin.m_vecStrategy)
	{
		IMonsterStrategy* newStrategy = strategy->Clone(); // Clone�� �� Strategy Ŭ�������� ����
		m_vecStrategy.push_back(newStrategy);
	}
}

CMonster::~CMonster()
{
	SafeDeleteVec(m_vecStrategy);
}

void CMonster::MovePattern(IMonsterStrategy* _stratey)
{
	_stratey->MoveStrategy(this);
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
		Died(this);

		// �����
		printf("���� ���\n");

		return;
	}
	else
	{
		GetAnimator()->PlayType(L"front_damaged", false);

		// �����
		printf("������ ���� : %1.f, ���� ü�� : %1.f\n", _damaged, m_fHP);
	}
}

void CMonster::Died(CObject* _pMonster)
{
	tEvent evn = {  };
	evn.eEvent = EVENT_TYPE::MONSTER_DIED;
	evn.lParam = (DWORD_PTR)_pMonster;

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

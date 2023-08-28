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

		// 디버깅
		printf("몬스터 사망\n");

		return;
	}
	else
	{
		GetAnimator()->PlayType(L"front_damaged", false);

		// 디버깅
		printf("데미지 받음 : %1.f, 남은 체력 : %1.f\n", _damaged, m_fHP);
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

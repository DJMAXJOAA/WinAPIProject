#include "pch.h"
#include "CMonsterFactory.h"

#include "CMonster.h"

#include "AI.h"
#include "CIdleState.h"
#include "CTraceState.h"

CMonsterFactory::CMonsterFactory()
{
}

CMonsterFactory::~CMonsterFactory()
{
}

CMonster* CMonsterFactory::CreateMonster(MON_TYPE _eType, Vec2 _vPos)
{
	CMonster* pMon = nullptr;

	switch (_eType)
	{
	case MON_TYPE::NORMAL:
	{
		pMon = new CMonster;
		pMon->SetPos(_vPos);

		AI* pAI = new AI;
		pAI->AddState(new CIdleState);
		pAI->AddState(new CTraceState);
		pAI->SetCurState(MON_STATE::IDLE);
	}
		break;
	case MON_TYPE::RANGE:
		break;
	default:
		break;
	}

	return pMon;
}
#include "pch.h"
#include "Monster_SelfHeal.h"

#include "CMonster.h"

void Monster_SelfHeal::Handle(CScene_Battle* _pScene, CMonster* _pMon)
{
	_pMon->GetRoute().clear();
	_pMon->Skill();
}

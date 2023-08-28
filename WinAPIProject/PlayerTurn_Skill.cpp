#include "pch.h"
#include "PlayerTurn_Skill.h"

#include "CScene_Battle.h"
#include "CPlayer.h"

#include "CTurnManager.h"

void PlayerTurn_Skill::Handle(CScene_Battle* _pScene)
{
	CTurnCenter* m_TurnCenter = _pScene->GetTurnCenter();
	CPlayer* m_pPlayer = _pScene->GetPlayer();

	if (m_pPlayer->IsAttacking() == false)
	{
		int combo = m_TurnCenter->GetCombo();

		printf("PlayerTurn_Skill::Handle :: 스킬을 발동합니다.\n");
		m_pPlayer->SetAttacking(true);
		m_pPlayer->PlayerSkillStart(combo);
	}
}

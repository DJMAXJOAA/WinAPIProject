#include "pch.h"
#include "PlayerTurn_Attack.h"

#include "CScene_Battle.h"
#include "CPlayer.h"
#include "CMonster.h"

#include "CTurnManager.h"

void PlayerTurn_Attack::Handle(CScene_Battle* _pScene)
{
	CTurnCenter* m_TurnCenter = _pScene->GetTurnCenter();
	CPlayer* m_pPlayer = _pScene->GetPlayer();

	list<CObject*>& targetList = m_TurnCenter->GetTargetList();

	// 타겟이 없으면, 다시 Move이벤트 진행
	if (targetList.empty())
	{
		printf("리스트가 비어서, MOVE 이벤트로 전환합니다.\n");
		m_TurnCenter->ChangeTurn(TURN_TYPE::PLAYER_MOVE);
		return;
	}

	// 리스트의 가장 위의 적군
	CMonster* pMonster = (CMonster*)targetList.front();
	GRID_DIRECTION gridDirection = GetGridDirection(m_pPlayer->GetGridPos(), pMonster->GetGridPos(), m_pPlayer->GetPos(), pMonster->GetPos());
	m_pPlayer->PlayerAttackDone(gridDirection, pMonster);

}

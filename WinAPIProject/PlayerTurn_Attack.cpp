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

	list<CObject*> targetList = m_TurnCenter->GetTargetList();

	// Ÿ���� ������, �ٽ� Move�̺�Ʈ ����
	if (targetList.empty())
	{
		m_TurnCenter->SetTurnState(TURN_TYPE::PLAYER_MOVE);
		return;
	}

	// ����Ʈ�� ���� ���� ����
	CMonster* pMonster = (CMonster*)targetList.front();
	GRID_DIRECTION gridDirection = GetGridDirection(m_pPlayer->GetGridPos(), pMonster->GetGridPos(), m_pPlayer->GetPos(), pMonster->GetPos());
	m_pPlayer->Attack(gridDirection, pMonster);

}

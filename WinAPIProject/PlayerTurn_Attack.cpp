#include "pch.h"
#include "PlayerTurn_Attack.h"

#include "CScene_Battle.h"
#include "CPlayer.h"
#include "CMonster.h"

#include "CTurnManager.h"

void PlayerTurn_Attack::Init(CScene_Battle* _pScene)
{
	printf("CScene_Battle::TurnInit :: �÷��̾� ���� ���� �ʱ�ȭ\n");
}

void PlayerTurn_Attack::Handle(CScene_Battle* _pScene)
{
	CLogicCenter* m_TurnCenter = _pScene->GetLogicCenter();
	CPlayer* m_pPlayer = _pScene->GetPlayer();

	list<CObject*>& targetList = m_TurnCenter->GetTargetList();

	// ����Ʈ�� ��������� �ٽ� Move���·� ��ȯ
	if (targetList.empty())
	{
		_pScene->ChangeTurn(TURN_TYPE::PLAYER_MOVE);
		return;
	}

	// BFS Ž�� ��� ����Ʈ ��� ���͵鿡�� ���� ó��
	CMonster* pMonster = (CMonster*)targetList.front();
	GRID_DIRECTION gridDirection = GetGridDirection(m_pPlayer->GetGridPos(), pMonster->GetGridPos(), m_pPlayer->GetPos(), pMonster->GetPos());
	
	if(m_pPlayer->IsAttacking() == false)
	{
		// ���� �ִϸ��̼� ���� �� �ִϸ��̼� �̺�Ʈ�� ȣ��Ǿ�
		// ���� ���� ������ Ž�� ��� ����Ʈ���� ���ŵȴ�.
		m_pPlayer->SetAttacking(true);
		m_pPlayer->Attack(gridDirection, pMonster);
	}
}

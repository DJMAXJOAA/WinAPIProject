#include "pch.h"
#include "PlayerTurn_Attack.h"

#include "CScene_Battle.h"
#include "CPlayer.h"
#include "CMonster.h"

#include "CTurnManager.h"

void PlayerTurn_Attack::Init(CScene_Battle* _pScene)
{
	_pScene->SetBattleState(TURN_TYPE::PLAYER_ATTACK);

	printf("CScene_Battle::TurnInit :: �÷��̾� ���� ���� �ʱ�ȭ\n");
}

void PlayerTurn_Attack::Handle(CScene_Battle* _pScene)
{
	CTurnCenter* m_TurnCenter = _pScene->GetTurnCenter();
	CPlayer* m_pPlayer = _pScene->GetPlayer();

	list<CObject*>& targetList = m_TurnCenter->GetTargetList();

	// Ÿ���� ������, �ٽ� Move�̺�Ʈ ����
	if (targetList.empty())
	{
		printf("PlayerTurn_Attack::Handle :: ����Ʈ�� ��, MOVE �̺�Ʈ�� ��ȯ�մϴ�.\n");
		m_TurnCenter->ChangeTurn(TURN_TYPE::PLAYER_MOVE);
		return;
	}

	// ����Ʈ�� ���� ���� ����
	CMonster* pMonster = (CMonster*)targetList.front();
	GRID_DIRECTION gridDirection = GetGridDirection(m_pPlayer->GetGridPos(), pMonster->GetGridPos(), m_pPlayer->GetPos(), pMonster->GetPos());
	
	if(m_pPlayer->IsAttacking() == false)
	{
		printf("PlayerTurn_Attack::Handle :: ������ �����մϴ�. Target Monster -> ");
		cout << pMonster << "\n";
		m_pPlayer->SetAttacking(true);
		m_pPlayer->Attack(gridDirection, pMonster);
	}
}

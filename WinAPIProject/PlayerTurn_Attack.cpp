#include "pch.h"
#include "PlayerTurn_Attack.h"

#include "CScene_Battle.h"
#include "CPlayer.h"
#include "CMonster.h"

#include "CTurnManager.h"

void PlayerTurn_Attack::Init(CScene_Battle* _pScene)
{
	_pScene->SetBattleState(TURN_TYPE::PLAYER_ATTACK);

	printf("CScene_Battle::TurnInit :: 플레이어 공격 상태 초기화\n");
}

void PlayerTurn_Attack::Handle(CScene_Battle* _pScene)
{
	CTurnCenter* m_TurnCenter = _pScene->GetTurnCenter();
	CPlayer* m_pPlayer = _pScene->GetPlayer();

	list<CObject*>& targetList = m_TurnCenter->GetTargetList();

	// 타겟이 없으면, 다시 Move이벤트 진행
	if (targetList.empty())
	{
		printf("PlayerTurn_Attack::Handle :: 리스트가 비어서, MOVE 이벤트로 전환합니다.\n");
		m_TurnCenter->ChangeTurn(TURN_TYPE::PLAYER_MOVE);
		return;
	}

	// 리스트의 가장 위의 적군
	CMonster* pMonster = (CMonster*)targetList.front();
	GRID_DIRECTION gridDirection = GetGridDirection(m_pPlayer->GetGridPos(), pMonster->GetGridPos(), m_pPlayer->GetPos(), pMonster->GetPos());
	
	if(m_pPlayer->IsAttacking() == false)
	{
		printf("PlayerTurn_Attack::Handle :: 공격을 시작합니다. Target Monster -> ");
		cout << pMonster << "\n";
		m_pPlayer->SetAttacking(true);
		m_pPlayer->Attack(gridDirection, pMonster);
	}
}

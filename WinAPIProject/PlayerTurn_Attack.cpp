#include "pch.h"
#include "PlayerTurn_Attack.h"

#include "CScene_Battle.h"
#include "CPlayer.h"
#include "CMonster.h"

#include "CTurnManager.h"

void PlayerTurn_Attack::Init(CScene_Battle* _pScene)
{
	printf("CScene_Battle::TurnInit :: 플레이어 공격 상태 초기화\n");
}

void PlayerTurn_Attack::Handle(CScene_Battle* _pScene)
{
	CLogicCenter* m_TurnCenter = _pScene->GetLogicCenter();
	CPlayer* m_pPlayer = _pScene->GetPlayer();

	list<CObject*>& targetList = m_TurnCenter->GetTargetList();

	// 리스트가 비어있으면 다시 Move상태로 전환
	if (targetList.empty())
	{
		_pScene->ChangeTurn(TURN_TYPE::PLAYER_MOVE);
		return;
	}

	// BFS 탐색 결과 리스트 결과 몬스터들에게 공격 처리
	CMonster* pMonster = (CMonster*)targetList.front();
	GRID_DIRECTION gridDirection = GetGridDirection(m_pPlayer->GetGridPos(), pMonster->GetGridPos(), m_pPlayer->GetPos(), pMonster->GetPos());
	
	if(m_pPlayer->IsAttacking() == false)
	{
		// 공격 애니메이션 종료 시 애니메이션 이벤트가 호출되어
		// 공격 받은 적군이 탐색 결과 리스트에서 제거된다.
		m_pPlayer->SetAttacking(true);
		m_pPlayer->Attack(gridDirection, pMonster);
	}
}

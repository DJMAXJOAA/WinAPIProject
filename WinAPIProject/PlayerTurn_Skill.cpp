#include "pch.h"
#include "PlayerTurn_Skill.h"

#include "CPlayer.h"
#include "CTile.h"

#include "BFSSearch.h"
#include "CTileManager.h"
#include "CTurnManager.h"

#include "CScene_Battle.h"

void PlayerTurn_Skill::Init(CScene_Battle* _pScene)
{
	m_bUseSkill = true;

	CLogicCenter* m_TurnCenter = _pScene->GetLogicCenter();
	CPlayer* m_pPlayer = _pScene->GetPlayer();
	CTileCenter* m_TileCenter = _pScene->GetTileCenter();

	// 카메라 타겟 해제
	CCamera::GetInstance()->SetTarget(nullptr);

	// 검은 타일들(밟고 지나왔던 타일들) 랜덤 타일들로 리셋시키기
	m_TileCenter->TileRandomInit();

	if (m_TurnCenter->GetCombo() < 4)
	{
		printf("CScene_Battle::TurnInit :: 스킬 초기화 -> 콤보가 4 이하라서 스킬을 사용하지 않습니다.\n");

		m_bUseSkill = false;
		_pScene->ChangeTurn(TURN_TYPE::ENEMY_MOVE);
		return;
	}

	// BFS로 스킬 적중 대상 탐색
	vector<vector<TileState>>& vecTile = m_TileCenter->GetTiles();
	list<CObject*>& lstMonsters = m_TurnCenter->GetTargetList();
	BFSSearch::BFS(m_pPlayer->GetGridPos(), vecTile, lstMonsters, DIRECTION::FOUR_WAY, 3);

	// 플레이어 탐색 제거
	auto it = lstMonsters.begin();
	while (it != lstMonsters.end())
	{
		if (*it == m_pPlayer)
		{
			it = lstMonsters.erase(it);
		}
		else
		{
			++it;
		}
	}

	printf("CScene_Battle::TurnInit :: BFS 탐색 결과 -> ");
	for (list<CObject*>::iterator iter = lstMonsters.begin(); iter != lstMonsters.end(); iter++)
	{
		cout << *iter << ", ";
	}
	printf("\n");

	// 만약 적중대상이 없거나 콤보가 4 이하라면, 다시 초기상태로 돌아감
	if (lstMonsters.empty())
	{
		printf("CScene_Battle::TurnInit :: 스킬 초기화 -> 적중 대상이 없어서 돌아갑니다.\n");
		
		m_bUseSkill = false;
		_pScene->ChangeTurn(TURN_TYPE::ENEMY_MOVE);
		return;
	}

	printf("CScene_Battle::TurnInit :: 플레이어 스킬 상태 초기화\n");
}

void PlayerTurn_Skill::Handle(CScene_Battle* _pScene)
{
	if (!m_bUseSkill) return;

	CLogicCenter* m_TurnCenter = _pScene->GetLogicCenter();
	CPlayer* m_pPlayer = _pScene->GetPlayer();

	if (m_pPlayer->IsAttacking() == false)
	{
		int combo = m_TurnCenter->GetCombo();

		printf("PlayerTurn_Skill::Handle :: 스킬을 발동합니다.\n");
		m_pPlayer->SetAttacking(true);
		m_pPlayer->Skill(combo);
	}
}

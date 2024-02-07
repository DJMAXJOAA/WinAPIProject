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

	// ī�޶� Ÿ�� ����
	CCamera::GetInstance()->SetTarget(nullptr);

	// ���� Ÿ�ϵ�(��� �����Դ� Ÿ�ϵ�) ���� Ÿ�ϵ�� ���½�Ű��
	m_TileCenter->TileRandomInit();

	if (m_TurnCenter->GetCombo() < 4)
	{
		printf("CScene_Battle::TurnInit :: ��ų �ʱ�ȭ -> �޺��� 4 ���϶� ��ų�� ������� �ʽ��ϴ�.\n");

		m_bUseSkill = false;
		_pScene->ChangeTurn(TURN_TYPE::ENEMY_MOVE);
		return;
	}

	// BFS�� ��ų ���� ��� Ž��
	vector<vector<TileState>>& vecTile = m_TileCenter->GetTiles();
	list<CObject*>& lstMonsters = m_TurnCenter->GetTargetList();
	BFSSearch::BFS(m_pPlayer->GetGridPos(), vecTile, lstMonsters, DIRECTION::FOUR_WAY, 3);

	// �÷��̾� Ž�� ����
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

	printf("CScene_Battle::TurnInit :: BFS Ž�� ��� -> ");
	for (list<CObject*>::iterator iter = lstMonsters.begin(); iter != lstMonsters.end(); iter++)
	{
		cout << *iter << ", ";
	}
	printf("\n");

	// ���� ���ߴ���� ���ų� �޺��� 4 ���϶��, �ٽ� �ʱ���·� ���ư�
	if (lstMonsters.empty())
	{
		printf("CScene_Battle::TurnInit :: ��ų �ʱ�ȭ -> ���� ����� ��� ���ư��ϴ�.\n");
		
		m_bUseSkill = false;
		_pScene->ChangeTurn(TURN_TYPE::ENEMY_MOVE);
		return;
	}

	printf("CScene_Battle::TurnInit :: �÷��̾� ��ų ���� �ʱ�ȭ\n");
}

void PlayerTurn_Skill::Handle(CScene_Battle* _pScene)
{
	if (!m_bUseSkill) return;

	CLogicCenter* m_TurnCenter = _pScene->GetLogicCenter();
	CPlayer* m_pPlayer = _pScene->GetPlayer();

	if (m_pPlayer->IsAttacking() == false)
	{
		int combo = m_TurnCenter->GetCombo();

		printf("PlayerTurn_Skill::Handle :: ��ų�� �ߵ��մϴ�.\n");
		m_pPlayer->SetAttacking(true);
		m_pPlayer->Skill(combo);
	}
}

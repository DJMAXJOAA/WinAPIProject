#include "pch.h"
#include "CEventCenter.h"

#include "CMonster.h"
#include "CPlayer.h"

#include "CBattleState.h"
#include "CTileManager.h"
#include "CTurnManager.h"
#include "CBattleStateMachine.h"

#include "CScene_Battle.h"
#include "CEffect.h"

CEventCenter::CEventCenter(CScene_Battle* _pScene)
	: m_pScene(_pScene)
{
}

CEventCenter::~CEventCenter()
{
}

void CEventCenter::OnTileSelect(CObject* _pObj)
{
	CTileCenter* m_pTileCenter = m_pScene->GetTileCenter();

	// ���� :: ���콺�� �� ���� ���¿��� Ÿ���� �ݶ��̴��� ���� ����
	// ���콺 �� ���� ���¿���, �ݶ��̴��� ������ -> �̺�Ʈ �Ŵ������� �� �Լ��� ȣ���Ŵ
	m_pScene->GetStateMachine()->GetState()->OnTileSelect(m_pScene, _pObj);

	// BFS �湮 �ʱ�ȭ
	m_pTileCenter->TileVisitedInit();
}

void CEventCenter::PlayerAttackMonster(float _damage, CMonster* _pMon)
{
	// �ǰ� ����Ʈ ����
	CEffect* pEffect = new CEffect(209, _pMon->GetPos());
	CreateObj(pEffect, GROUP_TYPE::EFFECT);

	_pMon->GetDamaged(_damage);

	printf("CScene_Battle::PlayerAttackMonster :: ������ %1.f �������� ���� ->", _damage);
	cout << _pMon << "\n";
}

void CEventCenter::PlayerAttackDone()
{
	list<CObject*>& list = m_pScene->GetLogicCenter()->GetTargetList();
	m_pScene->GetPlayer()->SetAttacking(false);

	if (list.empty())
	{
		printf("CScene_Battle::PlayerAttackDone :: ����Ʈ�� ������ϴ�.\n");
		return;
	}
	else
	{
		printf("CScene_Battle::PlayerAttackDone :: Ÿ�� ����Ʈ�� �ϳ� �����մϴ� ->");
		cout << list.front() << "�� �����߽��ϴ�. \n";
		list.pop_front();
		return;
	}
}

void CEventCenter::PlayerSkillCasted(float _value)
{
	list<CObject*>& list = m_pScene->GetLogicCenter()->GetTargetList();
	if (list.empty())
	{
		printf("CScene_Battle::PlayerSkillCasted :: ����Ʈ�� ������ϴ�.\n");
		return;
	}
	else
	{
		printf("CScene_Battle::PlayerSkillCasted :: ����Ʈ ��󿡰� ��ų�� �����մϴ�.\n");
		printf("CScene_Battle::PlayerSkillCasted :: ���鿡�� %1.f �������� ��ų ���� ->", _value);
		for (auto iter = list.begin(); iter != list.end(); iter++)
		{
			// �ǰ� ����Ʈ ����
			CEffect* pEffect = new CEffect(209, ((CMonster*)*iter)->GetPos());
			CreateObj(pEffect, GROUP_TYPE::EFFECT);

			((CMonster*)*iter)->GetDamaged(_value);
		}

		list.clear();
		return;
	}
}

void CEventCenter::PlayerSkillDone()
{
	m_pScene->GetPlayer()->SetAttacking(false);
	m_pScene->ChangeTurn(TURN_TYPE::ENEMY_MOVE);

	printf("CScene_Battle::PlayerSkillDone :: ��ų�� ����Ǿ����ϴ�.\n");
}

void CEventCenter::MonsterAttackPlayer(float _damage)
{
	CPlayer* m_pPlayer = m_pScene->GetPlayer();

	// �ǰ� ����Ʈ ����
	CEffect* pEffect = new CEffect(209, m_pPlayer->GetPos());
	CreateObj(pEffect, GROUP_TYPE::EFFECT);

	m_pPlayer->GetDamaged(_damage);

	printf("CScene_Battle::MonsterAttackPlayer :: ���Ͱ� �÷��̾�� %1.f �������� ���� ->", _damage);
	cout << m_pPlayer << "\n";
}

void CEventCenter::MonsterAttackDone(CMonster* _pMon)
{
	_pMon->SetActing(true);
	_pMon->AnimationDirection(L"idle", true);

	printf("CScene_Battle::MonsterAttackDone :: ������ ����Ǿ����ϴ�.\n");
}

void CEventCenter::MonsterDied(CMonster* _pObj)
{
	CPlayer* m_pPlayer = m_pScene->GetPlayer();

	int money = _pObj->GetMoney();
	m_pPlayer->SetMoney(m_pPlayer->GetMoney() + money);
	printf("CScene_Battle::MonsterDied :: %d���� �� ���� %d���� �ƽ��ϴ�.\n", money, m_pPlayer->GetMoney());

	Vec2 GridPos = _pObj->GetGridPos();
	vector<vector<TileState>>& vecTiles = m_pScene->GetTileCenter()->GetTiles();

	vecTiles[(int)GridPos.y][(int)GridPos.x].pObj = nullptr;

	printf("CScene_Battle::MonsterDied :: %d, %d Ÿ�� ���� ������Ʈ�� �ʱ�ȭ ���׽��ϴ�.\n", (int)GridPos.x, (int)GridPos.y);
}

void CEventCenter::PlayerDied()
{
	m_pScene->ChangeTurn(TURN_TYPE::DEFEAT);

	printf("CScene_Battle::PlayerDied :: �÷��̾� ���\n");
}

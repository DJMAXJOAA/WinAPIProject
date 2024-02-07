#include "pch.h"
#include "CEventCenter.h"
#include "CTileManager.h"
#include "CTurnManager.h"
#include "CBattleState.h"

#include "CScene_Battle.h"

CEventCenter::CEventCenter(CScene_Battle* _pScene)
	: m_pScene(_pScene)
{
}

CEventCenter::~CEventCenter()
{
}

//void CEventCenter::OnTileSelect(CObject* _pObj)
//{
//	CLogicCenter* m_pTurnCenter = m_pScene->GetTurnCenter();
//	CTileCenter* m_pTileCenter = m_pScene->GetTileCenter();
//
//	// ���� :: ���콺�� �� ���� ���¿��� Ÿ���� �ݶ��̴��� ���� ����
//	// ���콺 �� ���� ���¿���, �ݶ��̴��� ������ -> �̺�Ʈ �Ŵ������� �� �Լ��� ȣ���Ŵ
//	TURN_TYPE CurrnetTurn = m_pTurnCenter->GetTurnState();
//	vector<vector<TileState>>& vecTiles = m_pTileCenter->GetTiles();
//
//	// PLAYER_START, PLAYER_TILESELECT
//	m_vecStates[(int)CurrnetTurn]->OnTileSelect(this, _pObj);
//
//	// BFS �湮 �ʱ�ȭ
//	m_pTileCenter->TileVisitedInit();
//}
//
//void CEventCenter::OnChangeTurn(TURN_TYPE _type)
//{
//	m_vecStates[(int)_type]->Init(this);
//}
//
//void CEventCenter::PlayerAttackMonster(float _damage, CMonster* _pMon)
//{
//	// �ǰ� ����Ʈ ����
//	CEffect* pEffect = new CEffect(209, _pMon->GetPos());
//	CreateObj(pEffect, GROUP_TYPE::EFFECT);
//
//	_pMon->GetDamaged(_damage);
//
//	printf("CScene_Battle::PlayerAttackMonster :: ������ %1.f �������� ���� ->", _damage);
//	cout << _pMon << "\n";
//}
//
//void CEventCenter::PlayerAttackDone()
//{
//	list<CObject*>& list = m_pTurnCenter->GetTargetList();
//	m_pPlayer->SetAttacking(false);
//
//	if (list.empty())
//	{
//		printf("CScene_Battle::PlayerAttackDone :: ����Ʈ�� ������ϴ�.\n");
//		return;
//	}
//	else
//	{
//		printf("CScene_Battle::PlayerAttackDone :: Ÿ�� ����Ʈ�� �ϳ� �����մϴ� ->");
//		cout << list.front() << "�� �����߽��ϴ�. \n";
//		list.pop_front();
//		return;
//	}
//}
//
//void CEventCenter::PlayerSkillCasted(float _value)
//{
//	list<CObject*>& list = m_pTurnCenter->GetTargetList();
//	if (list.empty())
//	{
//		printf("CScene_Battle::PlayerSkillCasted :: ����Ʈ�� ������ϴ�.\n");
//		return;
//	}
//	else
//	{
//		printf("CScene_Battle::PlayerSkillCasted :: ����Ʈ ��󿡰� ��ų�� �����մϴ�.\n");
//		printf("CScene_Battle::PlayerSkillCasted :: ���鿡�� %1.f �������� ��ų ���� ->", _value);
//		for (auto iter = list.begin(); iter != list.end(); iter++)
//		{
//			// �ǰ� ����Ʈ ����
//			CEffect* pEffect = new CEffect(209, ((CMonster*)*iter)->GetPos());
//			CreateObj(pEffect, GROUP_TYPE::EFFECT);
//
//			((CMonster*)*iter)->GetDamaged(_value);
//		}
//
//		list.clear();
//		return;
//	}
//}
//
//void CEventCenter::PlayerSkillDone()
//{
//	m_pPlayer->SetAttacking(false);
//	m_pTurnCenter->ChangeTurn(TURN_TYPE::ENEMY_MOVE);
//
//	printf("CScene_Battle::PlayerSkillDone :: ��ų�� ����Ǿ����ϴ�.\n");
//}
//
//void CEventCenter::MonsterAttackPlayer(float _damage)
//{
//	// �ǰ� ����Ʈ ����
//	CEffect* pEffect = new CEffect(209, m_pPlayer->GetPos());
//	CreateObj(pEffect, GROUP_TYPE::EFFECT);
//
//	m_pPlayer->GetDamaged(_damage);
//
//	printf("CScene_Battle::MonsterAttackPlayer :: ���Ͱ� �÷��̾�� %1.f �������� ���� ->", _damage);
//	cout << m_pPlayer << "\n";
//}
//
//void CEventCenter::MonsterAttackDone(CMonster* _pMon)
//{
//	_pMon->SetActing(true);
//	_pMon->GetAnimator()->PlayType(L"front_idle", true);
//
//	printf("CScene_Battle::MonsterAttackDone :: ������ ����Ǿ����ϴ�.\n");
//}
//
//void CEventCenter::MonsterDied(CMonster* _pObj)
//{
//	int money = _pObj->GetMoney();
//	m_pPlayer->SetMoney(m_pPlayer->GetMoney() + money);
//	printf("CScene_Battle::MonsterDied :: %d���� �� ���� %d���� �ƽ��ϴ�.\n", money, m_pPlayer->GetMoney());
//
//	Vec2 GridPos = _pObj->GetGridPos();
//	vector<vector<TileState>>& vecTiles = m_pTileCenter->GetTiles();
//
//	vecTiles[(int)GridPos.y][(int)GridPos.x].pObj = nullptr;
//
//	printf("CScene_Battle::MonsterDied :: %d, %d Ÿ�� ���� ������Ʈ�� �ʱ�ȭ ���׽��ϴ�.\n", (int)GridPos.x, (int)GridPos.y);
//}
//
//void CEventCenter::PlayerDied()
//{
//	m_pTurnCenter->ChangeTurn(TURN_TYPE::DEFEAT);
//
//	printf("CScene_Battle::PlayerDied :: �÷��̾� ���\n");
//}

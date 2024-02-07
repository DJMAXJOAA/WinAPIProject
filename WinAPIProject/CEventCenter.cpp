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

	// 조건 :: 마우스를 꾹 누른 상태에서 타일의 콜라이더와 닿은 상태
	// 마우스 꾹 눌린 상태에서, 콜라이더가 닿으면 -> 이벤트 매니저에서 이 함수를 호출시킴
	m_pScene->GetStateMachine()->GetState()->OnTileSelect(m_pScene, _pObj);

	// BFS 방문 초기화
	m_pTileCenter->TileVisitedInit();
}

void CEventCenter::PlayerAttackMonster(float _damage, CMonster* _pMon)
{
	// 피격 이펙트 생성
	CEffect* pEffect = new CEffect(209, _pMon->GetPos());
	CreateObj(pEffect, GROUP_TYPE::EFFECT);

	_pMon->GetDamaged(_damage);

	printf("CScene_Battle::PlayerAttackMonster :: 적에게 %1.f 데미지로 공격 ->", _damage);
	cout << _pMon << "\n";
}

void CEventCenter::PlayerAttackDone()
{
	list<CObject*>& list = m_pScene->GetLogicCenter()->GetTargetList();
	m_pScene->GetPlayer()->SetAttacking(false);

	if (list.empty())
	{
		printf("CScene_Battle::PlayerAttackDone :: 리스트가 비었습니다.\n");
		return;
	}
	else
	{
		printf("CScene_Battle::PlayerAttackDone :: 타겟 리스트를 하나 삭제합니다 ->");
		cout << list.front() << "를 삭제했습니다. \n";
		list.pop_front();
		return;
	}
}

void CEventCenter::PlayerSkillCasted(float _value)
{
	list<CObject*>& list = m_pScene->GetLogicCenter()->GetTargetList();
	if (list.empty())
	{
		printf("CScene_Battle::PlayerSkillCasted :: 리스트가 비었습니다.\n");
		return;
	}
	else
	{
		printf("CScene_Battle::PlayerSkillCasted :: 리스트 대상에게 스킬을 시전합니다.\n");
		printf("CScene_Battle::PlayerSkillCasted :: 적들에게 %1.f 데미지로 스킬 시전 ->", _value);
		for (auto iter = list.begin(); iter != list.end(); iter++)
		{
			// 피격 이펙트 생성
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

	printf("CScene_Battle::PlayerSkillDone :: 스킬이 종료되었습니다.\n");
}

void CEventCenter::MonsterAttackPlayer(float _damage)
{
	CPlayer* m_pPlayer = m_pScene->GetPlayer();

	// 피격 이펙트 생성
	CEffect* pEffect = new CEffect(209, m_pPlayer->GetPos());
	CreateObj(pEffect, GROUP_TYPE::EFFECT);

	m_pPlayer->GetDamaged(_damage);

	printf("CScene_Battle::MonsterAttackPlayer :: 몬스터가 플레이어에게 %1.f 데미지로 공격 ->", _damage);
	cout << m_pPlayer << "\n";
}

void CEventCenter::MonsterAttackDone(CMonster* _pMon)
{
	_pMon->SetActing(true);
	_pMon->AnimationDirection(L"idle", true);

	printf("CScene_Battle::MonsterAttackDone :: 공격이 종료되었습니다.\n");
}

void CEventCenter::MonsterDied(CMonster* _pObj)
{
	CPlayer* m_pPlayer = m_pScene->GetPlayer();

	int money = _pObj->GetMoney();
	m_pPlayer->SetMoney(m_pPlayer->GetMoney() + money);
	printf("CScene_Battle::MonsterDied :: %d원을 얻어서 돈이 %d원이 됐습니다.\n", money, m_pPlayer->GetMoney());

	Vec2 GridPos = _pObj->GetGridPos();
	vector<vector<TileState>>& vecTiles = m_pScene->GetTileCenter()->GetTiles();

	vecTiles[(int)GridPos.y][(int)GridPos.x].pObj = nullptr;

	printf("CScene_Battle::MonsterDied :: %d, %d 타일 위의 오브젝트를 초기화 시켰습니다.\n", (int)GridPos.x, (int)GridPos.y);
}

void CEventCenter::PlayerDied()
{
	m_pScene->ChangeTurn(TURN_TYPE::DEFEAT);

	printf("CScene_Battle::PlayerDied :: 플레이어 사망\n");
}

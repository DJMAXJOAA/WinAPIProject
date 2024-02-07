#include "pch.h"
#include "CScene_Battle.h"

#include "CCollisionMgr.h"
#include "CKeyMgr.h"
#include "CDataMgr.h"
#include "CResMgr.h"

#include "CPlayer.h"
#include "CTile.h"
#include "CMouse.h"
#include "CMonster.h"
#include "CEffect.h"
#include "CPanelUI_Number.h"

#include "CSound.h"
#include "CAnimator.h"
#include "CAnimation.h"

#include "CBattleState.h"
#include "CEventCenter.h"
#include "CTurnManager.h"
#include "CTileManager.h"
#include "CMonsterSpawner.h"
#include "CBattleStateMachine.h"
#include "CEventMgr.h"

static std::random_device rd;
static std::mt19937 gen(rd());

static bool cheat;

CScene_Battle::CScene_Battle()
	: m_pPlayer(nullptr)
	, m_iFieldType(0)
	, m_iDifficulty(1)
	, m_FieldType(FIELD_TYPE::COMMON)
	, m_pMonsterSpawner(nullptr)
	, m_pEventCenter(nullptr)
	, m_pTurnCenter(nullptr)
	, m_pTileCenter(nullptr)
	, m_pStateMachine(nullptr)
	, m_pMoneyUI(nullptr)
	, m_pComboUI(nullptr)
	, m_vecSoundEffect((int)SOUND_TYPE::END)
{
	m_vecSoundEffect[(int)SOUND_TYPE::WIN] = CResMgr::GetInstance()->LoadSound(L"winSound", L"sound\\win.wav");
	m_vecSoundEffect[(int)SOUND_TYPE::LOSE] = CResMgr::GetInstance()->LoadSound(L"losdSound", L"sound\\lose.wav");
}

CScene_Battle::~CScene_Battle()
{
}

void CScene_Battle::PlayerAttackMonster(float _damage, CMonster* _pMon)
{
	// �ǰ� ����Ʈ ����
	CEffect* pEffect = new CEffect(209, _pMon->GetPos());
	CreateObj(pEffect, GROUP_TYPE::EFFECT);

	_pMon->GetDamaged(_damage);

	printf("CScene_Battle::PlayerAttackMonster :: ������ %1.f �������� ���� ->", _damage);
	cout << _pMon << "\n";
}

void CScene_Battle::PlayerAttackDone()
{
	list<CObject*>& list = m_pTurnCenter->GetTargetList();
	m_pPlayer->SetAttacking(false);

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

void CScene_Battle::PlayerSkillCasted(float _value)
{
	list<CObject*>& list = m_pTurnCenter->GetTargetList();
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

void CScene_Battle::PlayerSkillDone()
{
	m_pPlayer->SetAttacking(false);
	ChangeTurn(TURN_TYPE::ENEMY_MOVE);

	printf("CScene_Battle::PlayerSkillDone :: ��ų�� ����Ǿ����ϴ�.\n");
}

void CScene_Battle::MonsterAttackPlayer(float _damage)
{
	// �ǰ� ����Ʈ ����
	CEffect* pEffect = new CEffect(209, m_pPlayer->GetPos());
	CreateObj(pEffect, GROUP_TYPE::EFFECT);

	m_pPlayer->GetDamaged(_damage);

	printf("CScene_Battle::MonsterAttackPlayer :: ���Ͱ� �÷��̾�� %1.f �������� ���� ->", _damage);
	cout << m_pPlayer << "\n";
}

void CScene_Battle::MonsterAttackDone(CMonster* _pMon)
{
	_pMon->AnimationDirection(L"idle", true);
	_pMon->SetActing(true);

	printf("CScene_Battle::MonsterAttackDone :: ������ ����Ǿ����ϴ�.\n");
}

void CScene_Battle::MonsterDied(CMonster* _pObj)
{
	int money = _pObj->GetMoney();
	m_pPlayer->SetMoney(m_pPlayer->GetMoney() + money);
	printf("CScene_Battle::MonsterDied :: %d���� �� ���� %d���� �ƽ��ϴ�.\n", money, m_pPlayer->GetMoney());

	Vec2 GridPos = _pObj->GetGridPos();
	vector<vector<TileState>>& vecTiles = m_pTileCenter->GetTiles();

	vecTiles[(int)GridPos.y][(int)GridPos.x].pObj = nullptr;

	printf("CScene_Battle::MonsterDied :: %d, %d Ÿ�� ���� ������Ʈ�� �ʱ�ȭ ���׽��ϴ�.\n", (int)GridPos.x, (int)GridPos.y);
}

void CScene_Battle::PlayerDied()
{
	ChangeTurn(TURN_TYPE::DEFEAT);

	printf("CScene_Battle::PlayerDied :: �÷��̾� ���\n");
}

void CScene_Battle::ChangeTurn(TURN_TYPE _type)
{
	tEvent evn = {  };
	evn.eEvent = EVENT_TYPE::TURN_CHANGE;
	evn.lParam = (DWORD_PTR)_type;

	CEventMgr::GetInstance()->AddEventLate(evn);
}

void CScene_Battle::OnTileSelect(CObject* _pObj)
{
	// ���� :: ���콺�� �� ���� ���¿��� Ÿ���� �ݶ��̴��� ���� ����
	// ���콺 �� ���� ���¿���, �ݶ��̴��� ������ -> �̺�Ʈ �Ŵ������� �� �Լ��� ȣ���Ŵ
	m_pStateMachine->GetState()->OnTileSelect(this, _pObj);

	// BFS �湮 �ʱ�ȭ
	m_pTileCenter->TileVisitedInit();
}

void CScene_Battle::CameraEvent()
{
	ChangeScene(SCENE_TYPE::ROBBY);
}

void CScene_Battle::Update()
{
	InputKey();								// Ű �Է�
	SortGroupObj();							// ���� ������Ʈ�� ������ ���� ����(��ǥ�� �������� �� ���� ������)
	CScene::Update();						// ���� �� ������Ʈ (���� ������Ʈ���� ������Ʈ)

	// ���� ó�� (���� �ӽ�)
	m_pStateMachine->Handle(this);
	
	// ���� ���� �迭 ������Ʈ (��� ������ ������Ʈ���� ����)
	m_pMonsterSpawner->Update();

	// UI ������Ʈ (�ӽ�)
	m_pMoneyUI->SetNumber((int)m_pPlayer->GetMoney());
	m_pComboUI->SetNumber((int)m_pTurnCenter->GetCombo());
}

void CScene_Battle::Enter()
{
	// �Ŵ����� ��� -> ������Ʈ�� ����ϴ°� �ƴ϶� ���� ���� �����־�� �Ѵ�. Enter, Exit �Լ��� ���þ���
	// ���� ���� �ִ� �׷� ������Ʈ���� �����ؼ� �����ϴ� �����̶�, ���Ƿ� ������ ������Ű�� �ʰ� �����ؾ� �Ѵ�
	m_pTurnCenter = new CLogicCenter;
	m_pEventCenter = new CEventCenter(this);
	m_pMonsterSpawner = new CMonsterSpawner;
	m_pTileCenter = new CTileCenter;
	m_pStateMachine = new CBattleStateMachine(this);
}

void CScene_Battle::Exit()
{
	CCamera::GetInstance()->SetTarget(nullptr);

	m_pTurnCenter->Init();
	m_pTileCenter->Init();
	m_pMonsterSpawner->Init();
	DeleteAll();
	CCollisionMgr::GetInstance()->Reset();

	delete m_pMonsterSpawner;
	delete m_pEventCenter;
	delete m_pTurnCenter;
	delete m_pTileCenter;
	delete m_pStateMachine;
}

void CScene_Battle::InputKey()
{
	// ȭ�� ��ȯ
	if (KEY_TAP(KEY::ENTER))
	{
		ChangeScene(SCENE_TYPE::END);
	}

	// ġƮŰ
	if (KEY_TAP(KEY::P))
	{
		if (cheat == false)
		{
			m_pPlayer->SetHP(m_pPlayer->GetMaxHP());
			m_pPlayer->SetAtt(150.f);
			cheat = true;
		}
		else
		{
			m_pPlayer->SetAtt(40.f);
			cheat = false;
		}
	}

	// �¸�
	if (KEY_TAP(KEY::O))
	{
		ChangeTurn(TURN_TYPE::WIN);
	}

	// ���� ����
	if (KEY_TAP(KEY::RBTN))
	{
		Vec2 vLookAt = CCamera::GetInstance()->GetRealPos(MOUSE_POS);
		CCamera::GetInstance()->SetLookAt(vLookAt);
	}
}

void CScene_Battle::SortGroupObj()
{
	vector<CObject*>& groupObject = GetGroupObject(GROUP_TYPE::UNIT);
	sort(groupObject.begin(), groupObject.end(), CompareGridPos);
}

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

#include "CEventCenter.h"
#include "CTurnManager.h"
#include "CTileManager.h"
#include "CMonsterSpawner.h"

#include "EnterBattle.h"
#include "PlayerStart.h"
#include "PlayerTurn_TileSelect.h"
#include "PlayerTurn_Move.h"
#include "PlayerTurn_Attack.h"
#include "PlayerTurn_Skill.h"
#include "EnemyTurn_Move.h"
#include "PlayerWin.h"
#include "PlayerDefeat.h"

static std::random_device rd;
static std::mt19937 gen(rd());

static bool cheat;

CScene_Battle::CScene_Battle()
	: m_pPlayer(nullptr)
	, m_iFieldType(0)
	, m_iDifficulty(1)
	, m_FieldType(FIELD_TYPE::COMMON)
	, m_vecStates((int)TURN_TYPE::END)
	, m_pMonsterSpawner(nullptr)
	, m_pEventCenter(nullptr)
	, m_pTurnCenter(nullptr)
	, m_pTileCenter(nullptr)
	, m_pBattleState(nullptr)
	, m_pMoneyUI(nullptr)
	, m_pComboUI(nullptr)
	, m_vecSoundEffect((int)SOUND_TYPE::END)
{
	m_vecSoundEffect[(int)SOUND_TYPE::WIN] = CResMgr::GetInstance()->LoadSound(L"winSound", L"sound\\win.wav");
	m_vecSoundEffect[(int)SOUND_TYPE::LOSE] = CResMgr::GetInstance()->LoadSound(L"losdSound", L"sound\\lose.wav");
}

CScene_Battle::~CScene_Battle()
{
	SafeDeleteVec(m_vecStates);
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
	m_pTurnCenter->ChangeTurn(TURN_TYPE::ENEMY_MOVE);

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
	_pMon->SetActing(true);
	_pMon->GetAnimator()->PlayType(L"front_idle", true);

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
	m_pTurnCenter->ChangeTurn(TURN_TYPE::DEFEAT);

	printf("CScene_Battle::PlayerDied :: �÷��̾� ���\n");
}

void CScene_Battle::CameraEvent()
{
	ChangeScene(SCENE_TYPE::ROBBY);
}

void CScene_Battle::OnChangeTurn(TURN_TYPE _type)
{
	m_vecStates[(int)_type]->Init(this);
}

void CScene_Battle::OnTileSelect(CObject* _pObj)
{
	// ���� :: ���콺�� �� ���� ���¿��� Ÿ���� �ݶ��̴��� ���� ����
	// ���콺 �� ���� ���¿���, �ݶ��̴��� ������ -> �̺�Ʈ �Ŵ������� �� �Լ��� ȣ���Ŵ
	TURN_TYPE CurrnetTurn = m_pTurnCenter->GetTurnState();
	vector<vector<TileState>>& vecTiles = m_pTileCenter->GetTiles();

	// PLAYER_START, PLAYER_TILESELECT
	m_vecStates[(int)CurrnetTurn]->OnTileSelect(this, _pObj);

	// BFS �湮 �ʱ�ȭ
	m_pTileCenter->TileVisitedInit();
}

void CScene_Battle::Update()
{
	InputKey();								// Ű �Է�
	SortGroupObj();							// ���� ������Ʈ�� ������ ���� ����(��ǥ�� �������� �� ���� ������)
	CScene::Update();						// ���� �� ������Ʈ (���� ������Ʈ���� ������Ʈ)
	m_pTurnCenter->Update(this);		// �� ���� ������Ʈ

	// ���� ó�� (���� �ӽ�)
	if (m_pBattleState != nullptr) m_pBattleState->Handle(this);
	
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
	m_pTurnCenter = new CTurnCenter;
	m_pEventCenter = new CEventCenter;
	m_pMonsterSpawner = new CMonsterSpawner;
	m_pTileCenter = new CTileCenter;

	m_vecStates.resize((int)TURN_TYPE::END, nullptr);

	// ���� State���� �߰�
	m_vecStates[(int)TURN_TYPE::ENTER] = new EnterBattle;
	m_vecStates[(int)TURN_TYPE::PLAYER_START] = new PlayerStart;
	m_vecStates[(int)TURN_TYPE::PLAYER_TILESELECT] = new PlayerTurn_TileSelect;
	m_vecStates[(int)TURN_TYPE::PLAYER_MOVE] = new PlayerTurn_Move;
	m_vecStates[(int)TURN_TYPE::PLAYER_ATTACK] = new PlayerTurn_Attack;
	m_vecStates[(int)TURN_TYPE::PLAYER_SKILL] = new PlayerTurn_Skill;
	m_vecStates[(int)TURN_TYPE::ENEMY_MOVE] = new EnemyTurn_Move;
	m_vecStates[(int)TURN_TYPE::WIN] = new PlayerWin;
	m_vecStates[(int)TURN_TYPE::DEFEAT] = new PlayerDefeat;

	SetBattleState(TURN_TYPE::ENTER);
	m_pBattleState->Handle(this);
	SetBattleState(TURN_TYPE::PLAYER_START);
	m_pTurnCenter->ChangeTurn(TURN_TYPE::PLAYER_START);
}

void CScene_Battle::Exit()
{
	CCamera::GetInstance()->SetTarget(nullptr);

	m_pTurnCenter->Init();
	m_pTileCenter->Init();
	m_pMonsterSpawner->Init();
	DeleteAll();
	CCollisionMgr::GetInstance()->Reset();
	SafeDeleteVec(m_vecStates);

	delete m_pMonsterSpawner;
	delete m_pEventCenter;
	delete m_pTurnCenter;
	delete m_pTileCenter;
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
		m_pTurnCenter->ChangeTurn(TURN_TYPE::WIN);
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

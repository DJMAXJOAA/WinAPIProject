#include "pch.h"
#include "CScene_Battle.h"

#include "CCollisionMgr.h"
#include "CKeyMgr.h"
#include "CDataMgr.h"

#include "CPlayer.h"
#include "CTile.h"
#include "CMouse.h"
#include "CMonster.h"

#include "CAnimator.h"
#include "CAnimation.h"

#include "BFSSearch.h"
#include "AstarSearch.h"
#include "CTurnManager.h"
#include "CTileManager.h"
#include "CMonsterSpawner.h"

#include "EnterBattle.h"
#include "PlayerTurn_TileSelect.h"
#include "PlayerTurn_Move.h"
#include "PlayerTurn_Attack.h"
#include "PlayerTurn_Skill.h"
#include "EnemyTurn_Move.h"
#include "PlayerWin.h"
#include "PlayerDefeat.h"

#include "GameData.h"

using namespace battle;
static std::random_device rd;
static std::mt19937 gen(rd());

CScene_Battle::CScene_Battle()
	: m_pPlayer(nullptr)
	, m_iFieldType(0)
	, m_iDifficulty(1)
	, m_FieldType(FIELD_TYPE::COMMON)
	, m_vecStates((int)TURN_TYPE::END)
	, m_BFS(nullptr)
	, m_Astar(nullptr)
	, m_MonsterSpawner(nullptr)
	, m_TurnCenter(nullptr)
	, m_TileCenter(nullptr)
	, m_BattleState(nullptr)
{

}

CScene_Battle::~CScene_Battle()
{
	SafeDeleteVec(m_vecStates);
}

void CScene_Battle::TurnInit(TURN_TYPE _type)
{
	switch (_type)
	{
	case TURN_TYPE::ENTER:
	{
		printf("CScene_Battle::TurnInit :: ��Ʋ �� ���� �ʱ�ȭ\n");
		break;
	}
	case TURN_TYPE::PLAYER_START:
	{
		// ���� ����
		SetBattleState(TURN_TYPE::PLAYER_TILESELECT);

		// ī�޶� ĳ���ͷ� �ʱ�ȭ
		CCamera::GetInstance()->SetTarget(nullptr);
		CCamera::GetInstance()->SetLookAt(m_pPlayer->GetPos());

		// ����Ʈ ���� �ִ� ��� Ÿ�ϵ��� ������ ���� �������� ������
		list<Vec2>& moveRoute = m_TurnCenter->GetMoveRoute();
		m_TileCenter->TileRouteInit(moveRoute);

		// ĳ���� ���� �ʱ�ȭ
		m_pPlayer->SetState(PLAYER_STATE::IDLE);
		m_pPlayer->AnimationDirection(PLAYER_STATE::IDLE, true);
		m_pPlayer->SetAttacking(false);

		// ����Ʈ �ʱ�ȭ
		m_TurnCenter->RouteInit();
		m_TurnCenter->GetTargetList().clear();
		m_TileCenter->TileVisitedInit();

		// �޺� �ʱ�ȭ
		m_TurnCenter->SetCombo(0);

		// �÷��̾� �ִϸ��̼� ����
		m_pPlayer->SetState(PLAYER_STATE::IDLE);

		printf("CScene_Battle::TurnInit :: �÷��̾� �� ���� �ʱ�ȭ\n");
		break;
	}
	case TURN_TYPE::PLAYER_TILESELECT: break;
	case TURN_TYPE::PLAYER_MOVE:
	{
		// ���� ����
		SetBattleState(TURN_TYPE::PLAYER_MOVE);

		// ī�޶� �÷��̾�� Ÿ�� ����
		CCamera::GetInstance()->SetTarget(m_pPlayer);

		printf("CScene_Battle::TurnInit :: �÷��̾� �̵� ���� �ʱ�ȭ\n");
		break;
	}
	case TURN_TYPE::PLAYER_ATTACK:
	{
		// ���� ����
		SetBattleState(TURN_TYPE::PLAYER_ATTACK);

		printf("CScene_Battle::TurnInit :: �÷��̾� ���� ���� �ʱ�ȭ\n");
		break;
	}
	case TURN_TYPE::PLAYER_SKILL:
	{
		// ī�޶� Ÿ�� ����
		CCamera::GetInstance()->SetTarget(nullptr);

		// ���� Ÿ�ϵ�(��� �����Դ� Ÿ�ϵ�) ���� Ÿ�ϵ�� ���½�Ű��
		m_TileCenter->TileRandomInit();

		if (m_TurnCenter->GetCombo() < 4)
		{
			printf("CScene_Battle::TurnInit :: ��ų �ʱ�ȭ -> �޺��� 4 ���϶� ��ų�� ������� �ʽ��ϴ�.\n");

			m_TurnCenter->ChangeTurn(TURN_TYPE::ENEMY_MOVE);
			return;
		}

		// BFS�� ��ų ���� ��� Ž��
		vector<vector<TileState>>& vecTile = m_TileCenter->GetTiles();
		list<CObject*>& lstMonsters = m_TurnCenter->GetTargetList();
		m_BFS->BFS(m_pPlayer->GetGridPos(), vecTile, lstMonsters, DIRECTION::FOUR_WAY, 3);

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

			m_TurnCenter->ChangeTurn(TURN_TYPE::ENEMY_MOVE);
			return;
		}

		// ���� ����
		SetBattleState(TURN_TYPE::PLAYER_SKILL);

		printf("CScene_Battle::TurnInit :: �÷��̾� ��ų ���� �ʱ�ȭ\n");
		break;
	}
	case TURN_TYPE::ENEMY_MOVE:
	{
		// �÷��̾� Ÿ�� ����
		m_pPlayer->SetTarget(nullptr);

		// ���Ͱ� �� �׾�����, �� �Ѿ��
		if (m_MonsterSpawner->GetMonsterList().empty())
		{
			m_TurnCenter->ChangeTurn(TURN_TYPE::WIN);
			return;
		}

		// ���� ����
		SetBattleState(TURN_TYPE::ENEMY_MOVE);

		// �÷��̾� �ʱ�ȭ
		m_pPlayer->SetState(PLAYER_STATE::IDLE);
		m_pPlayer->AnimationDirection(PLAYER_STATE::IDLE, true);

		printf("CScene_Battle::TurnInit :: �� �̵� ���� �ʱ�ȭ\n");
		break;
	}
	case TURN_TYPE::WIN:
	{
		// ���� ����
		SetBattleState(TURN_TYPE::WIN);

		// ī�޶� ȿ�� :: 2�� ��, ���̵�ƿ� -> �ϳѱ� -> ���̵���
		CCamera::GetInstance()->WhiteScreen(1.0f);
		CCamera::GetInstance()->FadeOut(1.0f);
		CCamera::GetInstance()->Event(0.01f);
		CCamera::GetInstance()->BlackScreen(2.0f);
		CCamera::GetInstance()->FadeIn(1.0f);

		// ���� ������ ����
		GameData* data = (GameData*)CDataMgr::GetInstance()->FindData(0);
		
		data->m_PlayerInfo.fCurHP = m_pPlayer->GetHP();
		data->m_PlayerInfo.iMoney = m_pPlayer->GetMoney();

		data->SaveData();

		break;
	}
	case TURN_TYPE::DEFEAT:
	{
		// ���� ����
		SetBattleState(TURN_TYPE::DEFEAT);

		// ī�޶� ȿ�� :: 2�� ��, ���̵�ƿ� -> �ϳѱ� -> ���̵���
		CCamera::GetInstance()->WhiteScreen(1.0f);
		CCamera::GetInstance()->FadeOut(1.0f);
		CCamera::GetInstance()->Event(0.01f);
		CCamera::GetInstance()->BlackScreen(2.0f);
		CCamera::GetInstance()->FadeIn(1.0f);



		break;
	}
	}
}

void CScene_Battle::PlayerAttackMonster(float _damage, CMonster* _pMon)
{
	_pMon->GetDamaged(_damage);

	printf("CScene_Battle::PlayerAttackMonster :: ������ %1.f �������� ���� ->", _damage);
	cout << _pMon << "\n";
}

void CScene_Battle::PlayerAttackDone()
{
	list<CObject*>& list = m_TurnCenter->GetTargetList();
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
	list<CObject*>& list = m_TurnCenter->GetTargetList();
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
			((CMonster*)*iter)->GetDamaged(_value);
		}

		list.clear();
		return;
	}
}

void CScene_Battle::PlayerSkillDone()
{
	m_pPlayer->SetAttacking(false);
	m_TurnCenter->ChangeTurn(TURN_TYPE::ENEMY_MOVE);

	printf("CScene_Battle::PlayerSkillDone :: ��ų�� ����Ǿ����ϴ�.\n");
}

void CScene_Battle::MonsterAttackPlayer(float _damage)
{
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
	vector<vector<TileState>>& vecTiles = m_TileCenter->GetTiles();

	vecTiles[(int)GridPos.y][(int)GridPos.x].pObj = nullptr;

	printf("CScene_Battle::MonsterDied :: %d, %d Ÿ�� ���� ������Ʈ�� �ʱ�ȭ ���׽��ϴ�.\n", (int)GridPos.x, (int)GridPos.y);
}


void CScene_Battle::PlayerDied()
{
	m_TurnCenter->ChangeTurn(TURN_TYPE::DEFEAT);

	printf("CScene_Battle::PlayerDied :: �÷��̾� ���\n");
}

void CScene_Battle::CameraEvent()
{
	ChangeScene(SCENE_TYPE::ROBBY);
}

void CScene_Battle::TileSelectTrigger(CObject* _pObj)
{
	// ���� :: ���콺�� �� ���� ���¿��� Ÿ���� �ݶ��̴��� ���� ����
	// ���콺 �� ���� ���¿���, �ݶ��̴��� ������ -> �̺�Ʈ �Ŵ������� �� �Լ��� ȣ���Ŵ
	TURN_TYPE CurrnetTurn = m_TurnCenter->GetTurnState();
	vector<vector<TileState>>& vecTiles = m_TileCenter->GetTiles();

	switch (CurrnetTurn)
	{
	case TURN_TYPE::PLAYER_START:
	{
		Vec2 vPlayerPos = m_pPlayer->GetGridPos();
		Vec2 selectPos = _pObj->GetGridPos();

		// BFS�� 8���� Ž�� (�ֺ� 1ĭ), ������Ʈ�� �����ϸ�, 
		m_BFS->BFS(vPlayerPos, vecTiles, DIRECTION::EIGHT_WAY, 1);

		// BFS Ž�����, �湮 ���� + ���� ������Ʈ ����
		if (m_TileCenter->IsVisited(selectPos) && m_TileCenter->GetObj(selectPos) == nullptr)
		{
			// Ÿ�� ���� ����, ���� ��ġ ����, ����Ʈ �߰�, Ÿ�� ���õƴٰ� �Լ� �����ֱ�
			CTile* tile = (CTile*)_pObj;

			// Ÿ�� ���� ����, ���� ��ġ ����, ����Ʈ �߰�, Ÿ�� ���õƴٰ� �Լ� �����ֱ�, �� ��ȯ
			m_TurnCenter->SetTileColor(tile->GetTileState());
			m_TurnCenter->SetSelectTile(selectPos);
			m_TurnCenter->GetMoveRoute().push_back(selectPos);
			tile->SetTileState((TILE_STATE)((int)tile->GetTileState() + 4));

			// ī�޶� Ÿ�Ϸ� ����
			CCamera::GetInstance()->SetLookAt(REAL(selectPos));

			// �� ����
			m_TurnCenter->ChangeTurn(TURN_TYPE::PLAYER_TILESELECT);

			// Ÿ�� �޺� �߰�
			m_TurnCenter->SetCombo(1);

			// �����
			DEBUG2(selectPos.x, selectPos.y);
			printf("CScene_Battle::TileSelectTrigger :: Ÿ�� �׸��� ����\n");
		}
	}
		break;
	case TURN_TYPE::PLAYER_TILESELECT:
	{
		Vec2 selectPos = _pObj->GetGridPos();
		Vec2 currentPos = m_TurnCenter->GetSelectTile();
		m_BFS->BFS(currentPos, vecTiles, DIRECTION::EIGHT_WAY, 1);

		// �ߺ��� ��ġ�� ����Ʈ�� ���� ���ϰ� ����
		list<Vec2>& moveRoute = m_TurnCenter->GetMoveRoute();
		auto iter = std::find(moveRoute.begin(), moveRoute.end(), selectPos);

		if (m_TileCenter->IsVisited(selectPos) &&
			/*m_TileCenter->GetTile(selectPos)->GetTileState() == m_TurnCenter->GetTileColor() &&*/
			m_TileCenter->GetObj(selectPos) == nullptr &&
			iter == moveRoute.end())
		{
			CTile* tile = (CTile*)_pObj;
			
			// ī�޶� Ÿ�Ϸ� ����
			CCamera::GetInstance()->SetLookAt(REAL(selectPos));

			// ���� ��ġ ����, ����Ʈ �߰�, Ÿ�� ���õƴٰ� �Լ� �����ֱ�
			m_TurnCenter->SetSelectTile(selectPos);
			moveRoute.push_back(selectPos);
			tile->SetTileState((TILE_STATE)((int)tile->GetTileState() + 4));

			// Ÿ�� �޺� �߰�
			m_TurnCenter->SetCombo(m_TurnCenter->GetCombo() + 1);

			// �����
			printf("%d �޺� :: ", m_TurnCenter->GetCombo());
			for (auto& lstPos : moveRoute) printf("%1.f, %1.f -> ", lstPos.x, lstPos.y);
			printf("\n");
		}
	}
		break;
	default:
		break;
	}

	// BFS �湮 �ʱ�ȭ
	m_TileCenter->TileVisitedInit();
}

void CScene_Battle::Update()
{
	// ȭ�� ��ȯ
	if (KEY_TAP(KEY::ENTER))
	{
		ChangeScene(SCENE_TYPE::ROBBY);
	}

	// ���� ����
	if (KEY_TAP(KEY::RBTN))
	{
		Vec2 vLookAt = CCamera::GetInstance()->GetRealPos(MOUSE_POS);
		CCamera::GetInstance()->SetLookAt(vLookAt);
	}

	// ���� ������Ʈ�� ������ ���� ����(��ǥ�� �������� �� ���� ������)
	vector<CObject*>& groupObject = GetGroupObject(GROUP_TYPE::UNIT);
	sort(groupObject.begin(), groupObject.end(), CompareGridPos);

	// ���� �� ������Ʈ (���� ������Ʈ���� ������Ʈ)
	CScene::Update();

	// �� ���� ������Ʈ
	m_TurnCenter->Update(this);

	// ���� ó�� (���� �ӽ�)
	if (m_BattleState != nullptr)
	{
		m_BattleState->Handle(this);
	}
	
	// ���� ���� �迭 ������Ʈ (��� ������ ������Ʈ���� ����)
	m_MonsterSpawner->Update();
}

void CScene_Battle::Enter()
{
	// �Ŵ����� ��� -> ������Ʈ�� ����ϴ°� �ƴ϶� ���� ���� �����־�� �Ѵ�. Enter, Exit �Լ��� ���þ���
	// ���� ���� �ִ� �׷� ������Ʈ���� �����ؼ� �����ϴ� �����̶�, ���Ƿ� ������ ������Ű�� �ʰ� �����ؾ� �Ѵ�
	m_TurnCenter = new CTurnCenter;
	m_BFS = new BFSSearch;
	m_MonsterSpawner = new CMonsterSpawner;
	m_TileCenter = new CTileCenter;
	m_Astar = new AstarSearch;

	m_vecStates.resize((int)TURN_TYPE::END, nullptr);

	// ���� State���� �߰�
	m_vecStates[(int)TURN_TYPE::ENTER] = new EnterBattle;
	m_vecStates[(int)TURN_TYPE::PLAYER_TILESELECT] = new PlayerTurn_TileSelect;
	m_vecStates[(int)TURN_TYPE::PLAYER_MOVE] = new PlayerTurn_Move;
	m_vecStates[(int)TURN_TYPE::PLAYER_ATTACK] = new PlayerTurn_Attack;
	m_vecStates[(int)TURN_TYPE::PLAYER_SKILL] = new PlayerTurn_Skill;
	m_vecStates[(int)TURN_TYPE::ENEMY_MOVE] = new EnemyTurn_Move;
	m_vecStates[(int)TURN_TYPE::WIN] = new PlayerWin;
	m_vecStates[(int)TURN_TYPE::DEFEAT] = new PlayerDefeat;

	SetBattleState(TURN_TYPE::ENTER);
	m_BattleState->Handle(this);
	SetBattleState(TURN_TYPE::PLAYER_START);
	m_TurnCenter->ChangeTurn(TURN_TYPE::PLAYER_START);
}

void CScene_Battle::Exit()
{
	CCamera::GetInstance()->SetTarget(nullptr);

	m_TurnCenter->Init();
	m_TileCenter->Init();
	m_MonsterSpawner->Init();
	DeleteAll();
	CCollisionMgr::GetInstance()->Reset();
	SafeDeleteVec(m_vecStates);

	delete m_BFS;
	delete m_MonsterSpawner;
	delete m_TurnCenter;
	delete m_TileCenter;
	delete m_Astar;
}

bool CompareGridPos(CObject* _pObj1, CObject* _pObj2)
{
	Vec2 pos1 = _pObj1->GetGridPos();
	Vec2 pos2 = _pObj2->GetGridPos();

	// x ��ǥ �켱 ��
	if (pos1.x < pos2.x) return true;
	if (pos1.x > pos2.x) return false;

	// x ������ y ��
	return pos1.y < pos2.y;
}
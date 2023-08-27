#include "pch.h"
#include "CScene_Battle.h"

#include "CCollisionMgr.h"
#include "CKeyMgr.h"

#include "CPlayer.h"
#include "CTile.h"
#include "CMouse.h"
#include "CMonster.h"

#include "CAnimator.h"
#include "CAnimation.h"

#include "BFSSearch.h"
#include "CTurnManager.h"
#include "CTileManager.h"
#include "CMonsterSpawner.h"

#include "EnterBattle.h"
#include "PlayerTurn_TileSelect.h"
#include "PlayerTurn_Move.h"
#include "PlayerTurn_Attack.h"

using namespace battle;
static std::random_device rd;
static std::mt19937 gen(rd());

CScene_Battle::CScene_Battle()
	: m_pPlayer(nullptr)
	, m_iFieldType(0)
	, m_iDifficulty(1)
	, m_FieldType(FIELD_TYPE::COMMON)
	, m_vecStates((int)TURN_TYPE::EXIT)
	, m_BFS(nullptr)
	, m_MonsterSpawner(nullptr)
	, m_TurnCenter(nullptr)
	, m_TileCenter(nullptr)
	, m_BattleState(nullptr)
{
	// �Ŵ����� ��� -> ������Ʈ�� ����ϴ°� �ƴ϶� ���� ���� �����־�� �Ѵ�. Enter, Exit �Լ��� ���þ���
	// ���� ���� �ִ� �׷� ������Ʈ���� �����ؼ� �����ϴ� �����̶�, ���Ƿ� ������ ������Ű�� �ʰ� �����ؾ� �Ѵ�
	m_TurnCenter = new CTurnCenter;
	m_BFS = new BFSSearch;
	m_MonsterSpawner = new CMonsterSpawner;
	m_TileCenter = new CTileCenter;

	m_vecStates[(int)TURN_TYPE::ENTER] = new EnterBattle;
	m_vecStates[(int)TURN_TYPE::PLAYER_TILESELECT] = new PlayerTurn_TileSelect;
	m_vecStates[(int)TURN_TYPE::PLAYER_MOVE] = new PlayerTurn_Move;
	m_vecStates[(int)TURN_TYPE::PLAYER_ATTACK] = new PlayerTurn_Attack;
}

CScene_Battle::~CScene_Battle()
{
	delete m_BFS;
	delete m_MonsterSpawner;
	delete m_TurnCenter;
	delete m_TileCenter;

	SafeDeleteVec(m_vecStates);
}

void CScene_Battle::TurnInit(TURN_TYPE _type)
{
	switch (_type)
	{
	case TURN_TYPE::ENTER:
	{
		// �����
		printf("��Ʋ �� ���� �ʱ�ȭ\n");
		break;
	}
	case TURN_TYPE::PLAYER_START:
	{
		// ī�޶� ĳ���ͷ� �ʱ�ȭ
		CCamera::GetInstance()->SetTarget(nullptr);
		CCamera::GetInstance()->SetLookAt(m_pPlayer->GetPos());

		// ����Ʈ ���� �ִ� ��� Ÿ�ϵ��� ������ ���� �������� ������
		list<Vec2>& moveRoute = m_TurnCenter->GetMoveRoute();
		m_TileCenter->TileRouteInit(moveRoute);

		// ĳ���� �ִϸ��̼� �ʱ�ȭ
		m_pPlayer->GetAnimator()->PlayType(L"front_idle", true);

		// ����Ʈ �ʱ�ȭ
		m_TurnCenter->RouteInit();

		// ���� ����
		SetBattleState(TURN_TYPE::PLAYER_TILESELECT);

		// �÷��̾� �ִϸ��̼� ����
		m_pPlayer->SetState(PLAYER_STATE::IDLE);

		// �����
		printf("�÷��̾� �� ���� �ʱ�ȭ\n");
		break;
	}
	case TURN_TYPE::PLAYER_TILESELECT: break;
	case TURN_TYPE::PLAYER_MOVE:
	{
		// ī�޶� �÷��̾�� Ÿ�� ����
		CCamera::GetInstance()->SetTarget(m_pPlayer);

		// ���� ����
		SetBattleState(TURN_TYPE::PLAYER_MOVE);

		// �����
		printf("�÷��̾� �̵� ���� �ʱ�ȭ\n");
		break;
	}
	case TURN_TYPE::PLAYER_ATTACK:
	{
		// ���� ����
		SetBattleState(TURN_TYPE::PLAYER_ATTACK);

		// �����
		printf("�÷��̾� ���� ���� �ʱ�ȭ\n");
		break;
	}
	case TURN_TYPE::PLAYER_SKILL:
	{
		// ī�޶� Ÿ�� ����
		CCamera::GetInstance()->SetTarget(nullptr);

		// ���� Ÿ�ϵ�(��� �����Դ� Ÿ�ϵ�) ���� Ÿ�ϵ�� ���½�Ű��
		m_TileCenter->TileRandomInit();

		// �����
		printf("�÷��̾� ��ų ���� �ʱ�ȭ\n");
		break;
	}
	case TURN_TYPE::ENEMY_MOVE: break;
	case TURN_TYPE::ENEMY_ATTACK: break;
	case TURN_TYPE::EXIT: break;
	}
}

void CScene_Battle::PlayerAttackMonster(float _damage, CMonster* _pMon)
{
	_pMon->GetDamaged(_damage);

	printf("������ %1.f �������� �ִ� �̺�Ʈ\n", _damage);
}

void CScene_Battle::PlayerAttackDone()
{
	list<CObject*>& list = m_TurnCenter->GetTargetList();
	if (list.empty())
	{
		printf("����Ʈ�� ������ϴ�.\n");
		return;
	}
	else
	{
		printf("Ÿ�� ����Ʈ�� �ϳ� �����մϴ�.\n");
		list.pop_back();
		return;
	}

}

void CScene_Battle::MonsterDied(CMonster* _pObj)
{
	Vec2 GridPos = _pObj->GetGridPos();
	vector<vector<TileState>>& vecTiles = m_TileCenter->GetTiles();

	vecTiles[(int)GridPos.y][(int)GridPos.x].pObj = nullptr;

	printf("%d, %d Ÿ�� ���� ������Ʈ�� �ʱ�ȭ ���׽��ϴ�.\n", (int)GridPos.x, (int)GridPos.y);
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

			// �����
			DEBUG2(selectPos.x, selectPos.y);
			printf("Ÿ�� �׸��� ����\n");
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

			// �����
			for (auto& lstPos : moveRoute) printf("%1.f, %1.f -> ", lstPos.x, lstPos.y);
			printf("\n");
		}
	}
		break;
	default:
		break;
	}

	// BFS �湮 �ʱ�ȭ
	m_BFS->BFS_Init(vecTiles);
}

void CScene_Battle::Update()
{
	// ȭ�� ��ȯ
	if (KEY_TAP(KEY::ENTER))
	{
		ChangeScene(SCENE_TYPE::TOOL);
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
}

void CScene_Battle::Enter()
{
	// ��ư �̺�Ʈ�� ��ü��Ű��
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
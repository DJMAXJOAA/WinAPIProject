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
	// 매니저를 등록 -> 오브젝트로 등록하는게 아니라서 직접 해제 시켜주어야 한다. Enter, Exit 함수와 관련없음
	// 메인 씬에 있는 그룹 오브젝트들을 참조해서 관리하는 형식이라, 임의로 값들을 해제시키지 않게 주의해야 한다
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
		// 디버깅
		printf("배틀 씬 시작 초기화\n");
		break;
	}
	case TURN_TYPE::PLAYER_START:
	{
		// 카메라 캐릭터로 초기화
		CCamera::GetInstance()->SetTarget(nullptr);
		CCamera::GetInstance()->SetLookAt(m_pPlayer->GetPos());

		// 리스트 내에 있는 모든 타일들의 색깔을 원래 색상으로 돌리기
		list<Vec2>& moveRoute = m_TurnCenter->GetMoveRoute();
		m_TileCenter->TileRouteInit(moveRoute);

		// 캐릭터 애니메이션 초기화
		m_pPlayer->GetAnimator()->PlayType(L"front_idle", true);

		// 리스트 초기화
		m_TurnCenter->RouteInit();

		// 상태 변경
		SetBattleState(TURN_TYPE::PLAYER_TILESELECT);

		// 플레이어 애니메이션 설정
		m_pPlayer->SetState(PLAYER_STATE::IDLE);

		// 디버깅
		printf("플레이어 턴 시작 초기화\n");
		break;
	}
	case TURN_TYPE::PLAYER_TILESELECT: break;
	case TURN_TYPE::PLAYER_MOVE:
	{
		// 카메라 플레이어로 타겟 변경
		CCamera::GetInstance()->SetTarget(m_pPlayer);

		// 상태 변경
		SetBattleState(TURN_TYPE::PLAYER_MOVE);

		// 디버깅
		printf("플레이어 이동 시작 초기화\n");
		break;
	}
	case TURN_TYPE::PLAYER_ATTACK:
	{
		// 상태 변경
		SetBattleState(TURN_TYPE::PLAYER_ATTACK);

		// 디버깅
		printf("플레이어 공격 상태 초기화\n");
		break;
	}
	case TURN_TYPE::PLAYER_SKILL:
	{
		// 카메라 타겟 해제
		CCamera::GetInstance()->SetTarget(nullptr);

		// 검은 타일들(밟고 지나왔던 타일들) 랜덤 타일들로 리셋시키기
		m_TileCenter->TileRandomInit();

		// 디버깅
		printf("플레이어 스킬 상태 초기화\n");
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

	printf("적에게 %1.f 데미지를 주는 이벤트\n", _damage);
}

void CScene_Battle::PlayerAttackDone()
{
	list<CObject*>& list = m_TurnCenter->GetTargetList();
	if (list.empty())
	{
		printf("리스트가 비었습니다.\n");
		return;
	}
	else
	{
		printf("타겟 리스트를 하나 삭제합니다.\n");
		list.pop_back();
		return;
	}

}

void CScene_Battle::MonsterDied(CMonster* _pObj)
{
	Vec2 GridPos = _pObj->GetGridPos();
	vector<vector<TileState>>& vecTiles = m_TileCenter->GetTiles();

	vecTiles[(int)GridPos.y][(int)GridPos.x].pObj = nullptr;

	printf("%d, %d 타일 위의 오브젝트를 초기화 시켰습니다.\n", (int)GridPos.x, (int)GridPos.y);
}

void CScene_Battle::TileSelectTrigger(CObject* _pObj)
{
	// 조건 :: 마우스를 꾹 누른 상태에서 타일의 콜라이더와 닿은 상태
	// 마우스 꾹 눌린 상태에서, 콜라이더가 닿으면 -> 이벤트 매니저에서 이 함수를 호출시킴
	TURN_TYPE CurrnetTurn = m_TurnCenter->GetTurnState();
	vector<vector<TileState>>& vecTiles = m_TileCenter->GetTiles();

	switch (CurrnetTurn)
	{
	case TURN_TYPE::PLAYER_START:
	{
		Vec2 vPlayerPos = m_pPlayer->GetGridPos();
		Vec2 selectPos = _pObj->GetGridPos();

		// BFS로 8방향 탐색 (주변 1칸), 오브젝트가 존재하면, 
		m_BFS->BFS(vPlayerPos, vecTiles, DIRECTION::EIGHT_WAY, 1);

		// BFS 탐색결과, 방문 했음 + 위에 오브젝트 없음
		if (m_TileCenter->IsVisited(selectPos) && m_TileCenter->GetObj(selectPos) == nullptr)
		{
			// 타일 색깔 지정, 현재 위치 갱신, 리스트 추가, 타일 선택됐다고 함수 날려주기
			CTile* tile = (CTile*)_pObj;

			// 타일 색깔 지정, 현재 위치 갱신, 리스트 추가, 타일 선택됐다고 함수 날려주기, 턴 전환
			m_TurnCenter->SetTileColor(tile->GetTileState());
			m_TurnCenter->SetSelectTile(selectPos);
			m_TurnCenter->GetMoveRoute().push_back(selectPos);
			tile->SetTileState((TILE_STATE)((int)tile->GetTileState() + 4));

			// 카메라 타일로 지정
			CCamera::GetInstance()->SetLookAt(REAL(selectPos));

			// 턴 변경
			m_TurnCenter->ChangeTurn(TURN_TYPE::PLAYER_TILESELECT);

			// 디버깅
			DEBUG2(selectPos.x, selectPos.y);
			printf("타일 그리기 시작\n");
		}
	}
		break;
	case TURN_TYPE::PLAYER_TILESELECT:
	{
		Vec2 selectPos = _pObj->GetGridPos();
		Vec2 currentPos = m_TurnCenter->GetSelectTile();
		m_BFS->BFS(currentPos, vecTiles, DIRECTION::EIGHT_WAY, 1);

		// 중복된 위치는 리스트에 들어가지 못하게 설정
		list<Vec2>& moveRoute = m_TurnCenter->GetMoveRoute();
		auto iter = std::find(moveRoute.begin(), moveRoute.end(), selectPos);
		if (m_TileCenter->IsVisited(selectPos) &&
			/*m_TileCenter->GetTile(selectPos)->GetTileState() == m_TurnCenter->GetTileColor() &&*/
			m_TileCenter->GetObj(selectPos) == nullptr &&
			iter == moveRoute.end())
		{
			CTile* tile = (CTile*)_pObj;
			
			// 카메라 타일로 지정
			CCamera::GetInstance()->SetLookAt(REAL(selectPos));

			// 현재 위치 갱신, 리스트 추가, 타일 선택됐다고 함수 날려주기
			m_TurnCenter->SetSelectTile(selectPos);
			moveRoute.push_back(selectPos);
			tile->SetTileState((TILE_STATE)((int)tile->GetTileState() + 4));

			// 디버깅
			for (auto& lstPos : moveRoute) printf("%1.f, %1.f -> ", lstPos.x, lstPos.y);
			printf("\n");
		}
	}
		break;
	default:
		break;
	}

	// BFS 방문 초기화
	m_BFS->BFS_Init(vecTiles);
}

void CScene_Battle::Update()
{
	// 화면 전환
	if (KEY_TAP(KEY::ENTER))
	{
		ChangeScene(SCENE_TYPE::TOOL);
	}

	// 시점 조절
	if (KEY_TAP(KEY::RBTN))
	{
		Vec2 vLookAt = CCamera::GetInstance()->GetRealPos(MOUSE_POS);
		CCamera::GetInstance()->SetLookAt(vLookAt);
	}

	// 유닛 오브젝트의 렌더링 순서 조절(좌표가 낮을수록 더 낮은 순서로)
	vector<CObject*>& groupObject = GetGroupObject(GROUP_TYPE::UNIT);
	sort(groupObject.begin(), groupObject.end(), CompareGridPos);

	// 메인 씬 업데이트 (각자 오브젝트들의 업데이트)
	CScene::Update();

	// 턴 결정 업데이트
	m_TurnCenter->Update(this);

	// 로직 처리 (상태 머신)
	if (m_BattleState != nullptr)
	{
		m_BattleState->Handle(this);
	}
}

void CScene_Battle::Enter()
{
	// 버튼 이벤트로 대체시키기
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

	// x 좌표 우선 비교
	if (pos1.x < pos2.x) return true;
	if (pos1.x > pos2.x) return false;

	// x 같으면 y 비교
	return pos1.y < pos2.y;
}
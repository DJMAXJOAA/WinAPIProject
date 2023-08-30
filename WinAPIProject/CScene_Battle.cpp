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
		printf("CScene_Battle::TurnInit :: 배틀 씬 시작 초기화\n");
		break;
	}
	case TURN_TYPE::PLAYER_START:
	{
		// 상태 변경
		SetBattleState(TURN_TYPE::PLAYER_TILESELECT);

		// 카메라 캐릭터로 초기화
		CCamera::GetInstance()->SetTarget(nullptr);
		CCamera::GetInstance()->SetLookAt(m_pPlayer->GetPos());

		// 리스트 내에 있는 모든 타일들의 색깔을 원래 색상으로 돌리기
		list<Vec2>& moveRoute = m_TurnCenter->GetMoveRoute();
		m_TileCenter->TileRouteInit(moveRoute);

		// 캐릭터 상태 초기화
		m_pPlayer->SetState(PLAYER_STATE::IDLE);
		m_pPlayer->AnimationDirection(PLAYER_STATE::IDLE, true);
		m_pPlayer->SetAttacking(false);

		// 리스트 초기화
		m_TurnCenter->RouteInit();
		m_TurnCenter->GetTargetList().clear();
		m_TileCenter->TileVisitedInit();

		// 콤보 초기화
		m_TurnCenter->SetCombo(0);

		// 플레이어 애니메이션 설정
		m_pPlayer->SetState(PLAYER_STATE::IDLE);

		printf("CScene_Battle::TurnInit :: 플레이어 턴 시작 초기화\n");
		break;
	}
	case TURN_TYPE::PLAYER_TILESELECT: break;
	case TURN_TYPE::PLAYER_MOVE:
	{
		// 상태 변경
		SetBattleState(TURN_TYPE::PLAYER_MOVE);

		// 카메라 플레이어로 타겟 변경
		CCamera::GetInstance()->SetTarget(m_pPlayer);

		printf("CScene_Battle::TurnInit :: 플레이어 이동 시작 초기화\n");
		break;
	}
	case TURN_TYPE::PLAYER_ATTACK:
	{
		// 상태 변경
		SetBattleState(TURN_TYPE::PLAYER_ATTACK);

		printf("CScene_Battle::TurnInit :: 플레이어 공격 상태 초기화\n");
		break;
	}
	case TURN_TYPE::PLAYER_SKILL:
	{
		// 카메라 타겟 해제
		CCamera::GetInstance()->SetTarget(nullptr);

		// 검은 타일들(밟고 지나왔던 타일들) 랜덤 타일들로 리셋시키기
		m_TileCenter->TileRandomInit();

		if (m_TurnCenter->GetCombo() < 4)
		{
			printf("CScene_Battle::TurnInit :: 스킬 초기화 -> 콤보가 4 이하라서 스킬을 사용하지 않습니다.\n");

			m_TurnCenter->ChangeTurn(TURN_TYPE::ENEMY_MOVE);
			return;
		}

		// BFS로 스킬 적중 대상 탐색
		vector<vector<TileState>>& vecTile = m_TileCenter->GetTiles();
		list<CObject*>& lstMonsters = m_TurnCenter->GetTargetList();
		m_BFS->BFS(m_pPlayer->GetGridPos(), vecTile, lstMonsters, DIRECTION::FOUR_WAY, 3);

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

			m_TurnCenter->ChangeTurn(TURN_TYPE::ENEMY_MOVE);
			return;
		}

		// 상태 변경
		SetBattleState(TURN_TYPE::PLAYER_SKILL);

		printf("CScene_Battle::TurnInit :: 플레이어 스킬 상태 초기화\n");
		break;
	}
	case TURN_TYPE::ENEMY_MOVE:
	{
		// 플레이어 타겟 삭제
		m_pPlayer->SetTarget(nullptr);

		// 몬스터가 다 죽었으면, 턴 넘어가기
		if (m_MonsterSpawner->GetMonsterList().empty())
		{
			m_TurnCenter->ChangeTurn(TURN_TYPE::WIN);
			return;
		}

		// 상태 변경
		SetBattleState(TURN_TYPE::ENEMY_MOVE);

		// 플레이어 초기화
		m_pPlayer->SetState(PLAYER_STATE::IDLE);
		m_pPlayer->AnimationDirection(PLAYER_STATE::IDLE, true);

		printf("CScene_Battle::TurnInit :: 적 이동 상태 초기화\n");
		break;
	}
	case TURN_TYPE::WIN:
	{
		// 상태 변경
		SetBattleState(TURN_TYPE::WIN);

		// 카메라 효과 :: 2초 후, 페이드아웃 -> 턴넘김 -> 페이드인
		CCamera::GetInstance()->WhiteScreen(1.0f);
		CCamera::GetInstance()->FadeOut(1.0f);
		CCamera::GetInstance()->Event(0.01f);
		CCamera::GetInstance()->BlackScreen(2.0f);
		CCamera::GetInstance()->FadeIn(1.0f);

		// 게임 데이터 저장
		GameData* data = (GameData*)CDataMgr::GetInstance()->FindData(0);
		
		data->m_PlayerInfo.fCurHP = m_pPlayer->GetHP();
		data->m_PlayerInfo.iMoney = m_pPlayer->GetMoney();

		data->SaveData();

		break;
	}
	case TURN_TYPE::DEFEAT:
	{
		// 상태 변경
		SetBattleState(TURN_TYPE::DEFEAT);

		// 카메라 효과 :: 2초 후, 페이드아웃 -> 턴넘김 -> 페이드인
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

	printf("CScene_Battle::PlayerAttackMonster :: 적에게 %1.f 데미지로 공격 ->", _damage);
	cout << _pMon << "\n";
}

void CScene_Battle::PlayerAttackDone()
{
	list<CObject*>& list = m_TurnCenter->GetTargetList();
	m_pPlayer->SetAttacking(false);

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

void CScene_Battle::PlayerSkillCasted(float _value)
{
	list<CObject*>& list = m_TurnCenter->GetTargetList();
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

	printf("CScene_Battle::PlayerSkillDone :: 스킬이 종료되었습니다.\n");
}

void CScene_Battle::MonsterAttackPlayer(float _damage)
{
	m_pPlayer->GetDamaged(_damage);

	printf("CScene_Battle::MonsterAttackPlayer :: 몬스터가 플레이어에게 %1.f 데미지로 공격 ->", _damage);
	cout << m_pPlayer << "\n";
}

void CScene_Battle::MonsterAttackDone(CMonster* _pMon)
{
	_pMon->SetActing(true);
	_pMon->GetAnimator()->PlayType(L"front_idle", true);

	printf("CScene_Battle::MonsterAttackDone :: 공격이 종료되었습니다.\n");
}

void CScene_Battle::MonsterDied(CMonster* _pObj)
{
	int money = _pObj->GetMoney();
	m_pPlayer->SetMoney(m_pPlayer->GetMoney() + money);
	printf("CScene_Battle::MonsterDied :: %d원을 얻어서 돈이 %d원이 됐습니다.\n", money, m_pPlayer->GetMoney());

	Vec2 GridPos = _pObj->GetGridPos();
	vector<vector<TileState>>& vecTiles = m_TileCenter->GetTiles();

	vecTiles[(int)GridPos.y][(int)GridPos.x].pObj = nullptr;

	printf("CScene_Battle::MonsterDied :: %d, %d 타일 위의 오브젝트를 초기화 시켰습니다.\n", (int)GridPos.x, (int)GridPos.y);
}


void CScene_Battle::PlayerDied()
{
	m_TurnCenter->ChangeTurn(TURN_TYPE::DEFEAT);

	printf("CScene_Battle::PlayerDied :: 플레이어 사망\n");
}

void CScene_Battle::CameraEvent()
{
	ChangeScene(SCENE_TYPE::ROBBY);
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

			// 타일 콤보 추가
			m_TurnCenter->SetCombo(1);

			// 디버깅
			DEBUG2(selectPos.x, selectPos.y);
			printf("CScene_Battle::TileSelectTrigger :: 타일 그리기 시작\n");
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

			// 타일 콤보 추가
			m_TurnCenter->SetCombo(m_TurnCenter->GetCombo() + 1);

			// 디버깅
			printf("%d 콤보 :: ", m_TurnCenter->GetCombo());
			for (auto& lstPos : moveRoute) printf("%1.f, %1.f -> ", lstPos.x, lstPos.y);
			printf("\n");
		}
	}
		break;
	default:
		break;
	}

	// BFS 방문 초기화
	m_TileCenter->TileVisitedInit();
}

void CScene_Battle::Update()
{
	// 화면 전환
	if (KEY_TAP(KEY::ENTER))
	{
		ChangeScene(SCENE_TYPE::ROBBY);
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
	
	// 몬스터 관리 배열 업데이트 (사망 예정인 오브젝트들을 삭제)
	m_MonsterSpawner->Update();
}

void CScene_Battle::Enter()
{
	// 매니저를 등록 -> 오브젝트로 등록하는게 아니라서 직접 해제 시켜주어야 한다. Enter, Exit 함수와 관련없음
	// 메인 씬에 있는 그룹 오브젝트들을 참조해서 관리하는 형식이라, 임의로 값들을 해제시키지 않게 주의해야 한다
	m_TurnCenter = new CTurnCenter;
	m_BFS = new BFSSearch;
	m_MonsterSpawner = new CMonsterSpawner;
	m_TileCenter = new CTileCenter;
	m_Astar = new AstarSearch;

	m_vecStates.resize((int)TURN_TYPE::END, nullptr);

	// 씬의 State들을 추가
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

	// x 좌표 우선 비교
	if (pos1.x < pos2.x) return true;
	if (pos1.x > pos2.x) return false;

	// x 같으면 y 비교
	return pos1.y < pos2.y;
}
#include "pch.h"
#include "CScene_Battle.h"

#include "CCore.h"
#include "CCollisionMgr.h"
#include "CKeyMgr.h"
#include "CDataMgr.h"

#include "CPlayer.h"
#include "CTile.h"
#include "CBlock.h"
#include "CMouse.h"
#include "CEffect.h"
#include "CMonster.h"

#include "BFSSearch.h"
#include "CTurnManager.h"
#include "CTileManager.h"
#include "CMonsterSpawner.h"

#include "CData.h"
#include "FieldData.h"

using namespace battle;

CScene_Battle::CScene_Battle()
	: m_pPlayer(nullptr)
	, m_BFS(nullptr)
	, m_MonsterSpawner(nullptr)
	, m_TurnCenter(nullptr)
	, m_TileCenter(nullptr)
{
	// 매니저를 등록 -> 오브젝트로 등록하는게 아니라서 직접 해제 시켜주어야 한다. Enter, Exit 함수와 관련없음
	// 메인 씬에 있는 그룹 오브젝트들을 참조해서 관리하는 형식이라, 임의로 값들을 해제시키지 않게 주의해야 한다
	m_TurnCenter = new CTurnCenter;
	m_BFS = new BFSSearch;
	m_MonsterSpawner = new CMonsterSpawner;
	m_TileCenter = new CTileCenter;
}

CScene_Battle::~CScene_Battle()
{
	delete m_BFS;
	delete m_MonsterSpawner;
	delete m_TurnCenter;
	delete m_TileCenter;
}

void CScene_Battle::TurnInit(TURN_TYPE _type)
{
	switch (_type)
	{
	case TURN_TYPE::ENTER: break;
	case TURN_TYPE::PLAYER_START:
	{
		// 카메라 캐릭터로 초기화
		CCamera::GetInstance()->SetTarget(nullptr);
		CCamera::GetInstance()->SetLookAt(m_pPlayer->GetPos());

		// 리스트 내에 있는 모든 타일들의 색깔을 원래 색상으로 돌리기
		list<Vec2>& moveRoute = m_TurnCenter->GetMoveRoute();
		m_TileCenter->TileInit(moveRoute);

		// 리스트 초기화
		m_TurnCenter->RouteInit();
		break;
	}
	case TURN_TYPE::PLAYER_TILESELECT: break;
	case TURN_TYPE::PLAYER_MOVE:
	{
		CCamera::GetInstance()->SetTarget(m_pPlayer);

		// 플레이어 애니메이션 설정
		m_pPlayer->SetState(PLAYER_STATE::MOVE);
		break;
	}
	case TURN_TYPE::PLAYER_ATTACK:
	{
		m_pPlayer->SetState(PLAYER_STATE::SWORD);
		break;
	}
	case TURN_TYPE::PLAYER_SKILL:
	{
		// 검은 타일들(밟고 지나왔던 타일들) 랜덤 타일들로 리셋시키기
		CCamera::GetInstance()->SetTarget(nullptr);
		m_TileCenter->TileRandomInit();

		// 플레이어 애니메이션 설정
		m_pPlayer->SetState(PLAYER_STATE::ROLLING);
		break;
	}
	case TURN_TYPE::ENEMY_MOVE: break;
	case TURN_TYPE::ENEMY_ATTACK: break;
	case TURN_TYPE::EXIT: break;
	}
}

void CScene_Battle::InitField(int _level, FIELD_TYPE _type)
{
	Vec2 vResolution = CCore::GetInstance()->GetResolution();

	// 랜덤하게 필드 ID 결정
	// 600번 대의 필드 데이터 중에 조건에 맞는 데이터들
	vector<CData*> allData = CDataMgr::GetInstance()->LoadAllData(600);
	vector<FieldData*> fieldData{};
	for (int i = 0; i < allData.size(); i++)
	{
		if (((FieldData*)allData[i])->m_iDifficulty == _level
		&& ((FieldData*)allData[i])->m_MapType == _type)
			fieldData.push_back((FieldData*)allData[i]);
	}
	// 추려낸 데이터들중에 랜덤으로 1개 선택
	if (!fieldData.empty()) 
	{
		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_int_distribution<> distr(0, fieldData.size() - 1);

		int randomIndex = distr(gen);
		FieldData* randomFieldData = fieldData[randomIndex];
		m_iFieldType = randomFieldData->GetKey();
	}
	else
	{
		assert(0);	// 데이터 가져오기 실패 디버깅
	}

	// 마우스 추가
	CMouse* pMouse = new CMouse;
	AddObject(pMouse, GROUP_TYPE::MOUSE);

	// 타일 + 블럭 추가
	int startX = (int)(vResolution.x / 2);
	int startY = (int)(vResolution.y / 4);
	map<Vec2, Vec2>& mapRealPoint = m_TileCenter->GetMapRealPoint();
	map<Vec2, Vec2>& mapGridPoint = m_TileCenter->GetMapGridPoint();
	vector<vector<TileState>>& vecTileState = m_TileCenter->GetTileState();

	for (int y = 0; y < 9; ++y) {
		for (int x = 0; x < 9; ++x) {
			int drawX = startX + (x - y) * (TILE_WIDTH / 2);
			int drawY = startY + (x + y) * (TILE_HEIGHT / 2) - (TILE_HEIGHT / 2);

			// 좌표 저장
			Vec2 gridPos = Vec2(x, y);
			Vec2 realPos = Vec2(drawX, drawY);

			// 이중맵으로 격자 좌표계 <-> 실제 좌표 연결
			mapRealPoint.insert(make_pair(gridPos, realPos));
			mapGridPoint.insert(make_pair(realPos, gridPos));

			// 타일 생성
			CTile* pTile = new CTile;
			pTile->SetPos(Vec2(drawX, drawY));
			vecTileState[y][x].pTile = pTile;
			AddObject(pTile, GROUP_TYPE::TILE);

			// 블럭 복사생성
			CBlock* cBlcok = new CBlock(BLOCK_TYPE::SNOW);
			cBlcok->SetPos(Vec2(drawX, drawY));
			AddObject(cBlcok, GROUP_TYPE::BLOCK);
		}
	}

	// Player 추가
	CPlayer* pPlayer = new CPlayer;
	Vec2 PlayerStartPos(4, 2);
	pPlayer->SetPos(REAL(PlayerStartPos));
	m_pPlayer = pPlayer;
	AddObject(pPlayer, GROUP_TYPE::PLAYER);

	// GDI+ Test (Effect)
	CEffect* pEffect = new CEffect;
	pEffect->SetPos(REAL(PlayerStartPos));
	AddObject(pEffect, GROUP_TYPE::MISSILE_PLAYER);

	// Monster 추가
	CMonster* pMonster = new CMonster;
	Vec2 MonsterStartPos(6, 3);
	pMonster->SetPos(REAL(MonsterStartPos));
	AddObject(pMonster, GROUP_TYPE::MONSTER);

	// 카메라 설정
	CCamera::GetInstance()->SetLookAt(REAL(PlayerStartPos));

	// 타일과 마우스의 충돌처리
	CCollisionMgr::GetInstance()->CheckGroup(GROUP_TYPE::MOUSE, GROUP_TYPE::TILE);
}

void CScene_Battle::TurnLogic()
{
	TURN_TYPE currentTurn = m_TurnCenter->GetTurnState();
	switch (currentTurn)
	{
	case TURN_TYPE::ENTER: break;
	case TURN_TYPE::PLAYER_START: break;
	case TURN_TYPE::PLAYER_TILESELECT: break;
	case TURN_TYPE::PLAYER_MOVE:
	{
		PlayerMove();
		break;
	}
	case TURN_TYPE::PLAYER_ATTACK: break;
	case TURN_TYPE::PLAYER_SKILL: break;
	case TURN_TYPE::ENEMY_MOVE: break;
	case TURN_TYPE::ENEMY_ATTACK: break;
	case TURN_TYPE::EXIT: break;
	}
}

void CScene_Battle::PlayerMove()
{
	list<Vec2>& moveRoute = m_TurnCenter->GetMoveRoute();
	if (moveRoute.empty())
	{
		// 턴 매니저에서 조건 체크 해서, 상태 변경됨
		return;
	}

	Vec2 vDestination = REAL(moveRoute.front());
	Vec2 vCurrentPos = m_pPlayer->GetPos();

	// 도착했음(위치가 일치) = 주변 적군 체크 -> 갱신
	if (vCurrentPos == vDestination)
	{
		// 1. 적군 탐색 후, 적군이 있으면 함수 리턴 후 공격상태 들어감(lstTargetEnemy 배열에 적들 정보 들어감)
		// Update에서 적 리스트 빈거 체크하고, 안비었으면 빌때까지 공격상태 돌입
		/*BFS(m_mapGridPoint[vDestination], DIRECTION::FOUR_WAY, 1);*/

		// 2. 타일 상태 갱신 -> 이동한 발판은 검은색 처리
		Vec2 gridDestination = moveRoute.front();
		vector<vector<TileState>>& tiles = m_TileCenter->GetTileState();
		tiles[(int)gridDestination.y][(int)gridDestination.x].pTile->SetTileState(TILE_STATE::BLACK);

		// 3. 타일 상태 갱신(목적지, 현재위치 갱신), 타일 리스트 한칸 삭제
		m_TurnCenter->SetPlayerPos(moveRoute.front());
		moveRoute.pop_front();
		if (moveRoute.empty()) return;		// 목적지 타일 도착하면, 함수 탈출

		vDestination = REAL(moveRoute.front());
	}

	// 캐릭터에게 도착지쪽으로 움직이게 명령
	m_pPlayer->Move(vDestination);
}

void CScene_Battle::TileSelectTrigger(CObject* _pObj)
{
	// 조건 :: 마우스를 꾹 누른 상태에서 타일의 콜라이더와 닿은 상태
	// 마우스 꾹 눌린 상태에서, 콜라이더가 닿으면 -> 이벤트 매니저에서 이 함수를 호출시킴
	TURN_TYPE CurrnetTurn = m_TurnCenter->GetTurnState();
	vector<vector<TileState>>& vecTiles = m_TileCenter->GetTileState();

	switch (CurrnetTurn)
	{
	case TURN_TYPE::PLAYER_START:
	{
		Vec2 vPlayerPos = GRID(m_pPlayer->GetPos());
		Vec2 selectPos = GRID(_pObj->GetPos());

		// BFS로 8방향 탐색 (주변 1칸)
		m_BFS->BFS(vPlayerPos, vecTiles, DIRECTION::EIGHT_WAY, 1);

		// BFS 탐색결과, 방문 했었으면(result 배열에 있으면, 찾은것)
		if (vecTiles[(int)selectPos.y][(int)selectPos.x].bVisited)
		{
			// 타일 색깔 지정, 현재 위치 갱신, 리스트 추가, 타일 선택됐다고 함수 날려주기
			CTile* tile = (CTile*)_pObj;

			// 타일 색깔 지정, 현재 위치 갱신, 리스트 추가, 타일 선택됐다고 함수 날려주기, 턴 전환
			m_TurnCenter->SetTileColor(tile->GetTileState());
			m_TurnCenter->SetSelectTile(selectPos);
			m_TurnCenter->GetMoveRoute().push_back(selectPos);
			tile->SetTileState((TILE_STATE)((int)tile->GetTileState() + 4));

			// 디버깅
			DEBUG2(selectPos.x, selectPos.y);
			printf("타일 시작\n");

			// 카메라 타일로 지정
			CCamera::GetInstance()->SetLookAt(REAL(selectPos));

			// 턴 변경
			m_TurnCenter->SetTurnState(TURN_TYPE::PLAYER_TILESELECT);
		}
	}
		break;
	case TURN_TYPE::PLAYER_TILESELECT:
	{
		Vec2 selectPos = GRID(_pObj->GetPos());
		Vec2 currentPos = m_TurnCenter->GetSelectTile();
		m_BFS->BFS(currentPos, vecTiles, DIRECTION::EIGHT_WAY, 1);

		// 중복된 위치는 리스트에 들어가지 못하게 설정
		list<Vec2>& moveRoute = m_TurnCenter->GetMoveRoute();
		auto iter = std::find(moveRoute.begin(), moveRoute.end(), selectPos);
		if (vecTiles[(int)selectPos.y][(int)selectPos.x].bVisited && 
			vecTiles[(int)selectPos.y][(int)selectPos.x].pTile->GetTileState() == m_TurnCenter->GetTileColor() &&
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

	// 메인 씬 업데이트 (각자 오브젝트들의 업데이트)
	CScene::Update();

	// 전투 로직 처리
	TurnLogic();

	// 턴 결정 업데이트
	m_TurnCenter->Update();
}

void CScene_Battle::Enter()
{
	// 버튼 이벤트로 대체시키기
	InitField(1, FIELD_TYPE::COMMON);
}

void CScene_Battle::Exit()
{
	DeleteAll();
	m_BFS->BFS_Init(m_TileCenter->GetTileState());
	m_TurnCenter->Init();
	CCollisionMgr::GetInstance()->Reset();
}


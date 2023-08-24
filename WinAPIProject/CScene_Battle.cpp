#include "pch.h"
#include <queue>
#include "CScene_Battle.h"

#include "CCore.h"
#include "CCollisionMgr.h"
#include "CKeyMgr.h"

#include "CPlayer.h"
#include "CTile.h"
#include "CBlock.h"
#include "CMouse.h"
#include "CEffect.h"
#include "CMonster.h"



CScene_Battle::CScene_Battle()
	: m_vecTileInfo(GRID_X, vector<TileInfo>(GRID_Y,0))
	, m_mapRealPoint{}
	, m_mapGridPoint{}
	, m_CurrentTurn(TURN_TYPE::PLAYER_START)
	, m_lstTile{}
{
}

CScene_Battle::~CScene_Battle()
{
}

void CScene_Battle::PlayerStart()
{

}

void CScene_Battle::PlayerMove()
{
	CPlayer* pPlayer = (CPlayer*)GetGroupObject(GROUP_TYPE::PLAYER).front();
	assert(pPlayer);
	CCamera::GetInstance()->SetTarget(pPlayer);

	if (m_lstTile.empty())
	{
		return;
	}

	Vec2 vDestination = m_mapRealPoint[m_lstTile.front()];
	Vec2 vCurrentPos = pPlayer->GetPos();

	// 도착했음(위치가 일치) -> 주변 적군 체크
	if (vCurrentPos == vDestination)
	{
		// 1. 적군 탐색 후, 적군이 있으면 함수 리턴 후 공격상태 들어감(lstTargetEnemy 배열에 적들 정보 들어감)
		// Update에서 적 리스트 빈거 체크하고, 안비었으면 빌때까지 공격상태 돌입
		/*BFS(m_mapGridPoint[vDestination], DIRECTION::FOUR_WAY, 1);*/

		// 2. 타일 상태 갱신 -> 이동한 발판은 검은색 처리
		Vec2 gridDestination = m_lstTile.front();
		int tile_number = int(gridDestination.x + gridDestination.y * GRID_Y);
		vector<CObject*> groupTile = GetGroupObject(GROUP_TYPE::TILE);
		((CTile*)groupTile[tile_number])->SetTileState(TILE_STATE::BLACK);

		// 3. 타일 상태 갱신(목적지, 현재위치 갱신), 타일 리스트 한칸 삭제
		m_vPlayerPos = m_lstTile.front();
		m_lstTile.pop_front();
		if (m_lstTile.empty()) return;		// 목적지 타일 도착하면, 함수 탈출

		vDestination = m_mapRealPoint[m_lstTile.front()];
	}
	pPlayer->Move(vDestination);
}

void CScene_Battle::PlayerAttack()
{
	if (m_lstTargetEnemy.empty())
	{
		return;
	}

	// 타격 판정이 들어가면 (플레이어의 공격 애니메이션 종료) -> 그때 적군 타격 + 리스트 삭제하게 하기
	CPlayer* pPlayer = (CPlayer*)GetGroupObject(GROUP_TYPE::PLAYER).front();
	CMonster* pMonster = m_lstTargetEnemy.front();
}

void CScene_Battle::PlayerSkillInit()
{
	// 검은 타일들(밟고 지나왔던 타일들) 랜덤 타일들로 리셋시키기
	vector<CObject*> groupTiles = GetGroupObject(GROUP_TYPE::TILE);
	
	for (auto& tile : groupTiles)
	{
		CTile* pTile = (CTile*)tile;
		if (pTile->GetTileState() == TILE_STATE::BLACK)
		{
			pTile->SetTileState(pTile->RandomTile());
		}
	}

	// 플레이어 애니메이션 설정
	CPlayer* pPlayer = (CPlayer*)GetGroupObject(GROUP_TYPE::PLAYER).front();
	pPlayer->SetState(PLAYER_STATE::ROLLING);
}

void CScene_Battle::TileSelectTrigger(CObject* _pObj)
{
	// 조건 :: 마우스를 꾹 누른 상태에서 타일의 콜라이더와 닿은 상태
	// 마우스 꾹 눌린 상태에서, 콜라이더가 닿으면 -> 이벤트 매니저에서 이 함수를 호출시킴
	switch (m_CurrentTurn)
	{
	case TURN_TYPE::PLAYER_START:
	{
		// BFS로 8방향 탐색 (주변 1칸)
		BFS(m_vPlayerPos, DIRECTION::EIGHT_WAY, 1);
		Vec2 selectPos = m_mapGridPoint[_pObj->GetPos()];

		// BFS 탐색결과, 방문 했었으면
		if (m_vecTileInfo[(int)selectPos.y][(int)selectPos.x].bVisited)
		{
			CTile* tile = (CTile*)_pObj;

			// 카메라 타일로 지정
			CCamera::GetInstance()->SetLookAt(m_mapRealPoint[selectPos]);

			// 타일 색깔 지정, 현재 위치 갱신, 리스트 추가, 타일 선택됐다고 함수 날려주기, 턴 전환
			m_TileColor = tile->GetTileState();
			m_vSelectTile = selectPos;
			m_lstTile.push_back(selectPos);
			tile->SetTileState((TILE_STATE)((int)tile->GetTileState() + 4));
			m_CurrentTurn = TURN_TYPE::PLAYER_BLOCKSELECT;

			// 디버깅
			DEBUG2(selectPos.x, selectPos.y);
			DEBUG1((int)m_TileColor);
			printf("타일 시작\n");
		}
	}
		break;
	case TURN_TYPE::PLAYER_BLOCKSELECT:
	{
		BFS(m_vSelectTile, DIRECTION::EIGHT_WAY, 1);
		Vec2 selectPos = m_mapGridPoint[_pObj->GetPos()];

		// 중복된 위치는 리스트에 들어가지 못하게 설정
		auto iter = std::find(m_lstTile.begin(), m_lstTile.end(), selectPos);
		if (m_vecTileInfo[(int)selectPos.y][(int)selectPos.x].bVisited && iter == m_lstTile.end())
		{
			CTile* tile = (CTile*)_pObj;

			// 카메라 타일로 지정
			CCamera::GetInstance()->SetLookAt(m_mapRealPoint[selectPos]);

			// 현재 위치 갱신, 리스트 추가, 타일 선택됐다고 함수 날려주기
			m_vSelectTile = selectPos;
			m_lstTile.push_back(selectPos);
			tile->SetTileState((TILE_STATE)((int)tile->GetTileState() + 4));

			// 디버깅
			for (auto& lstPos : m_lstTile) printf("%1.f, %1.f -> ", lstPos.x, lstPos.y);
			printf("\n");
		}
	}
		break;
	default:
		break;
	}

	// BFS 방문 초기화
	BFSInit();
}

void CScene_Battle::TileSelectInit()
{
	vector<CObject*> groupTile = GetGroupObject(GROUP_TYPE::TILE);

	// 리스트 내에 있는 모든 타일들의 색깔을 원래 색상으로 돌리기
	int tile_number;
	for (auto& tilePos : m_lstTile)
	{
		tile_number = int(tilePos.x + tilePos.y * GRID_Y);
		CTile* tile = (CTile*)groupTile[tile_number];
		tile->SetTileState((TILE_STATE)((int)tile->GetTileState() - 4));
	}

	// 카메라 캐릭터로 초기화
	CCamera::GetInstance()->SetTarget(nullptr);
	CCamera::GetInstance()->SetLookAt(m_mapRealPoint[m_vPlayerPos]);

	// 리스트 초기화
	m_lstTile.clear();
}

void CScene_Battle::PlayerMoveInit()
{
	CPlayer* pPlayer = (CPlayer*)GetGroupObject(GROUP_TYPE::PLAYER).front();
	pPlayer->SetState(PLAYER_STATE::MOVE);
}

void CScene_Battle::EnemyStart()
{

}

bool CScene_Battle::isValid(Vec2 _vPos)
{

	return (_vPos.x >= 0 && _vPos.x < GRID_X &&
			_vPos.y >= 0 && _vPos.y < GRID_Y &&
			!m_vecTileInfo[(int)_vPos.y][(int)_vPos.x].bVisited);
}

void CScene_Battle::BFS(Vec2 _startPos, DIRECTION _dir, int _depth)
{
	vector<Vec2> direction;
	vector<CObject*> groupTile = GetGroupObject(GROUP_TYPE::TILE);

	// 방향 선택
	switch (_dir)
	{
	case DIRECTION::FOUR_WAY:
		direction.push_back(Vec2(-1, 0));
		direction.push_back(Vec2(1, 0));
		direction.push_back(Vec2(0, -1));
		direction.push_back(Vec2(0, 1));
		break;
	case DIRECTION::DIAGONAL:
		direction.push_back(Vec2(-1, -1));
		direction.push_back(Vec2(1, -1));
		direction.push_back(Vec2(-1, 1));
		direction.push_back(Vec2(1, 1));
		break;
	case DIRECTION::EIGHT_WAY:
		direction.push_back(Vec2(-1, 0));
		direction.push_back(Vec2(1, 0));
		direction.push_back(Vec2(0, -1));
		direction.push_back(Vec2(0, 1));
		direction.push_back(Vec2(-1, -1));
		direction.push_back(Vec2(1, -1));
		direction.push_back(Vec2(-1, 1));
		direction.push_back(Vec2(1, 1));
		break;
	}

	std::queue<Vec2> q;
	std::queue<int> moveCount;	// 각각의 타일마다 탐색 횟수 (깊이 설정)
	q.push(_startPos);
	moveCount.push(0);

	while (!q.empty())
	{
		Vec2 currentPos = q.front();
		int count = moveCount.front();
		q.pop();
		moveCount.pop();

		if (count >= _depth) continue;	// 깊이가 최대깊이 도달하면 스킵

		for (int i = 0; i < direction.size(); i++)
		{
			Vec2 searchPos(currentPos.x + direction[i].x, currentPos.y + direction[i].y);
			if (isValid(searchPos))
			{
				switch (m_CurrentTurn)
				{
				case TURN_TYPE::PLAYER_START:
				{
					// 블럭 선택중일때는, 첫 선택한 블럭의 색깔과 일치해야함
					m_vecTileInfo[(int)searchPos.y][(int)searchPos.x].bVisited = true;
					q.push(searchPos);
					moveCount.push(count + 1);
				}
					break;
				case TURN_TYPE::PLAYER_BLOCKSELECT:
				{
					// 블럭 첫선택일때는 그냥 넣음
					int tile_number = int(searchPos.x + searchPos.y * GRID_Y);

					if (((CTile*)groupTile[tile_number])->GetTileState() == m_TileColor)
					{
						m_vecTileInfo[(int)searchPos.y][(int)searchPos.x].bVisited = true;
						q.push(searchPos);
						moveCount.push(count + 1);
					}
				}
					break;
				case TURN_TYPE::PLAYER_MOVE:
				{
					// 타일 위에 적군이 있는지 체크
					int tile_number = int(searchPos.x + searchPos.y * GRID_Y);
					CMonster* pEnemy = (CMonster*)m_vecTileInfo[(int)searchPos.y][(int)searchPos.x].ObjOnTile;

					if (pEnemy)
					{
						// 리스트에 적군들을 넣고, BFS가 끝나면 한번에 처리(리스트 순서대로)
						m_lstTargetEnemy.push_back(pEnemy);

						m_vecTileInfo[(int)searchPos.y][(int)searchPos.x].bVisited = true;
						q.push(searchPos);
						moveCount.push(count + 1);
					}
				}
				default:
					assert(0);	// 잘못 선택됨
					break;
				}
			}
		}
	}
}

void CScene_Battle::BFSInit()
{
	// BFS 방문 초기화
	for (size_t i = 0; i < m_vecTileInfo.size(); i++)
	{
		for (size_t j = 0; j < m_vecTileInfo[i].size(); j++)
		{
			m_vecTileInfo[j][i].bVisited = false;
		}
	}
}

void CScene_Battle::Update()
{
	CScene::Update();

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

	switch (m_CurrentTurn)
	{
	case TURN_TYPE::ENTER:
		break;
	case TURN_TYPE::PLAYER_START:
		break;
	case TURN_TYPE::PLAYER_BLOCKSELECT:
	{
		// 타일 결정
		if (KEY_TAP(KEY::CTRL))
		{
			PlayerMoveInit();
			m_CurrentTurn = TURN_TYPE::PLAYER_MOVE;
			break;
		}
		// 선택 취소
		if (KEY_TAP(KEY::ESC))
		{
			TileSelectInit();
			m_CurrentTurn = TURN_TYPE::PLAYER_START;
			break;
		}
	}
		break;
	case TURN_TYPE::PLAYER_MOVE:
	{
		// BFS 탐색 결과로 적이 존재하게 되면, 잠깐 공격 상태에 돌입 후 다시 이동상태 돌입
		if (!m_lstTargetEnemy.empty())
		{
			m_CurrentTurn = TURN_TYPE::PLAYER_ATTACK;
			break;
		}

		// 이동할 타일이 사라지면, 스킬 단계로 넘어가게됨
		if(m_lstTile.empty())
		{
			PlayerSkillInit();
			m_CurrentTurn = TURN_TYPE::PLAYER_SKILL;
			break;
		}
		PlayerMove();
	}
		break;
	case TURN_TYPE::PLAYER_ATTACK:
	{
		// 공격 처리를 전부 처리하면, 다시 움직이는 상태로 전환시키기
		if (m_lstTargetEnemy.empty())
		{
			m_CurrentTurn = TURN_TYPE::PLAYER_MOVE;
			break;
		}
		PlayerAttack();
	}
	break;
	case TURN_TYPE::PLAYER_SKILL:
	{
		TileSelectInit();
		m_CurrentTurn = TURN_TYPE::PLAYER_START;
	}
		break;
	case TURN_TYPE::ENEMY_MOVE:
		break;
	case TURN_TYPE::ENEMY_ATTACK:
		break;
	case TURN_TYPE::EXIT:
		break;
	default:
		break;
	}
}

void CScene_Battle::Enter()
{
	Vec2 vResolution = CCore::GetInstance()->GetResolution();
	m_CurrentTurn = TURN_TYPE::PLAYER_START;

	Vec2 PlayerStartPos(4, 2);

	int startX = (int)(vResolution.x / 2);
	int startY = (int)(vResolution.y / 4);

	// 마우스 추가
	CMouse* pMouse = new CMouse;
	AddObject(pMouse, GROUP_TYPE::MOUSE);

	// 타일 + 블럭 추가
	for (int y = 0; y < 9; ++y) {
		for (int x = 0; x < 9; ++x) {
			int drawX = startX + (x - y) * (TILE_WIDTH / 2);
			int drawY = startY + (x + y) * (TILE_HEIGHT / 2) - (TILE_HEIGHT / 2);

			// 좌표 저장
			Vec2 Coordinate = Vec2(x, y);
			Vec2 actualCoord = Vec2(drawX, drawY);

			// 이중맵으로 격자 좌표계 <-> 실제 좌표 연결
			m_mapRealPoint.insert(make_pair(Coordinate, actualCoord));
			m_mapGridPoint.insert(make_pair(actualCoord, Coordinate));

			// 타일 생성
			CTile* pTile = new CTile;
			pTile->SetPos(Vec2(drawX, drawY));
			m_vecTileInfo[y][x] = ((int)pTile->GetTileState());
			AddObject(pTile, GROUP_TYPE::TILE);

			// 타일 블럭 생성
			CBlock* pBlcok = new CBlock(MAP_TYPE::SNOW);
			pBlcok->SetPos(Vec2(drawX, drawY));
			AddObject(pBlcok, GROUP_TYPE::BLOCK);
		}
	}

	// Player 추가
	CPlayer* pObj = new CPlayer;
	pObj->SetPos(m_mapRealPoint[PlayerStartPos]);
	m_vPlayerPos = PlayerStartPos;
	AddObject(pObj, GROUP_TYPE::PLAYER);

	 // GDI+ Test (Effect)
	{
		CEffect* pEffect = new CEffect;
		pEffect->SetPos(m_mapRealPoint[Vec2(4, 2)]);
		AddObject(pEffect, GROUP_TYPE::MISSILE_PLAYER);
	}

	// 타일과 마우스의 충돌처리
	CCollisionMgr::GetInstance()->CheckGroup(GROUP_TYPE::MOUSE, GROUP_TYPE::TILE);
	
	// 카메라 설정
	CCamera::GetInstance()->SetLookAt(m_mapRealPoint[PlayerStartPos]);
}

void CScene_Battle::Exit()
{
	DeleteAll();
	m_mapRealPoint.clear();
	m_mapGridPoint.clear();
	m_lstTile.clear();
	CCollisionMgr::GetInstance()->Reset();
}


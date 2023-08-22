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

void CScene_Battle::TileSelectTrigger(CObject* _pObj)
{
	// 마우스 꾹 눌린 상태에서, 콜라이더가 닿으면 -> 이벤트 매니저에서 이 함수가 호출됨
	switch (m_CurrentTurn)
	{
	case TURN_TYPE::PLAYER_START:
	{
		BFS(m_vPlayerPos, DIRECTION::EIGHT_WAY, 1);
		Vec2 selectPos = m_mapGridPoint[_pObj->GetPos()];

		// BFS 탐색결과, 방문 했었으면
		if (m_vecTileInfo[selectPos.y][selectPos.x].bVisited)
		{
			printf("asdf");
		}
	}
		break;
	case TURN_TYPE::PLAYER_BLOCKSELECT:
		break;
	default:
		break;
	}

	// BFS 방문 초기화
	//Init();
}

void CScene_Battle::EnemyStart()
{

}

bool CScene_Battle::isValid(Vec2 _vPos)
{
	// 추후 조건 추가
	return (_vPos.x >= 0 && _vPos.x < GRID_X && _vPos.y >= 0 && _vPos.y < GRID_Y && !m_vecTileInfo[_vPos.y][_vPos.x].bVisited);
}

void CScene_Battle::BFS(Vec2 _startPos, DIRECTION _dir, int _depth)
{
	vector<Vec2> direction;

	// 방향 선택
	switch (_dir)
	{
	case FOUR_WAY:
		direction.push_back(Vec2(-1, 0));
		direction.push_back(Vec2(1, 0));
		direction.push_back(Vec2(0, -1));
		direction.push_back(Vec2(0, 1));
		break;
	case DIAGONAL:
		direction.push_back(Vec2(-1, -1));
		direction.push_back(Vec2(1, -1));
		direction.push_back(Vec2(-1, 1));
		direction.push_back(Vec2(1, 1));
		break;
	case EIGHT_WAY:
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
				m_vecTileInfo[searchPos.y][searchPos.x].bVisited = true;
				q.push(searchPos);
				moveCount.push(count + 1);
			}
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

	switch (m_CurrentTurn)
	{
	case TURN_TYPE::ENTER:
		break;
	case TURN_TYPE::PLAYER_START:
		PlayerStart();
		break;
	case TURN_TYPE::PLAYER_BLOCKSELECT:
		break;
	case TURN_TYPE::PLAYER_MOVE:
		break;
	case TURN_TYPE::PLAYER_SKILL:
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
	Vec2 PlayerStartPos(4, 2);

	Vec2 vResolution = CCore::GetInstance()->GetResolution();

	int startX = (int)(vResolution.x / 2);
	int startY = (int)(vResolution.y / 4);

	// 마우스 추가
	CMouse* pMouse = new CMouse;
	AddObject(pMouse, GROUP_TYPE::MOUSE);

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

	 // GDI+ Test
	{
		CEffect* pEffect = new CEffect;
		pEffect->SetPos(m_mapRealPoint[Vec2(4, 2)]);
		AddObject(pEffect, GROUP_TYPE::MISSILE_PLAYER);
	}

	// 타일과 마우스의 충돌처리
	CCollisionMgr::GetInstance()->CheckGroup(GROUP_TYPE::MOUSE, GROUP_TYPE::TILE);
	
	// 카메라 설정
	CCamera::GetInstance()->SetLookAt(vResolution / 2.f);
}

void CScene_Battle::Exit()
{
	DeleteAll();
	CCollisionMgr::GetInstance()->Reset();
}

void CScene_Battle::Init()
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

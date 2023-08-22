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
	// ���콺 �� ���� ���¿���, �ݶ��̴��� ������ -> �̺�Ʈ �Ŵ������� �� �Լ��� ȣ���
	switch (m_CurrentTurn)
	{
	case TURN_TYPE::PLAYER_START:
	{
		BFS(m_vPlayerPos, DIRECTION::EIGHT_WAY, 1);
		Vec2 selectPos = m_mapGridPoint[_pObj->GetPos()];

		// BFS Ž�����, �湮 �߾�����
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

	// BFS �湮 �ʱ�ȭ
	//Init();
}

void CScene_Battle::EnemyStart()
{

}

bool CScene_Battle::isValid(Vec2 _vPos)
{
	// ���� ���� �߰�
	return (_vPos.x >= 0 && _vPos.x < GRID_X && _vPos.y >= 0 && _vPos.y < GRID_Y && !m_vecTileInfo[_vPos.y][_vPos.x].bVisited);
}

void CScene_Battle::BFS(Vec2 _startPos, DIRECTION _dir, int _depth)
{
	vector<Vec2> direction;

	// ���� ����
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
	std::queue<int> moveCount;	// ������ Ÿ�ϸ��� Ž�� Ƚ�� (���� ����)
	q.push(_startPos);
	moveCount.push(0);

	while (!q.empty())
	{
		Vec2 currentPos = q.front();
		int count = moveCount.front();
		q.pop();
		moveCount.pop();

		if (count >= _depth) continue;	// ���̰� �ִ���� �����ϸ� ��ŵ

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

	// ���콺 �߰�
	CMouse* pMouse = new CMouse;
	AddObject(pMouse, GROUP_TYPE::MOUSE);

	for (int y = 0; y < 9; ++y) {
		for (int x = 0; x < 9; ++x) {
			int drawX = startX + (x - y) * (TILE_WIDTH / 2);
			int drawY = startY + (x + y) * (TILE_HEIGHT / 2) - (TILE_HEIGHT / 2);

			// ��ǥ ����
			Vec2 Coordinate = Vec2(x, y);
			Vec2 actualCoord = Vec2(drawX, drawY);

			// ���߸����� ���� ��ǥ�� <-> ���� ��ǥ ����
			m_mapRealPoint.insert(make_pair(Coordinate, actualCoord));
			m_mapGridPoint.insert(make_pair(actualCoord, Coordinate));

			// Ÿ�� ����
			CTile* pTile = new CTile;
			pTile->SetPos(Vec2(drawX, drawY));
			m_vecTileInfo[y][x] = ((int)pTile->GetTileState());
			AddObject(pTile, GROUP_TYPE::TILE);

			// Ÿ�� �� ����
			CBlock* pBlcok = new CBlock(MAP_TYPE::SNOW);
			pBlcok->SetPos(Vec2(drawX, drawY));
			AddObject(pBlcok, GROUP_TYPE::BLOCK);
		}
	}

	// Player �߰�
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

	// Ÿ�ϰ� ���콺�� �浹ó��
	CCollisionMgr::GetInstance()->CheckGroup(GROUP_TYPE::MOUSE, GROUP_TYPE::TILE);
	
	// ī�޶� ����
	CCamera::GetInstance()->SetLookAt(vResolution / 2.f);
}

void CScene_Battle::Exit()
{
	DeleteAll();
	CCollisionMgr::GetInstance()->Reset();
}

void CScene_Battle::Init()
{
	// BFS �湮 �ʱ�ȭ
	for (size_t i = 0; i < m_vecTileInfo.size(); i++)
	{
		for (size_t j = 0; j < m_vecTileInfo[i].size(); j++)
		{
			m_vecTileInfo[j][i].bVisited = false;
		}
	}
}

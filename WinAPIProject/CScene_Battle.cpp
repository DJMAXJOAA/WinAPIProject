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

	if (m_lstTile.empty())
	{
		return;
	}

	Vec2 vDestination = m_mapRealPoint[m_lstTile.front()];
	Vec2 vCurrentPos = pPlayer->GetPos();

	// ��������(��ġ�� ��ġ) -> �ֺ� ���� üũ
	if (vCurrentPos == vDestination)
	{
		// 1. ���� Ž�� ��, ������ ����
		BFS(m_mapGridPoint[vDestination], DIRECTION::FOUR_WAY, 1);
		for (auto& pMonster : m_lstTargetEnemy)
		{
			// ���� �ѹ��� ó��
			PlayerAttack(pPlayer, pMonster);
		}
		m_lstTargetEnemy.clear();

		// 2. ���� ����ġ ���� + �̵� ����Ʈ�� ����ó�� + Ÿ�� ����
		int tile_number = int(vDestination.x + vDestination.y * GRID_Y);
		vector<CObject*> groupTile = GetGroupObject(GROUP_TYPE::TILE);

		m_vPlayerPos = m_lstTile.front();
		m_lstTile.pop_front();
		((CTile*)groupTile[tile_number])->SetTileState(TILE_STATE::BLACK);
	}

	// ���� Ÿ�Ϸ� �̵� ��
	
}

void CScene_Battle::PlayerAttack(CObject* _pPlayer, CObject* _pEnmey)
{
}

bool CScene_Battle::IsListTileEmpty()
{
	return m_lstTile.empty();
}

void CScene_Battle::TileSelectTrigger(CObject* _pObj)
{
	// ���� :: ���콺�� �� ���� ���¿��� Ÿ���� �ݶ��̴��� ���� ����
	// ���콺 �� ���� ���¿���, �ݶ��̴��� ������ -> �̺�Ʈ �Ŵ������� �� �Լ��� ȣ���Ŵ
	switch (m_CurrentTurn)
	{
	case TURN_TYPE::PLAYER_START:
	{
		// BFS�� 8���� Ž�� (�ֺ� 1ĭ)
		BFS(m_vPlayerPos, DIRECTION::EIGHT_WAY, 1);
		Vec2 selectPos = m_mapGridPoint[_pObj->GetPos()];

		// BFS Ž�����, �湮 �߾�����
		if (m_vecTileInfo[(int)selectPos.y][(int)selectPos.x].bVisited)
		{
			CTile* tile = (CTile*)_pObj;

			// ī�޶� Ÿ�Ϸ� ����
			CCamera::GetInstance()->SetLookAt(m_mapRealPoint[selectPos]);

			// Ÿ�� ���� ����, ���� ��ġ ����, ����Ʈ �߰�, Ÿ�� ���õƴٰ� �Լ� �����ֱ�, �� ��ȯ
			m_TileColor = tile->GetTileState();
			m_vSelectTile = selectPos;
			m_lstTile.push_back(selectPos);
			tile->SetTileState((TILE_STATE)((int)tile->GetTileState() + 4));
			m_CurrentTurn = TURN_TYPE::PLAYER_BLOCKSELECT;

			// �����
			DEBUG2(selectPos.x, selectPos.y);
			DEBUG1((int)m_TileColor);
			printf("Ÿ�� ����\n");
		}
	}
		break;
	case TURN_TYPE::PLAYER_BLOCKSELECT:
	{
		BFS(m_vSelectTile, DIRECTION::EIGHT_WAY, 1);
		Vec2 selectPos = m_mapGridPoint[_pObj->GetPos()];

		// �ߺ��� ��ġ�� ����Ʈ�� ���� ���ϰ� ����
		auto iter = std::find(m_lstTile.begin(), m_lstTile.end(), selectPos);
		if (m_vecTileInfo[(int)selectPos.y][(int)selectPos.x].bVisited && iter == m_lstTile.end())
		{
			CTile* tile = (CTile*)_pObj;

			// ī�޶� Ÿ�Ϸ� ����
			CCamera::GetInstance()->SetLookAt(m_mapRealPoint[selectPos]);

			// ���� ��ġ ����, ����Ʈ �߰�, Ÿ�� ���õƴٰ� �Լ� �����ֱ�
			m_vSelectTile = selectPos;
			m_lstTile.push_back(selectPos);
			tile->SetTileState((TILE_STATE)((int)tile->GetTileState() + 4));

			// �����
			for (auto& lstPos : m_lstTile) printf("%1.f, %1.f -> ", lstPos.x, lstPos.y);
			printf("\n");
		}
	}
		break;
	default:
		break;
	}

	// BFS �湮 �ʱ�ȭ
	BFSInit();
}

void CScene_Battle::TileSelectInit()
{
	vector<CObject*> groupTile = GetGroupObject(GROUP_TYPE::TILE);

	// ����Ʈ ���� �ִ� ��� Ÿ�ϵ��� ������ ���� �������� ������
	int tile_number;
	for (auto& tilePos : m_lstTile)
	{
		tile_number = int(tilePos.x + tilePos.y * GRID_Y);
		CTile* tile = (CTile*)groupTile[tile_number];
		tile->SetTileState((TILE_STATE)((int)tile->GetTileState() - 4));
	}

	// ī�޶� ĳ���ͷ� �ʱ�ȭ
	CCamera::GetInstance()->SetLookAt(m_mapRealPoint[m_vPlayerPos]);

	// ����Ʈ �ʱ�ȭ
	m_lstTile.clear();
}

void CScene_Battle::PlayerMoveInit()
{
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

	// ���� ����
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
				switch (m_CurrentTurn)
				{
				case TURN_TYPE::PLAYER_START:
				{
					// �� �������϶���, ù ������ ���� ����� ��ġ�ؾ���
					m_vecTileInfo[(int)searchPos.y][(int)searchPos.x].bVisited = true;
					q.push(searchPos);
					moveCount.push(count + 1);
				}
					break;
				case TURN_TYPE::PLAYER_BLOCKSELECT:
				{
					// �� ù�����϶��� �׳� ����
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
					// Ÿ�� ���� ������ �ִ��� üũ
					int tile_number = int(searchPos.x + searchPos.y * GRID_Y);
					CMonster* pEnemy = (CMonster*)m_vecTileInfo[(int)searchPos.y][(int)searchPos.x].ObjOnTile;

					if (pEnemy)
					{
						// ����Ʈ�� �������� �ְ�, BFS�� ������ �ѹ��� ó��(����Ʈ �������)
						m_lstTargetEnemy.push_back(pEnemy);

						m_vecTileInfo[(int)searchPos.y][(int)searchPos.x].bVisited = true;
						q.push(searchPos);
						moveCount.push(count + 1);
					}
				}
				default:
					assert(0);	// �߸� ���õ�
					break;
				}
			}
		}
	}
}

void CScene_Battle::BFSInit()
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


void CScene_Battle::Update()
{
	CScene::Update();

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

	switch (m_CurrentTurn)
	{
	case TURN_TYPE::ENTER:
		break;
	case TURN_TYPE::PLAYER_START:
		break;
	case TURN_TYPE::PLAYER_BLOCKSELECT:
	{
		// Ÿ�� ����
		if (KEY_TAP(KEY::CTRL))
		{
			PlayerMoveInit();
			m_CurrentTurn = TURN_TYPE::PLAYER_MOVE;
		}
		// ���� ���
		if (KEY_TAP(KEY::ESC))
		{
			TileSelectInit();
			m_CurrentTurn = TURN_TYPE::PLAYER_START;
		}
	}
		break;
	case TURN_TYPE::PLAYER_MOVE:
	{
		if (IsListTileEmpty())
		{
			m_CurrentTurn = TURN_TYPE::PLAYER_SKILL;
		}
		PlayerMove();
	}
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
	Vec2 vResolution = CCore::GetInstance()->GetResolution();
	m_CurrentTurn = TURN_TYPE::PLAYER_START;

	Vec2 PlayerStartPos(4, 2);

	int startX = (int)(vResolution.x / 2);
	int startY = (int)(vResolution.y / 4);

	// ���콺 �߰�
	CMouse* pMouse = new CMouse;
	AddObject(pMouse, GROUP_TYPE::MOUSE);

	// Ÿ�� + �� �߰�
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

	 // GDI+ Test (Effect)
	{
		CEffect* pEffect = new CEffect;
		pEffect->SetPos(m_mapRealPoint[Vec2(4, 2)]);
		AddObject(pEffect, GROUP_TYPE::MISSILE_PLAYER);
	}

	// Ÿ�ϰ� ���콺�� �浹ó��
	CCollisionMgr::GetInstance()->CheckGroup(GROUP_TYPE::MOUSE, GROUP_TYPE::TILE);
	
	// ī�޶� ����
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


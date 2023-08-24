#pragma once
#include "CScene.h"

class CPlayer;
class BFSSearch;
class CMonsterFactory;
class CTurnManager;
class CTileManager;
using namespace battle;

struct PlayerState
{
    // �÷��̾� ���� ������
    CPlayer*    pPlayer;        // �÷��̾� ��ü�� ���� ����
    Vec2        vSelectTile;    // ���� �������� Ÿ�� ��ǥ
    list<Vec2>  lstMoveRoute;   // Ÿ�� �̵� ���
    TILE_STATE  TileColor;      // ���� �������� Ÿ�� ����
    PlayerState() : pPlayer(nullptr) {};
};

class CScene_Battle :
    public CScene
{
private:
    PlayerState             m_Player;

    BFSSearch*              m_BFS;              // BFS�� ���ڸ� ��� Ž��
    CMonsterFactory*        m_MonsterFactory;   // ���� ����
    CTurnManager*           m_TurnManager;      // �� ���� �ൿ ����
    CTileManager*           m_TileManager;      // Ÿ�� ����

public:
    CScene_Battle();
    ~CScene_Battle();

public:
    // �̺�Ʈ ȣ�� ���� �Լ�
    void TileSelectTrigger(CObject* _pObj);

public:
    void TurnInit(TURN_TYPE _type);

public:
    virtual void Update();
    virtual void Enter();
    virtual void Exit();
};


#pragma once
#include "CScene.h"
#include "tile.h"

class CPlayer;
class BFSSearch;
class CMonsterFactory;
class CTurnManager;
class CTileManager;

class CScene_Battle :
    public CScene
{
private:
    CPlayer*                m_pPlayer;        // �÷��̾� ��ü�� ���� ����

    BFSSearch*              m_BFS;              // BFS�� ���ڸ� ��� Ž��
    CMonsterFactory*        m_MonsterFactory;   // ���� ����
    CTurnManager*           m_TurnManager;      // �� ���� �ൿ ����
    CTileManager*           m_TileManager;      // Ÿ�� ����

public:
    CScene_Battle();
    ~CScene_Battle();

public:
    // �̺�Ʈ ȣ�� ���� ó�� �Լ�
    void TileSelectTrigger(CObject* _pObj);
    void TurnInit(TURN_TYPE _type);
    void TurnLogic(TURN_TYPE _type);

public:
    void PlayerMove();

public:
    virtual void Update();
    virtual void Enter();
    virtual void Exit();
};


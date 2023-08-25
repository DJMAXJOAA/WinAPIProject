#pragma once
#include "CScene.h"
#include "tile.h"

class CPlayer;
class BFSSearch;
class CMonsterSpawner;
class CTurnManager;
class CTileManager;

class CScene_Battle :
    public CScene
{
private:
    CPlayer*                m_pPlayer;        // �÷��̾� ��ü�� ���� ����

    BFSSearch*              m_BFS;              // BFS�� ���ڸ� ��� Ž��
    CMonsterSpawner*        m_MonsterSpawner;   // ���� ����
    CTurnManager*           m_TurnManager;      // �� ���� �ൿ ����
    CTileManager*           m_TileManager;      // Ÿ�� ����

public:
    CScene_Battle();
    ~CScene_Battle();

public:
    // �̺�Ʈ ȣ�� ���� ó�� �Լ�
    void TileSelectTrigger(CObject* _pObj);
    void TurnInit(TURN_TYPE _type);

public:
    void TurnLogic();
    void PlayerMove();

public:
    virtual void Update() override;
    virtual void Enter() override;
    virtual void Exit() override;
};


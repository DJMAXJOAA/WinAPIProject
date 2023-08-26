#pragma once
#include "CScene.h"
#include "tile.h"

class CPlayer;
class BFSSearch;
class CMonsterSpawner;
class CTurnCenter;
class CTileCenter;

bool CompareGridPos(CObject* _pObj1, CObject* _pObj2);	// ������ ���� ���Ŀ� �� �Լ�

class CScene_Battle :
    public CScene
{
private:
    int                     m_iFieldType;       // �ʵ��� ���� Ÿ�� ID

    CPlayer*                m_pPlayer;          // �÷��̾� ��ü�� ���� ����

    BFSSearch*              m_BFS;              // BFS�� ���ڸ� ��� Ž��
    CMonsterSpawner*        m_MonsterSpawner;   // ���� ����
    CTurnCenter*            m_TurnCenter;       // �� ���� �ൿ ����
    CTileCenter*            m_TileCenter;       // Ÿ�� ����

public:
    CScene_Battle();
    ~CScene_Battle();

public:
    // �̺�Ʈ ȣ�� ���� ó�� �Լ�
    void TileSelectTrigger(CObject* _pObj);
    void TurnInit(TURN_TYPE _type);
    void InitField(int _level, FIELD_TYPE _type);    // ���⸦ Enter�� ���

public:
    // ���� ����
    void TurnLogic();
    void PlayerMove();
    void PlayerAttack();

public:
    // ��Ʋ �������� ����ϴ� ��ƿ��Ƽ
    Vec2 GetRandomGridPos();
    //bool CompareGridPos(CObject* _pObj1, CObject* _pObj2);	// ������ ���� ���Ŀ� �� �Լ�


public:
    virtual void Update() override;
    virtual void Enter() override;
    virtual void Exit() override;
};


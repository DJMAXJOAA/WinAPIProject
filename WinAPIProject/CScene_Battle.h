#pragma once
#include "CScene.h"

enum class TURN_TYPE
{
    PLAYER_START,       // 1. �÷��̾ Ÿ���� �����ϱ� ��
    PLAYER_SELECT,      // 2. �÷��̾ Ÿ���� ������
    PLAYER_MOVE,        // 3. �÷��̾ �̵���
    PLAYER_ATTACK,      // 4. �÷��̾ ������
    ENEMY_MOVE,         // 5. ������ �̵�+������
};

class CScene_Battle :
    public CScene
{
private:
    TURN_TYPE               m_CurrentTurn;      // ���� �� ��Ȳ

    vector<vector<int>>     m_vecTileState;     // Ÿ�� ��Ȳ
    map<Vec2, Vec2>         m_mapPoint;         // ��ǥ ����� ���� ��ǥ��? (�̵��� �ʿ�)

    Vec2                    m_vPlayerPos;       // �÷��̾��� ���� ��ġ
    Vec2                    m_vSelectTile;      // ���� �÷��̾ �������� Ÿ��

public:
    CScene_Battle();
    ~CScene_Battle();

public:
    virtual void Update();
    virtual void Enter();
    virtual void Exit();
};


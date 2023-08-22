#pragma once
#include "CScene.h"

enum class TURN_TYPE
{
    ENTER,                  // ���� ����
    PLAYER_START,           // �÷��̾� :: ù �� ���� ��
    PLAYER_BLOCKSELECT,     // �÷��̾� :: �� ������
    PLAYER_MOVE,            // �÷��̾� :: ���� Ȯ���ϰ�, �����̴���
    PLAYER_SKILL,           // �÷��̾� :: �������� ������, ��ų ������
    ENEMY_MOVE,             // �� :: ������ ��� + ������
    ENEMY_ATTACK,           // �� :: �������� ������, �÷��̾� ���� Ȥ�� ��ų
    EXIT,                   // ���� ����
};

struct Node
{
    Vec2            vPos;           // ���� ��ǥ
    vector<Node*>   vecNeighbor;    // �ش� ����� �̿�
};

class CScene_Battle :
    public CScene
{
private:
    TURN_TYPE               m_CurrentTurn;      // ���� �� ��Ȳ

    vector<vector<int>>     m_vecTileState;     // Ÿ���� ������
    map<Vec2, Vec2>         m_mapPoint;         // ��ǥ ����� ���� ��ǥ��? (�̵��� �ʿ�)

    Vec2                    m_vPlayerPos;       // �÷��̾��� ���� ��ġ
    Vec2                    m_vSelectTile;      // ���� �÷��̾ �������� Ÿ��
    TILE_STATE              m_TileColor;        // ù��° �������� � ���� ����?

public:
    CScene_Battle();
    ~CScene_Battle();

public:
    void PlayerStart();

public:
    void EnemyStart();

public:
    virtual void Update();
    virtual void Enter();
    virtual void Exit();
};


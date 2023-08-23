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

enum class DIRECTION
{
    FOUR_WAY,       // �����¿� ����
    DIAGONAL,       // �밢�� ����
    EIGHT_WAY,      // 8���� ����
};

struct TileInfo
{
    bool            bVisited;       // bfs �湮 ����
    CObject*        ObjOnTile;      // Ÿ�� ���� �ִ� ������Ʈ (ĳ���ʹ� ����)
    TileInfo(int x) : bVisited(false), ObjOnTile(nullptr) {};
};

class CMonster;

class CScene_Battle :
    public CScene
{
private:
    TURN_TYPE               m_CurrentTurn;      // ���� �� ��Ȳ

    vector<vector<TileInfo>>    m_vecTileInfo;     // Ÿ���� ������
    map<Vec2, Vec2>             m_mapRealPoint;    // ��ǥ�� ��ǥ -> ���� ��ǥ (���߸�)
    map<Vec2, Vec2>             m_mapGridPoint;    // ���� ��ǥ -> ��ǥ�� ��ǥ (���߸�)

    Vec2                    m_vPlayerPos;       // �÷��̾��� ���� ��ġ
    Vec2                    m_vSelectTile;      // ���� �÷��̾ �������� Ÿ��
    TILE_STATE              m_TileColor;        // ù��° �������� � ���� ����?

    list<Vec2>              m_lstTile;          // �÷��̾ ������ Ÿ�� ����Ʈ(�̵� ����)
    list<CMonster*>         m_lstTargetEnemy;   // BFS�� Ž���� ���� Ÿ�ٵ� ����Ʈ

public:
    CScene_Battle();
    ~CScene_Battle();

public:
    void PlayerStart();

    void TileSelectTrigger(CObject* _pObj);
    void TileSelectInit();
    void PlayerMoveInit();

    void PlayerMove();
    void PlayerAttack(CObject* _pPlayer, CObject* _pEnmey);
    bool IsListTileEmpty();

public:
    void EnemyStart();

public:
    bool isValid(Vec2 _vPos);
    void BFS(Vec2 _startPos, DIRECTION _dir, int _depth);
    void BFSInit();

public:
    virtual void Update();
    virtual void Enter();
    virtual void Exit();
};


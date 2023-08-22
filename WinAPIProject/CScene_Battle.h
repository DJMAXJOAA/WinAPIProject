#pragma once
#include "CScene.h"

enum class TURN_TYPE
{
    ENTER,                  // 전투 입장
    PLAYER_START,           // 플레이어 :: 첫 블럭 선택 전
    PLAYER_BLOCKSELECT,     // 플레이어 :: 블럭 선택중
    PLAYER_MOVE,            // 플레이어 :: 선택 확정하고, 움직이는중
    PLAYER_SKILL,           // 플레이어 :: 움직임이 끝나고, 스킬 시전중
    ENEMY_MOVE,             // 적 :: 움직임 계산 + 움직임
    ENEMY_ATTACK,           // 적 :: 움직임이 끝나고, 플레이어 공격 혹은 스킬
    EXIT,                   // 전투 종료
};

struct Node
{
    Vec2            vPos;           // 실제 좌표
    vector<Node*>   vecNeighbor;    // 해당 노드의 이웃
};

class CScene_Battle :
    public CScene
{
private:
    TURN_TYPE               m_CurrentTurn;      // 현재 턴 상황

    vector<vector<int>>     m_vecTileState;     // 타일의 정보들
    map<Vec2, Vec2>         m_mapPoint;         // 좌표 몇몇의 실제 좌표는? (이동시 필요)

    Vec2                    m_vPlayerPos;       // 플레이어의 현재 위치
    Vec2                    m_vSelectTile;      // 현재 플레이어가 선택중인 타일
    TILE_STATE              m_TileColor;        // 첫번째 선택으로 어떤 색깔 선택?

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


#pragma once
#include "CScene.h"

enum class TURN_TYPE
{
    PLAYER_START,       // 1. 플레이어가 타일을 선택하기 전
    PLAYER_SELECT,      // 2. 플레이어가 타일을 선택중
    PLAYER_MOVE,        // 3. 플레이어가 이동중
    PLAYER_ATTACK,      // 4. 플레이어가 공격중
    ENEMY_MOVE,         // 5. 적군이 이동+공격중
};

class CScene_Battle :
    public CScene
{
private:
    TURN_TYPE               m_CurrentTurn;      // 현재 턴 상황

    vector<vector<int>>     m_vecTileState;     // 타일 상황
    map<Vec2, Vec2>         m_mapPoint;         // 좌표 몇몇의 실제 좌표는? (이동시 필요)

    Vec2                    m_vPlayerPos;       // 플레이어의 현재 위치
    Vec2                    m_vSelectTile;      // 현재 플레이어가 선택중인 타일

public:
    CScene_Battle();
    ~CScene_Battle();

public:
    virtual void Update();
    virtual void Enter();
    virtual void Exit();
};


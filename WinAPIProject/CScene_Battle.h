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

enum class DIRECTION
{
    FOUR_WAY,       // 상하좌우 방향
    DIAGONAL,       // 대각선 방향
    EIGHT_WAY,      // 8방향 전부
};

struct TileInfo
{
    bool            bVisited;       // bfs 방문 여부
    CObject*        ObjOnTile;      // 타일 위에 있는 오브젝트 (캐릭터는 제외)
    TileInfo(int x) : bVisited(false), ObjOnTile(nullptr) {};
};

class CMonster;

class CScene_Battle :
    public CScene
{
private:
    TURN_TYPE               m_CurrentTurn;      // 현재 턴 상황

    vector<vector<TileInfo>>    m_vecTileInfo;     // 타일의 정보들
    map<Vec2, Vec2>             m_mapRealPoint;    // 좌표계 좌표 -> 실제 좌표 (이중맵)
    map<Vec2, Vec2>             m_mapGridPoint;    // 실제 좌표 -> 좌표계 좌표 (이중맵)

    Vec2                    m_vPlayerPos;       // 플레이어의 현재 위치
    Vec2                    m_vSelectTile;      // 현재 플레이어가 선택중인 타일
    TILE_STATE              m_TileColor;        // 첫번째 선택으로 어떤 색깔 선택?

    list<Vec2>              m_lstTile;          // 플레이어가 선택한 타일 리스트(이동 순서)
    list<CMonster*>         m_lstTargetEnemy;   // BFS로 탐색한 공격 타겟들 리스트

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


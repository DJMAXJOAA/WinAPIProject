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
    // 플레이어 현재 정보들
    CPlayer*    pPlayer;        // 플레이어 객체를 따로 관리
    Vec2        vSelectTile;    // 현재 선택중인 타일 좌표
    list<Vec2>  lstMoveRoute;   // 타일 이동 경로
    TILE_STATE  TileColor;      // 현재 선택중인 타일 색상
    PlayerState() : pPlayer(nullptr) {};
};

class CScene_Battle :
    public CScene
{
private:
    PlayerState             m_Player;

    BFSSearch*              m_BFS;              // BFS로 격자맵 경로 탐색
    CMonsterFactory*        m_MonsterFactory;   // 몬스터 관리
    CTurnManager*           m_TurnManager;      // 턴 세부 행동 관리
    CTileManager*           m_TileManager;      // 타일 관리

public:
    CScene_Battle();
    ~CScene_Battle();

public:
    // 이벤트 호출 관련 함수
    void TileSelectTrigger(CObject* _pObj);

public:
    void TurnInit(TURN_TYPE _type);

public:
    virtual void Update();
    virtual void Enter();
    virtual void Exit();
};


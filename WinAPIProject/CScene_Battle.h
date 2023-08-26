#pragma once
#include "CScene.h"
#include "tile.h"

class CPlayer;
class BFSSearch;
class CMonsterSpawner;
class CTurnCenter;
class CTileCenter;

bool CompareGridPos(CObject* _pObj1, CObject* _pObj2);	// 렌더링 순서 정렬용 비교 함수

class CScene_Battle :
    public CScene
{
private:
    int                     m_iFieldType;       // 필드의 생성 타입 ID

    CPlayer*                m_pPlayer;          // 플레이어 객체를 따로 관리

    BFSSearch*              m_BFS;              // BFS로 격자맵 경로 탐색
    CMonsterSpawner*        m_MonsterSpawner;   // 몬스터 관리
    CTurnCenter*            m_TurnCenter;       // 턴 세부 행동 관리
    CTileCenter*            m_TileCenter;       // 타일 관리

public:
    CScene_Battle();
    ~CScene_Battle();

public:
    // 이벤트 호출 관련 처리 함수
    void TileSelectTrigger(CObject* _pObj);
    void TurnInit(TURN_TYPE _type);
    void InitField(int _level, FIELD_TYPE _type);    // 여기를 Enter로 사용

public:
    // 로직 관련
    void TurnLogic();
    void PlayerMove();
    void PlayerAttack();

public:
    // 배틀 씬에서만 사용하는 유틸리티
    Vec2 GetRandomGridPos();
    //bool CompareGridPos(CObject* _pObj1, CObject* _pObj2);	// 렌더링 순서 정렬용 비교 함수


public:
    virtual void Update() override;
    virtual void Enter() override;
    virtual void Exit() override;
};


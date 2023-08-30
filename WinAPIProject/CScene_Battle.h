#pragma once
#include "CScene.h"
#include "tile.h"

class CPlayer;
class CMonster;

class BFSSearch;
class AstarSearch;
class CMonsterSpawner;
class CTurnCenter;
class CTileCenter;

class CBattleState;

bool CompareGridPos(CObject* _pObj1, CObject* _pObj2);	// 렌더링 순서 정렬용 비교 함수

class CScene_Battle :
    public CScene
{
private:
    int                     m_iFieldType;       // 필드의 생성 타입 ID
    int                     m_iDifficulty;
    FIELD_TYPE              m_FieldType;        

    CPlayer*                m_pPlayer;          // 플레이어 객체를 따로 관리
    vector<CBattleState*>   m_vecStates;        // 스테이트 패턴들

    BFSSearch*              m_BFS;              // BFS로 격자맵 경로 탐색
    AstarSearch*            m_Astar;            // A*로 최단경로 탐색
    CMonsterSpawner*        m_MonsterSpawner;   // 몬스터 관리
    CTurnCenter*            m_TurnCenter;       // 조건에 따라 턴 전환, 턴 관련 변수들
    CTileCenter*            m_TileCenter;       // 타일 관리
    CBattleState*           m_BattleState;      // 배틀씬 로직 상태 머신

public:
    CScene_Battle();
    ~CScene_Battle();

public:
    int GetDifficulty() { return m_iDifficulty; }
    FIELD_TYPE GetFieldType() { return m_FieldType; }
    CPlayer* GetPlayer() { return m_pPlayer; }
    BFSSearch* GetBFS() { return m_BFS; }
    AstarSearch* GetAstar() { return m_Astar; }
    CMonsterSpawner* GetSpawner() { return m_MonsterSpawner; }
    CTurnCenter* GetTurnCenter() {return m_TurnCenter; }
    CTileCenter* GetTileCenter() { return m_TileCenter; }

    void SetPlayer(CPlayer* _pPlayer) { m_pPlayer = _pPlayer; }
    void SetFieldType(int _iID) { m_iFieldType = _iID; }
    void SetBattleState(TURN_TYPE _type) { m_BattleState = m_vecStates[(int)_type]; }

public:
    // 이벤트 호출 관련 처리 함수
    void TileSelectTrigger(CObject* _pObj);
    void TurnInit(TURN_TYPE _type);

    void PlayerAttackMonster(float _damage, CMonster* _pMon);
    void PlayerAttackDone();
    void PlayerSkillCasted(float _value);
    void PlayerSkillDone();

    void MonsterAttackPlayer(float _damage);
    void MonsterAttackDone(CMonster* _pMon);

    void MonsterDied(CMonster* _pObj);
    void PlayerDied();

    virtual void CameraEvent() override;

public:
    virtual void Update() override;
    virtual void Enter() override;
    virtual void Exit() override;
};


#pragma once
#include "CScene.h"
#include "tile.h"

class CPlayer;
class CMonster;
class CPanelUI_Number;

class CMonsterSpawner;
class CEventCenter;
class CTurnCenter;
class CTileCenter;
class CBattleState;
class CSound;

class CScene_Battle :
    public CScene
{
private:
    int                     m_iFieldType;       // 필드의 생성 타입 ID
    int                     m_iDifficulty;      // 필드의 난이도
    FIELD_TYPE              m_FieldType;        // 필드의 방 타입
    vector<CSound*>         m_vecSoundEffect;   // 사운드 목록
    vector<CBattleState*>   m_vecStates;        // 스테이트 패턴들

    CPlayer*                m_pPlayer;          // 플레이어 객체를 따로 관리
    CEventCenter*           m_pEventCenter;     // 이벤트 함수 처리
    CMonsterSpawner*        m_pMonsterSpawner;  // 몬스터 관리
    CTurnCenter*            m_pTurnCenter;      // 조건에 따라 턴 전환, 턴 관련 변수들
    CTileCenter*            m_pTileCenter;      // 타일 관리
    CBattleState*           m_pBattleState;     // 배틀씬 로직 상태 머신

    CPanelUI_Number*        m_pMoneyUI;         // 돈 표시용 (임시 UI)
    CPanelUI_Number*        m_pComboUI;         // 콤보 표시용 (임시 UI)

public:
    CScene_Battle();
    ~CScene_Battle();

public:
    int GetDifficulty() { return m_iDifficulty; }
    FIELD_TYPE GetFieldType() { return m_FieldType; }
    CPlayer* GetPlayer() { return m_pPlayer; }
    vector<CSound*> GetSoundEffect() { return m_vecSoundEffect; }

    CMonsterSpawner* GetSpawner() { return m_pMonsterSpawner; }
    CTurnCenter* GetTurnCenter() {return m_pTurnCenter; }
    CTileCenter* GetTileCenter() { return m_pTileCenter; }

    void SetPlayer(CPlayer* _pPlayer) { m_pPlayer = _pPlayer; }
    void SetMoneyUI(CPanelUI_Number* _pUI) { m_pMoneyUI = _pUI; }
    void SetComboUI(CPanelUI_Number* _pUI) { m_pComboUI = _pUI; }
    void SetFieldType(int _iID) { m_iFieldType = _iID; }
    void SetBattleState(TURN_TYPE _type) { m_pBattleState = m_vecStates[(int)_type]; }

public:
    // 이벤트 호출 관련 처리 함수
    void OnTileSelect(CObject* _pObj);
    void OnChangeTurn(TURN_TYPE _type);

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

private:
    void InputKey();
    void SortGroupObj();
};


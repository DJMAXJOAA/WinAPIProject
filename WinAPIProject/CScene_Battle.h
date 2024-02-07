#pragma once
#include "CScene.h"
#include "tile.h"

class CPlayer;
class CMonster;
class CPanelUI_Number;

class CMonsterSpawner;
class CEventCenter;
class CLogicCenter;
class CTileCenter;
class CBattleStateMachine;

class CSound;

class CScene_Battle :
    public CScene
{
private:
    int                     m_iFieldType;       // 필드의 생성 타입 ID
    int                     m_iDifficulty;      // 필드의 난이도
    FIELD_TYPE              m_FieldType;        // 필드의 방 타입
    vector<CSound*>         m_vecSoundEffect;   // 사운드 목록

    CPlayer*                m_pPlayer;          // 플레이어 객체를 따로 관리
    CEventCenter*           m_pEventCenter;     // 이벤트 함수 처리
    CMonsterSpawner*        m_pMonsterSpawner;  // 몬스터 관리
    CLogicCenter*            m_pTurnCenter;      // 조건에 따라 턴 전환, 턴 관련 변수들
    CTileCenter*            m_pTileCenter;      // 타일 관리
    CBattleStateMachine*    m_pStateMachine;    // 배틀씬 로직 상태 머신

    CPanelUI_Number*        m_pMoneyUI;         // 돈 표시용 (임시 UI)
    CPanelUI_Number*        m_pComboUI;         // 콤보 표시용 (임시 UI)

public:
    CScene_Battle();
    virtual ~CScene_Battle();

public:
    int GetDifficulty() { return m_iDifficulty; }
    FIELD_TYPE GetFieldType() { return m_FieldType; }
    vector<CSound*> GetSoundEffect() { return m_vecSoundEffect; }

    CPlayer* GetPlayer() { return m_pPlayer; }
    CMonsterSpawner* GetSpawner() { return m_pMonsterSpawner; }
    CLogicCenter* GetTurnCenter() {return m_pTurnCenter; }
    CTileCenter* GetTileCenter() { return m_pTileCenter; }
    CBattleStateMachine* GetStateMachine() { return m_pStateMachine; }

    void SetPlayer(CPlayer* _pPlayer) { m_pPlayer = _pPlayer; }
    void SetMoneyUI(CPanelUI_Number* _pUI) { m_pMoneyUI = _pUI; }
    void SetComboUI(CPanelUI_Number* _pUI) { m_pComboUI = _pUI; }
    void SetFieldType(int _iID) { m_iFieldType = _iID; }

public:
    // 이벤트 호출 관련 처리 함수
    void OnTileSelect(CObject* _pObj);

    void PlayerAttackMonster(float _damage, CMonster* _pMon);
    void PlayerAttackDone();
    void PlayerSkillCasted(float _value);
    void PlayerSkillDone();

    void MonsterAttackPlayer(float _damage);
    void MonsterAttackDone(CMonster* _pMon);

    void MonsterDied(CMonster* _pObj);
    void PlayerDied();

public:
    void ChangeTurn(TURN_TYPE _type);

public:
    virtual void CameraEvent();

    virtual void Update() override;
    virtual void Enter() override;
    virtual void Exit() override;

private:
    void InputKey();
    void SortGroupObj();
};


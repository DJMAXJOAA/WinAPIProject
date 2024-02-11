#pragma once
#include "CScene.h"

class CPlayer;
class CPanelUI_Number;

class CMonsterSpawner;
class CEventCenter;
class CLogicCenter;
class CTileCenter;
class CBattleStateMachine;

class CScene_Battle :
    public CScene
{
private:
    int                     m_iFieldType;       // �ʵ��� ���� Ÿ�� ID
    int                     m_iDifficulty;      // �ʵ��� ���̵�
    FIELD_TYPE              m_FieldType;        // �ʵ��� �� Ÿ��

    CPlayer*                m_pPlayer;          // �÷��̾�
    CEventCenter*           m_pEventCenter;     // �̺�Ʈ ó��
    CMonsterSpawner*        m_pMonsterSpawner;  // ���� ����
    CLogicCenter*           m_pLogicCenter;     // ���� ���� ����
    CTileCenter*            m_pTileCenter;      // �� Ÿ�� ����
    CBattleStateMachine*    m_pStateMachine;    // ���� ���� �ӽ�

    CPanelUI_Number*        m_pMoneyUI;         // �� ǥ�ÿ� (�ӽ� UI)
    CPanelUI_Number*        m_pComboUI;         // �޺� ǥ�ÿ� (�ӽ� UI)

public:
    CScene_Battle();
    virtual ~CScene_Battle();

public:
    int GetDifficulty() { return m_iDifficulty; }
    FIELD_TYPE GetFieldType() { return m_FieldType; }

    CPlayer* GetPlayer() { return m_pPlayer; }
    CMonsterSpawner* GetSpawner() { return m_pMonsterSpawner; }
    CLogicCenter* GetLogicCenter() {return m_pLogicCenter; }
    CTileCenter* GetTileCenter() { return m_pTileCenter; }
    CEventCenter* GetEventCenter() { return m_pEventCenter; }
    CBattleStateMachine* GetStateMachine() { return m_pStateMachine; }

    void SetPlayer(CPlayer* _pPlayer) { m_pPlayer = _pPlayer; }
    void SetMoneyUI(CPanelUI_Number* _pUI) { m_pMoneyUI = _pUI; }
    void SetComboUI(CPanelUI_Number* _pUI) { m_pComboUI = _pUI; }
    void SetFieldType(int _iID) { m_iFieldType = _iID; }

public:
    void ChangeTurn(TURN_TYPE _type);

public:
    virtual void CameraEvent();

    virtual void Update() override;
    virtual void Enter() override;
    virtual void Exit() override;

private:
    void CheatKey();
    void SortGroupObj();
};


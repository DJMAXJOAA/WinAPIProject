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
    int                     m_iFieldType;       // �ʵ��� ���� Ÿ�� ID
    int                     m_iDifficulty;      // �ʵ��� ���̵�
    FIELD_TYPE              m_FieldType;        // �ʵ��� �� Ÿ��
    vector<CSound*>         m_vecSoundEffect;   // ���� ���

    CPlayer*                m_pPlayer;          // �÷��̾� ��ü�� ���� ����
    CEventCenter*           m_pEventCenter;     // �̺�Ʈ �Լ� ó��
    CMonsterSpawner*        m_pMonsterSpawner;  // ���� ����
    CLogicCenter*            m_pTurnCenter;      // ���ǿ� ���� �� ��ȯ, �� ���� ������
    CTileCenter*            m_pTileCenter;      // Ÿ�� ����
    CBattleStateMachine*    m_pStateMachine;    // ��Ʋ�� ���� ���� �ӽ�

    CPanelUI_Number*        m_pMoneyUI;         // �� ǥ�ÿ� (�ӽ� UI)
    CPanelUI_Number*        m_pComboUI;         // �޺� ǥ�ÿ� (�ӽ� UI)

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
    // �̺�Ʈ ȣ�� ���� ó�� �Լ�
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


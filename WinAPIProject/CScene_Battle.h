#pragma once
#include "CScene.h"
#include "tile.h"

enum class SOUND_TYPE
{
    WIN,
    LOSE,

    END,
};

class CPlayer;
class CMonster;
class CPanelUI_Number;

class BFSSearch;
class AstarSearch;
class CMonsterSpawner;
class CTurnCenter;
class CTileCenter;
class CBattleState;
class CSound;

bool CompareGridPos(CObject* _pObj1, CObject* _pObj2);	// ������ ���� ���Ŀ� �� �Լ�

class CScene_Battle :
    public CScene
{
private:
    int                     m_iFieldType;       // �ʵ��� ���� Ÿ�� ID
    int                     m_iDifficulty;      // �ʵ��� ���̵�
    FIELD_TYPE              m_FieldType;        // �ʵ��� �� Ÿ��
    vector<CSound*>         m_vecSoundEffect;   // ���� ���
    vector<CBattleState*>   m_vecStates;        // ������Ʈ ���ϵ�

    CPlayer*                m_pPlayer;          // �÷��̾� ��ü�� ���� ����
    CPanelUI_Number*        m_pMoneyUI;         // �� ǥ�ÿ� (�ӽ� UI)
    CPanelUI_Number*        m_pComboUI;         // �޺� ǥ�ÿ� (�ӽ� UI)

    BFSSearch*              m_BFS;              // BFS�� ���ڸ� ��� Ž��
    AstarSearch*            m_Astar;            // A*�� �ִܰ�� Ž��
    CMonsterSpawner*        m_MonsterSpawner;   // ���� ����
    CTurnCenter*            m_TurnCenter;       // ���ǿ� ���� �� ��ȯ, �� ���� ������
    CTileCenter*            m_TileCenter;       // Ÿ�� ����
    CBattleState*           m_BattleState;      // ��Ʋ�� ���� ���� �ӽ�

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
    void SetMoneyUI(CPanelUI_Number* _pUI) { m_pMoneyUI = _pUI; }
    void SetComboUI(CPanelUI_Number* _pUI) { m_pComboUI = _pUI; }
    void SetFieldType(int _iID) { m_iFieldType = _iID; }
    void SetBattleState(TURN_TYPE _type) { m_BattleState = m_vecStates[(int)_type]; }

public:
    // �̺�Ʈ ȣ�� ���� ó�� �Լ�
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


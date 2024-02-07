#pragma once

class CScene_Battle;
class CBattleState;
class CLogicCenter;

class CBattleStateMachine
{
private:
	CScene_Battle*			m_pScene;

	vector<CBattleState*>   m_vecStates;        // ������Ʈ ���ϵ�
	CBattleState*			m_pBattleState;		// ���� ����

public:
	CBattleStateMachine(CScene_Battle* _pScene);
	~CBattleStateMachine();

public:
	CBattleState* GetState() { return m_pBattleState; }

	void ChangeState(TURN_TYPE _type);

public:
	void Handle(CScene_Battle* _pScene);
};


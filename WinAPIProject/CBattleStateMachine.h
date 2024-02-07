#pragma once

class CScene_Battle;
class CBattleState;
class CLogicCenter;

class CBattleStateMachine
{
private:
	CScene_Battle*			m_pScene;

	vector<CBattleState*>   m_vecStates;        // 스테이트 패턴들
	CBattleState*			m_pBattleState;		// 현재 상태

public:
	CBattleStateMachine(CScene_Battle* _pScene);
	~CBattleStateMachine();

public:
	CBattleState* GetState() { return m_pBattleState; }

	void ChangeState(TURN_TYPE _type);

public:
	void Handle(CScene_Battle* _pScene);
};


#pragma once

class AI;

class CState
{
private:
	AI*			m_pAI;
	MON_STATE	m_eState;

public:
	CState(MON_STATE _eState);		// 기본 생성자가 없음 -> 자식이 State 생성자를 지정해줘야 한다
	virtual ~CState();

	friend class AI;

public:
	AI* GetAI() { return m_pAI; }
	MON_STATE GetType() { return m_eState; }

public:
	virtual void Update() = 0;
	virtual void Enter() = 0;
	virtual void Exit() = 0;		// 인터페이스화
};


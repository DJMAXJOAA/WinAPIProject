#pragma once

class CMonster;
class CState;

class AI
{
private:
	map<MON_STATE, CState*>	m_mapState;		// 배열로 해도 노상관
	CState*					m_pCurState;
	CMonster*				m_pOwner;

public:
	AI();
	~AI();

	friend class CMonster;

public:
	CState* GetState(MON_STATE _eState);

public:
	void AddState(CState* _pState);
	void SetCurState(MON_STATE _eState);

public:
	void Update();
};


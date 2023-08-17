#pragma once

class CMonster;
class CState;

class AI
{
private:
	map<MON_STATE, CState*>	m_mapState;		// �迭�� �ص� ����
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


#pragma once

class CObject;

struct tEvent
{
	EVENT_TYPE	eEvent;
	DWORD_PTR	lParam;
	DWORD_PTR	wParam;
};

class CEventMgr
{
	SINGLE(CEventMgr);
private:
	vector<tEvent> m_vecEvent;
	vector<tEvent> m_vecLateEvent;	// ���� �ڿ� �߰��ϴ� �̺�Ʈ ���
	vector<tEvent> m_vecEarlyEvent;	// ���� �տ� �߰��ϴ� �̺�Ʈ ���

	vector<CObject*> m_vecDead; // ���� ���� ������Ʈ, �� ���� �����ӿ� ����(�����ϰ� �����ϱ� ���� ��������)

private:
	CEventMgr();
	~CEventMgr();

public:
	void AddEvent(const tEvent& _eve) { m_vecEvent.push_back(_eve); }
	void AddEventEarly(const tEvent& _eve) { m_vecEarlyEvent.push_back(_eve); }
	void AddEventLate(const tEvent& _eve) { m_vecLateEvent.push_back(_eve); }

public:
	void Update();

private:
	void Excute(const tEvent& _eve);		// ���� �̺�Ʈ ó�� �Լ�
};


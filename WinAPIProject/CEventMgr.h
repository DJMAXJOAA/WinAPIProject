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

	vector<CObject*> m_vecDead; // 삭제 예정 오브젝트, 그 다음 프레임에 삭제(안전하게 제거하기 위한 스레기통)

private:
	CEventMgr();
	~CEventMgr();

public:
	void AddEvent(const tEvent& _eve) { m_vecEvent.push_back(_eve); }

public:
	void Update();

private:
	void Excute(const tEvent& _eve);		// 쌓인 이벤트 처리 함수
};


#pragma once

class CTimeMgr
{
	SINGLE(CTimeMgr);

private:
	LARGE_INTEGER	m_liCurCount;
	LARGE_INTEGER	m_liPrevCount;
	LARGE_INTEGER	m_liFrequency;

	double			m_dDT;	// 한 프레임당 시간값
	double			m_dAcc;	// 누적 시간
	UINT			m_iCallCount;
	UINT			m_iFPS;

private:
	CTimeMgr();
	~CTimeMgr();

public:
	void Init();
	void Update();
	void Render();

public:
	double GetDT() { return m_dDT; }
	float GetfDT() { return (float)m_dDT; }
};
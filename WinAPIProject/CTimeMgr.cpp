#include "pch.h"
#include "CTimeMgr.h"
#include "CCore.h"

void CTimeMgr::Init()
{
	// 현재 카운트 횟수
	QueryPerformanceCounter(&m_liCurCount);

	// 초당 카운트 횟수
	QueryPerformanceFrequency(&m_liFrequency);
}

void CTimeMgr::Update()
{
	QueryPerformanceCounter(&m_liCurCount);

	// 이전 프레임의 카운팅과, 현재 프레임의 카운팅 값의 차이를 구한다
	m_dDT = (double)(m_liCurCount.QuadPart - m_liPrevCount.QuadPart) /
		(double)m_liFrequency.QuadPart;

	// 이전카운트 값을 현재 카운트값으로 갱신(다음 계산 위해)
	m_liPrevCount = m_liCurCount;

//#ifdef _DEBUG
//	if (m_dDT > (1./60.))
//		m_dDT = (1./60.);
//#endif
}

void CTimeMgr::Render()
{
	++m_iCallCount;
	m_dAcc += m_dDT; // DT 누적

	if (m_dAcc >= 1.) // 1초마다 한번 체크
	{
		m_iFPS = m_iCallCount;
		m_dAcc = 0.;
		m_iCallCount = 0;

		wchar_t szBuffer[255] = {};
		swprintf_s(szBuffer, L"FPS : %d, DT : %f", m_iFPS, m_dDT);
		SetWindowText(CCore::GetInstance()->GetMainhWnd(), szBuffer); // 타이틀 지정
	}
}

CTimeMgr::CTimeMgr()
	: m_liCurCount{}
	, m_liFrequency{}
	, m_liPrevCount{}
	, m_dDT(0.)
	, m_dAcc(0.)
	, m_iCallCount(0)
	, m_iFPS(0)
{
}

CTimeMgr::~CTimeMgr()
{
}
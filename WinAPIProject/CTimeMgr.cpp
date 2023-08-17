#include "pch.h"
#include "CTimeMgr.h"
#include "CCore.h"

void CTimeMgr::Init()
{
	// ���� ī��Ʈ Ƚ��
	QueryPerformanceCounter(&m_liCurCount);

	// �ʴ� ī��Ʈ Ƚ��
	QueryPerformanceFrequency(&m_liFrequency);
}

void CTimeMgr::Update()
{
	QueryPerformanceCounter(&m_liCurCount);

	// ���� �������� ī���ð�, ���� �������� ī���� ���� ���̸� ���Ѵ�
	m_dDT = (double)(m_liCurCount.QuadPart - m_liPrevCount.QuadPart) /
		(double)m_liFrequency.QuadPart;

	// ����ī��Ʈ ���� ���� ī��Ʈ������ ����(���� ��� ����)
	m_liPrevCount = m_liCurCount;

//#ifdef _DEBUG
//	if (m_dDT > (1./60.))
//		m_dDT = (1./60.);
//#endif
}

void CTimeMgr::Render()
{
	++m_iCallCount;
	m_dAcc += m_dDT; // DT ����

	if (m_dAcc >= 1.) // 1�ʸ��� �ѹ� üũ
	{
		m_iFPS = m_iCallCount;
		m_dAcc = 0.;
		m_iCallCount = 0;

		wchar_t szBuffer[255] = {};
		swprintf_s(szBuffer, L"FPS : %d, DT : %f", m_iFPS, m_dDT);
		SetWindowText(CCore::GetInstance()->GetMainhWnd(), szBuffer); // Ÿ��Ʋ ����
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
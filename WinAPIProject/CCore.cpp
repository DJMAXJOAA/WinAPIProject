#include "pch.h"
#include "CCore.h"

#include "CTimeMgr.h"
#include "CKeyMgr.h"
#include "CSceneMgr.h"
#include "CPathMgr.h"
#include "CCollisionMgr.h"
#include "CEventMgr.h"
#include "CCamera.h"
#include "CUIMgr.h"
#include "CResMgr.h"
#include "CDataMgr.h"

#include "CTexture.h"
#include "CObject.h"

ULONG_PTR g_gdiplusToken;

CCore::CCore()
	: m_hWnd(0)
	, m_ptResolution{}
	, m_hDC(0)
	, m_arrBrush{}
	, m_arrPen{}
	, m_pMemTex(nullptr)
	, m_graphics(nullptr)
{
}

CCore::~CCore()
{
	ReleaseDC(m_hWnd, m_hDC);
	delete m_graphics;

	// ���ֻ���ϴ� �� ����ߴ��� �����ֱ�
	for (int i = 0; i < (UINT)PEN_TYPE::END; i++)
	{
		DeleteObject(m_arrPen[i]);
	}

	GdiplusShutdown(g_gdiplusToken);
}

void CCore::CreateBrushPen()
{
	// Hollow Brush
	m_arrBrush[(UINT)BRUSH_TYPE::HOLLOW] = (HBRUSH)GetStockObject(HOLLOW_BRUSH); // �̹� ���ǵǾ� �ִ� �귯�� -> �ҷο� �귯��(���� �ʿ����)

	// RGB Pen
	m_arrPen[(UINT)PEN_TYPE::RED] = CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
	m_arrPen[(UINT)PEN_TYPE::GREEN] = CreatePen(PS_SOLID, 1, RGB(0, 255, 0));
	m_arrPen[(UINT)PEN_TYPE::BLUE] = CreatePen(PS_SOLID, 1, RGB(0, 0, 255));
}


int CCore::Init(HWND hWnd, POINT ptResolution)
{
	GdiplusStartupInput gdiplusStartupInput;
	GdiplusStartup(&g_gdiplusToken, &gdiplusStartupInput, NULL);

	m_hWnd = hWnd;
	m_ptResolution = ptResolution;

	// �ػ󵵿� �°� ������ ũ�� ����
	RECT rt = { 0, 0, m_ptResolution.x, m_ptResolution.y };
	AdjustWindowRect(&rt, WS_OVERLAPPEDWINDOW, true);
	SetWindowPos(m_hWnd, nullptr, 100, 100, rt.right - rt.left, rt.bottom - rt.top, 0);

	m_hDC = GetDC(m_hWnd);

	// ���� ���۸� �뵵�� �ؽ��� ����
	m_pMemTex = CResMgr::GetInstance()->CreateTexture(L"BackBuffer", (UINT)m_ptResolution.x, (UINT)m_ptResolution.y);

	// ���� ���Ǵ� �귯�ÿ� �� �迭�� ����
	CreateBrushPen();

	m_graphics = new Graphics(m_pMemTex->GetDC());

	// Manager �ʱ�ȭ
	CPathMgr::GetInstance()->Init();
	CTimeMgr::GetInstance()->Init();
	CKeyMgr::GetInstance()->Init();
	CCamera::GetInstance()->Init();		// ���Ŵ������� ����
	CSceneMgr::GetInstance()->Init();

	return S_OK;
}

void CCore::Progress()
{
	// �Ŵ��� ������Ʈ================================================
	CTimeMgr::GetInstance()->Update();
	CKeyMgr::GetInstance()->Update();
	CCamera::GetInstance()->Update();

	// �� ������Ʈ====================================================
	CSceneMgr::GetInstance()->Update();
	CCollisionMgr::GetInstance()->Update();
	CUIMgr::GetInstance()->Update();

	// ������ ����====================================================
	Rectangle(m_pMemTex->GetDC(), -1, -1, m_ptResolution.x + 1, m_ptResolution.y + 1); // ȭ�� Ŭ����

	CSceneMgr::GetInstance()->Render(m_pMemTex->GetDC()); // ȭ�� �׸���
	CCamera::GetInstance()->Render(m_pMemTex->GetDC());

	// �ι�° ���ۿ��� ù��° ����(������ ǥ�õǴ� ȭ��)�� ����->�ٿ��ֱ�
	BitBlt(m_hDC, 0, 0, m_ptResolution.x, m_ptResolution.y, m_pMemTex->GetDC(), 0, 0, SRCCOPY);

	CTimeMgr::GetInstance()->Render();

	// �̺�Ʈ ���� ó��================================================
	// (���������� ��� ��ġ�� ���������� �Ѿ�� ���� ����)=============
	CEventMgr::GetInstance()->Update();
}


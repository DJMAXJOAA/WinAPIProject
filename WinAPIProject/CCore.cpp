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

	// 자주사용하는 펜 등록했던거 지워주기
	for (int i = 0; i < (UINT)PEN_TYPE::END; i++)
	{
		DeleteObject(m_arrPen[i]);
	}

	GdiplusShutdown(g_gdiplusToken);
}

void CCore::CreateBrushPen()
{
	// Hollow Brush
	m_arrBrush[(UINT)BRUSH_TYPE::HOLLOW] = (HBRUSH)GetStockObject(HOLLOW_BRUSH); // 이미 정의되어 있는 브러쉬 -> 할로우 브러쉬(지울 필요없음)

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

	// 해상도에 맞게 윈도우 크기 조정
	RECT rt = { 0, 0, m_ptResolution.x, m_ptResolution.y };
	AdjustWindowRect(&rt, WS_OVERLAPPEDWINDOW, true);
	SetWindowPos(m_hWnd, nullptr, 100, 100, rt.right - rt.left, rt.bottom - rt.top, 0);

	m_hDC = GetDC(m_hWnd);

	// 더블 버퍼링 용도의 텍스쳐 생성
	m_pMemTex = CResMgr::GetInstance()->CreateTexture(L"BackBuffer", (UINT)m_ptResolution.x, (UINT)m_ptResolution.y);

	// 자주 사용되는 브러시와 펜 배열을 생성
	CreateBrushPen();

	m_graphics = new Graphics(m_pMemTex->GetDC());

	// Manager 초기화
	CPathMgr::GetInstance()->Init();
	CTimeMgr::GetInstance()->Init();
	CKeyMgr::GetInstance()->Init();
	CCamera::GetInstance()->Init();		// 씬매니저보다 먼저
	CSceneMgr::GetInstance()->Init();

	return S_OK;
}

void CCore::Progress()
{
	// 매니저 업데이트================================================
	CTimeMgr::GetInstance()->Update();
	CKeyMgr::GetInstance()->Update();
	CCamera::GetInstance()->Update();

	// 씬 업데이트====================================================
	CSceneMgr::GetInstance()->Update();
	CCollisionMgr::GetInstance()->Update();
	CUIMgr::GetInstance()->Update();

	// 렌더링 과정====================================================
	Rectangle(m_pMemTex->GetDC(), -1, -1, m_ptResolution.x + 1, m_ptResolution.y + 1); // 화면 클리어

	CSceneMgr::GetInstance()->Render(m_pMemTex->GetDC()); // 화면 그리기
	CCamera::GetInstance()->Render(m_pMemTex->GetDC());

	// 두번째 버퍼에서 첫번째 버퍼(실제로 표시되는 화면)에 복사->붙여넣기
	BitBlt(m_hDC, 0, 0, m_ptResolution.x, m_ptResolution.y, m_pMemTex->GetDC(), 0, 0, SRCCOPY);

	CTimeMgr::GetInstance()->Render();

	// 이벤트 지연 처리================================================
	// (렌더링까지 모두 마치고 다음프레임 넘어가기 전에 실행)=============
	CEventMgr::GetInstance()->Update();
}


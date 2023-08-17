#pragma once

class CTexture;

class CCore
{
	SINGLE(CCore);
private:
	HWND		m_hWnd;			// 메인 윈도우 핸들
	POINT		m_ptResolution;	// 해상도 정보
	HDC			m_hDC;			// 메인 윈도우에 Draw할 DC

	CTexture* m_pMemTex;		// 백 버퍼 텍스쳐 (더블버퍼링)

	// 자주 사용하는 GDI 오브젝트
	HBRUSH	m_arrBrush[(UINT)BRUSH_TYPE::END];
	HPEN	m_arrPen[(UINT)PEN_TYPE::END];

private:
	CCore();
	~CCore();

public:
	int Init(HWND hWnd, POINT ptResolution);
	void Progress();

public:
	HWND GetMainhWnd() { return m_hWnd; }
	HDC GetMainDC() { return m_hDC; }
	POINT GetResolution() { return m_ptResolution; }
	HBRUSH GetBrush(BRUSH_TYPE _eType) { return m_arrBrush[(UINT)_eType]; }
	HPEN GetPen(PEN_TYPE _eType) { return m_arrPen[(UINT)_eType]; }

private:
	void CreateBrushPen();
};

#pragma once

class CUI;

class CUIMgr
{
	SINGLE(CUIMgr);
private:
	CUI*		m_pFocusedUI;	// 포커싱 된 ui만 움직이게 수정

private:
	CUIMgr();
	~CUIMgr();

private:
	// 부모 UI 내에서 실제로 타겟팅 된 UI를 찾아서 반환(자식 UI들까지 우선 검사)
	CUI* GetFocusedUI();
	CUI* GetTargetedUI(CUI* _pParentUI);

public:
	void SetFocusedUI(CUI* _pUI);

public:
	void Update();
};


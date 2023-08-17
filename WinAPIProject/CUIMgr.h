#pragma once

class CUI;

class CUIMgr
{
	SINGLE(CUIMgr);
private:
	CUI*		m_pFocusedUI;	// ��Ŀ�� �� ui�� �����̰� ����

public:
	CUIMgr();
	~CUIMgr();

private:
	// �θ� UI ������ ������ Ÿ���� �� UI�� ã�Ƽ� ��ȯ(�ڽ� UI����� �켱 �˻�)
	CUI* GetFocusedUI();
	CUI* GetTargetedUI(CUI* _pParentUI);

public:
	void SetFocusedUI(CUI* _pUI);

public:
	void Update();
};


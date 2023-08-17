#include "pch.h"
#include "CUIMgr.h"

#include "CSceneMgr.h"
#include "CKeyMgr.h"

#include "CObject.h"
#include "CScene.h"
#include "CUI.h"

CUIMgr::CUIMgr()
	: m_pFocusedUI(nullptr)
{
}

CUIMgr::~CUIMgr()
{
}

void CUIMgr::Update()
{
	// 1. Focused UI Ȯ���ϱ� (�ΰ��� â�� �Ѵ� update�޾ƹ����� �ȴ�)
	// ������ �ִ� �ָ� ��Ŀ���ϸ� �Ǵ���, ���ο� ui�� ��Ŀ���ؾ��ϴ��� ����
	m_pFocusedUI = GetFocusedUI();

	if (!m_pFocusedUI)
		return;		// �̺�Ʈ�� �� ui�� ����

	// 2. focusui ������ �θ� ui ����, �ڽ� ui�� �߿��� ���� Ÿ���� ui
	CUI* pTargetUI = GetTargetedUI(m_pFocusedUI);

	bool bLbtnTap = KEY_TAP(KEY::LBTN);
	bool bLbtnAway = KEY_AWAY(KEY::LBTN);

	if (pTargetUI != nullptr)
	{
		pTargetUI->MouseOn();

		if (bLbtnTap)
		{
			pTargetUI->MouseLbtnDown();
			pTargetUI->m_bLbtnDown = true;
		}
		else if (bLbtnAway)
		{
			pTargetUI->MouseLbtnUp();

			// UI ��ư ������ ��������, �� �ڿ� UI ��ư ������ �þ����� �߰� ȣ��
			if (pTargetUI->m_bLbtnDown)
			{
				pTargetUI->MouseLbtnClicked();
			}

			pTargetUI->m_bLbtnDown = false;
		}
	}
}

CUI* CUIMgr::GetFocusedUI()
{
	CScene* pCurScene = CSceneMgr::GetInstance()->GetCurScene();
	vector<CObject*>& vecUI = pCurScene->GetUIGroup();

	bool bLbnTap = KEY_TAP(KEY::LBTN);
	if (!bLbnTap)
		return m_pFocusedUI;

	// ���� ��Ŀ�̵� UI�� �⺻������ ��ȯ, ������ ������ �����ؼ� ��ȯ�ϱ�
	CUI* pFocusedUI = m_pFocusedUI;

	// ���� ��ư�� tap�� �߻��ƴٴ� �����Ͽ�, �ڿ������� �˻�(�ڿ����� �켱������ �����ϱ�)
	vector<CObject*>::iterator targetIter = vecUI.end();	// ������ �˻��޴µ� end ���� -> ��Ŀ�� ui�� ����
	vector<CObject*>::iterator iter = vecUI.begin();

	for (; iter != vecUI.end(); ++iter)
	{
		if (((CUI*)*iter)->IsMouseOn())
		{
			targetIter = iter;
		}
	}

	// ���࿡ �ٰ˻��ߴµ� ��Ŀ�� ui�� ������
	if (vecUI.end() == targetIter)
	{
		return nullptr;
	}

	// ��������� �̸� �����ؼ� �޾Ƴ���
	pFocusedUI = (CUI*)*targetIter;

	// ���� ������ �� �ڷ� ���� ��ü -> ���� ���� �켱������
	vecUI.erase(targetIter);
	vecUI.push_back(pFocusedUI);

	return pFocusedUI;
}

CUI* CUIMgr::GetTargetedUI(CUI* _pParentUI)
{
	bool bLbtnAway = KEY_AWAY(KEY::LBTN);

	//// 1. �θ� UI �����ؼ�, ��� �ڽĵ��� �˻��Ѵ� (�ڽ��� �������ϼ���, �θ� �������ϼ��� ����)
	CUI* pTargetUI = nullptr;
	
	// �׻� ����ϴ� �����̳ʵ��̴ϱ�, �̸� ���������� �����ؼ� �������� �����ϰ� �����ٰ� �ݺ��� �ʿ� ���� �������ֱ�
	static list<CUI*> queue;
	static vector<CUI*> vecNoneTarget;

	queue.clear();	
	vecNoneTarget.clear();	// �Ǻ� ���� ���� ���������� �� ��� �ʱ�ȭ

	queue.push_back(_pParentUI);

	//// 2. BFS�� ���ؼ�, ���� ����� UI���� Ÿ���� �˻� (���� ��ȸ ���)
	while (!queue.empty())
	{
		CUI* pUI = queue.front();
		queue.pop_front();

		// ť���� ������ UI�� TargetUI���� Ȯ���Ѵ�=====================
		// Ÿ�� UI���߿� �� �켱������ ���� �������� �� ���� ������ �ڽ� UI
		if (pUI->IsMouseOn())
		{
			if (pTargetUI != nullptr)
			{
				// �ڸ� ����ֵ� �־��ֱ�
				vecNoneTarget.push_back(pTargetUI);
			}
			pTargetUI = pUI;
		}
		else
		{
			vecNoneTarget.push_back(pUI);
		}
		
		// ==========================================================

		const vector<CUI*> vecChild = pUI->GetChildUI();
		for (size_t i = 0; i < vecChild.size(); i++)
		{
			queue.push_back(vecChild[i]);
		}
	}

	if (bLbtnAway)
	{
		for (size_t i = 0; i < vecNoneTarget.size(); i++)
		{
			// Ÿ�� �ƴѾֵ� �̸� ��Ƶΰ� �³׵鸸 ���� ��Ȱ��ȭ ó��
			vecNoneTarget[i]->m_bLbtnDown = false;
		}
	}

	return pTargetUI;
}

void CUIMgr::SetFocusedUI(CUI* _pUI)
{
	// �̹� ��Ŀ�� ���� ui Ȥ�� ��Ŀ�� ���� ��û
	if (m_pFocusedUI == _pUI || _pUI == nullptr)
	{
		m_pFocusedUI = _pUI;
		return;
	}

	m_pFocusedUI = _pUI;

	// �켱���� ���� (���� ����)
	CScene* pCurScene = CSceneMgr::GetInstance()->GetCurScene();
	vector<CObject*>& vecUI = pCurScene->GetUIGroup();

	vector<CObject*>::iterator iter = vecUI.begin();

	for (; iter != vecUI.end(); ++iter)
	{
		if (m_pFocusedUI == *iter)
		{
			break;
		}
	}

	// ���� ������ �� �ڷ� ���� ��ü -> ���� ���� �켱������ ����
	vecUI.erase(iter);
	vecUI.push_back(m_pFocusedUI);
}

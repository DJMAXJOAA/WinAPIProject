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
	// 1. Focused UI 확인하기 (두개의 창이 둘다 update받아버리면 안댐)
	// 기존에 있던 애를 포커싱하면 되는지, 새로운 ui를 포커싱해야하는지 결정
	m_pFocusedUI = GetFocusedUI();

	if (!m_pFocusedUI)
		return;		// 이벤트를 줄 ui가 없음

	// 2. focusui 내에서 부모 ui 포함, 자식 ui들 중에서 실제 타겟팅 ui
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

			// UI 버튼 위에서 눌렀었고, 그 뒤에 UI 버튼 위에서 뗐었으면 추가 호출
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

	// 기존 포커싱된 UI를 기본값으로 반환, 변경점 있으면 수정해서 반환하기
	CUI* pFocusedUI = m_pFocusedUI;

	// 왼쪽 버튼이 tap이 발생됐다는 전제하에, 뒤에서부터 검색(뒤에부터 우선순위가 높으니까)
	vector<CObject*>::iterator targetIter = vecUI.end();	// 끝까지 검색햇는데 end 유지 -> 포커싱 ui가 없음
	vector<CObject*>::iterator iter = vecUI.begin();

	for (; iter != vecUI.end(); ++iter)
	{
		if (((CUI*)*iter)->IsMouseOn())
		{
			targetIter = iter;
		}
	}

	// 만약에 다검사했는데 포커싱 ui가 없으면
	if (vecUI.end() == targetIter)
	{
		return nullptr;
	}

	// 지우기전에 미리 복사해서 받아놓기
	pFocusedUI = (CUI*)*targetIter;

	// 벡터 내에서 맨 뒤로 순번 교체 -> 가장 높은 우선순위로
	vecUI.erase(targetIter);
	vecUI.push_back(pFocusedUI);

	return pFocusedUI;
}

CUI* CUIMgr::GetTargetedUI(CUI* _pParentUI)
{
	bool bLbtnAway = KEY_AWAY(KEY::LBTN);

	//// 1. 부모 UI 포함해서, 모든 자식들을 검사한다 (자식이 여러명일수도, 부모가 여러명일수도 있음)
	CUI* pTargetUI = nullptr;
	
	// 항상 사용하는 컨테이너들이니까, 미리 정적변수로 선언해서 힙영역을 생성하고 지웠다가 반복할 필요 없게 설정해주기
	static list<CUI*> queue;
	static vector<CUI*> vecNoneTarget;

	queue.clear();	
	vecNoneTarget.clear();	// 판별 전에 정적 지역변수들 값 모두 초기화

	queue.push_back(_pParentUI);

	//// 2. BFS를 통해서, 가장 가까운 UI부터 타겟을 검사 (레벨 순회 방식)
	while (!queue.empty())
	{
		CUI* pUI = queue.front();
		queue.pop_front();

		// 큐에서 꺼내온 UI가 TargetUI인지 확인한다=====================
		// 타겟 UI들중에 더 우선순위가 높은 기준으로 더 낮은 계층의 자식 UI
		if (pUI->IsMouseOn())
		{
			if (pTargetUI != nullptr)
			{
				// 자리 뺏긴애도 넣어주기
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
			// 타겟 아닌애들 미리 모아두고 걔네들만 따로 비활성화 처리
			vecNoneTarget[i]->m_bLbtnDown = false;
		}
	}

	return pTargetUI;
}

void CUIMgr::SetFocusedUI(CUI* _pUI)
{
	// 이미 포커싱 중인 ui 혹은 포커싱 해제 요청
	if (m_pFocusedUI == _pUI || _pUI == nullptr)
	{
		m_pFocusedUI = _pUI;
		return;
	}

	m_pFocusedUI = _pUI;

	// 우선순위 변경 (제일 위로)
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

	// 벡터 내에서 맨 뒤로 순번 교체 -> 가장 높은 우선순위로 갱신
	vecUI.erase(iter);
	vecUI.push_back(m_pFocusedUI);
}

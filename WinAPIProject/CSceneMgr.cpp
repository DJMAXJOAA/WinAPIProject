#include "pch.h"
#include "CSceneMgr.h"

#include "CScene_Start.h"
#include "CScene_Tool.h"
#include "CScene_Battle.h"
#include "CScene_Robby.h"

CSceneMgr::CSceneMgr()
	: m_arrScene{}
	, m_pCurScene(nullptr)
{
}

CSceneMgr::~CSceneMgr()
{
	for (UINT i = 0; i < (UINT)SCENE_TYPE::END; i++)
	{
		if (nullptr != m_arrScene[i])
		{
			delete m_arrScene[i];
		}
	}
}

void CSceneMgr::Init()
{
	m_arrScene[(UINT)SCENE_TYPE::START] = new CScene_Start;
	m_arrScene[(UINT)SCENE_TYPE::START]->SetName(L"Start Scene");

	m_arrScene[(UINT)SCENE_TYPE::TOOL] = new CScene_Tool;
	m_arrScene[(UINT)SCENE_TYPE::TOOL]->SetName(L"Tool Scene");

	m_arrScene[(UINT)SCENE_TYPE::BATTLE] = new CScene_Battle;
	m_arrScene[(UINT)SCENE_TYPE::BATTLE]->SetName(L"Battle Scene");

	m_arrScene[(UINT)SCENE_TYPE::ROBBY] = new CScene_Robby;
	m_arrScene[(UINT)SCENE_TYPE::ROBBY]->SetName(L"Robby Scene");
	
	// ÇöÀç ¾À ÀúÀå
	m_pCurScene = m_arrScene[(UINT)SCENE_TYPE::START];
	m_pCurScene->Enter(); // ¾À ÁøÀÔ
}

void CSceneMgr::Update()
{
	// ÇöÀç ¾ÀÀ» ¾÷µ¥ÀÌÆ® ÇØ¾ßµÊ
	m_pCurScene->Update();
	m_pCurScene->FinalUpdate();
}

void CSceneMgr::Render(HDC hdc)
{
	// Àü´Þ
	m_pCurScene->Render(hdc);
}

void CSceneMgr::ChangeScene(SCENE_TYPE _eNext)
{
	m_pCurScene->Exit();
	m_pCurScene = m_arrScene[(UINT)_eNext];

	m_pCurScene->Enter();
}

#include "pch.h"
#include "CScene_Robby.h"

#include "CCore.h"
#include "CKeyMgr.h"
#include "CTimeMgr.h"
#include "CCamera.h"
#include "CSceneMgr.h"

#include "CPanelUI_Back.h"
#include "CBtnUI_Stage.h"

static bool enter;

CScene_Robby::CScene_Robby()
	: m_CameraPos{}
	, m_vecStage{}
	, m_MapGenerator(nullptr)
	, m_mainUI(nullptr)
	, m_mapGridNode{}
	, m_mapBtnUI{}
	, m_pCurrentNode(nullptr)
{
	m_MapGenerator = new CMapGenerator;

	// 맵 생성
	m_vecStage = m_MapGenerator->CreateRandomMap();
	m_vecNodes = m_MapGenerator->CreateStartPos(m_vecStage, m_mapGridNode);
}

CScene_Robby::~CScene_Robby()
{
	delete m_MapGenerator;
	DeleteAll();
}

void CScene_Robby::EnterStage(Vec2 _vPos)
{
	CCamera::GetInstance()->FadeOut(0.5f);
	CCamera::GetInstance()->BlackScreen(1.0f);
	CCamera::GetInstance()->Event(0.01f);
	CCamera::GetInstance()->FadeIn(0.5f);

	// 같은 x들 선택 못하게 삭제
	int x = (int)_vPos.x;
	for (int i = 0; i < m_vecStage.size(); ++i)
	{
		Vec2 pos(x, i);
		if (m_mapBtnUI[pos] != nullptr)
		{
			m_mapBtnUI[pos]->SetSelect(false);
		}
	}

	if (m_mapBtnUI[_vPos] != nullptr)
	{
		m_mapBtnUI[_vPos]->SetSelect(true);
		//m_mapBtnUI[_vPos]->SetClear(true);		// 이건 따로 이벤트로 수정

		Vec2 CameraPos = Vec2(m_mapBtnUI[_vPos]->GetPos().x, m_CameraPos.y);
		CCamera::GetInstance()->SetLookAt(CameraPos);
	}

	auto iter = m_mapGridNode.find(_vPos);
	if (iter == m_mapGridNode.end())
		assert(1);
	else
	{
		m_pCurrentNode = iter->second;
		m_lstRoute.push_back(_vPos);

		for (auto& child : iter->second->children)
		{
			if (child != nullptr)
			{
				m_mapBtnUI[Vec2(child->x, child->y)]->SetSelect(true);
			}
			else
			{
				printf("nullptr");
			}
		}
	}
}

void CScene_Robby::CameraEvent()
{
	ChangeScene(SCENE_TYPE::BATTLE);
}

void CScene_Robby::Render(HDC hdc)
{
	CScene::Render(hdc);
}

void CScene_Robby::Update()
{
	CScene::Update();
	
	// 화면 전환
	if (KEY_TAP(KEY::ENTER))
	{
		ChangeScene(SCENE_TYPE::BATTLE);
	}
	

	if (KEY_HOLD(KEY::LEFT))
	{
		Vec2 vResolution = CCore::GetInstance()->GetResolution();

		m_CameraPos.x -= 1000.f * CTimeMgr::GetInstance()->GetfDT();
		CCamera::GetInstance()->SetLookAt(m_CameraPos);
	}
	if (KEY_HOLD(KEY::RIGHT))
	{
		Vec2 vResolution = CCore::GetInstance()->GetResolution();

		m_CameraPos.x += 1000.f * fDT;
		CCamera::GetInstance()->SetLookAt(m_CameraPos);
	}
}

void CScene_Robby::Enter()
{
	// 최초 1회만 실행
	if (!enter)
	{
		// UI 추가
		Vec2 vResolution = CCore::GetInstance()->GetResolution(); // 화면 크기 가져오기

		const int WidthMargin = 50;
		const int HeightMargin = 50;
		const int extendedWidth = (WIDTH * 2) - 1;
		const int extendedHeight = (HEIGHT * 2) - 1;
		const int PanelWidth = 2400;
		const int PanelHeight = 640;

		CPanelUI_Back* pPanelUI = new CPanelUI_Back;
		pPanelUI->SetName(L"ParentUI");
		pPanelUI->SetScale(Vec2(PanelWidth, PanelHeight));
		pPanelUI->SetPos(Vec2(0, 0));
		m_mainUI = pPanelUI;

		// 패널 내에서 사용 가능한 너비와 높이
		int available_width = PanelWidth - WidthMargin * 2;
		int available_height = PanelHeight - HeightMargin * 2;

		// 일정한 간격으로 오브젝트를 배치하기 위한 간격을 계산합니다.
		int horizontal_gap = available_width / extendedWidth;
		int vertical_gap = available_height / extendedHeight;

		// 2차원 배열을 순회하면서 오브젝트를 생성합니다.
		for (int i = 0; i < extendedWidth; ++i)
		{
			for (int j = 0; j < extendedHeight; ++j)
			{
				int value = m_vecStage[j][i];  // 적절한 값으로 설정

				// 노드나 화살표를 생성해야 하는 경우
				if (value >= 1) {
					// 오브젝트의 x, y 좌표를 계산
					int x = WidthMargin + (i * horizontal_gap);
					int y = HeightMargin + (j * vertical_gap);

					// 오브젝트를 생성 및 설정
					CBtnUI_Stage* pBtnUI = new CBtnUI_Stage(value);
					pBtnUI->SetName((value >= 4) ? L"ChildUI_Node" : L"ChildUI_Arrow");
					pBtnUI->SetGridPos(Vec2(i, j));
					pBtnUI->SetPos(Vec2(x, y));
					m_mapBtnUI[Vec2(i, j)] = pBtnUI;

					// 버튼에 함수 포인터 설정 -> 버튼 클릭시 스테이지 선택
					/*((CBtnUI_Stage*)pBtnUI)->SetClickedCallBack(this, (SCENE_BTNCLICK)&CScene_Robby::SetCurrentNode);*/

					// 패널에 오브젝트를 추가
					pPanelUI->AddChild(pBtnUI);
				}
			}
		}
		AddObject(pPanelUI, GROUP_TYPE::UI);

		// 맨 앞 노드들 선택 가능으로 설정
		for (int i = 0; i < m_vecStage.size(); i++)
		{
			auto iter = m_mapBtnUI.find(Vec2(0, i));

			// 맵 검색 결과에 있으면, 선택 가능 설정
			if (iter != m_mapBtnUI.end())
			{
				iter->second->SetSelect(true);
			}
		}

		// 카메라 설정
		m_CameraPos = Vec2(float(vResolution.x / 4), float(PanelHeight / 2));
		
		enter = true;
	}
	CCamera::GetInstance()->SetLookAt(m_CameraPos);
}

void CScene_Robby::Exit()
{
}

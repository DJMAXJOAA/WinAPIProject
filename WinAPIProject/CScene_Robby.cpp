#include "pch.h"
#include "CScene_Robby.h"

#include "CCore.h"
#include "CKeyMgr.h"
#include "CTimeMgr.h"
#include "CCamera.h"
#include "CSceneMgr.h"
#include "CDataMgr.h"

#include "CBackground.h"

#include "CPanelUI_Back.h"
#include "CBtnUI_Stage.h"

#include "GameData.h"

// 최초 1회 필드 구성
static bool enter;

CScene_Robby::CScene_Robby()
	: m_CameraPos{}
	, m_vecStage{}
	, m_mainUI(nullptr)
	, m_mapGridNode{}
	, m_mapBtnUI{}
	, m_pCurrentNode(nullptr)
{
}

CScene_Robby::~CScene_Robby()
{
	SafeDeleteVec(m_vecNodes);
	DeleteAll();
}

void CScene_Robby::NodeInit()
{
	// 전투 종료 시, 함수 실행
	GameData* data = (GameData*)CDataMgr::GetInstance()->FindData(0);

	for (auto& route : data->m_vecRoute)
	{
		// 같은 x들 선택 못하게 삭제
		int x = (int)route.x;
		for (int i = 0; i < m_vecStage.size(); ++i)
		{
			Vec2 pos(x, i);
			if (m_mapBtnUI[pos] != nullptr)
			{
				m_mapBtnUI[pos]->SetSelect(false);
			}
		}

		if (m_mapBtnUI[route] != nullptr)
		{
			// 선택 상태로 만들기
			m_mapBtnUI[route]->SetSelect(true);
			m_mapBtnUI[route]->SetClear(true);

			Vec2 CameraPos = Vec2(m_mapBtnUI[route]->GetPos().x, m_CameraPos.y);
			CCamera::GetInstance()->SetLookAt(CameraPos);
		}

		auto iter = m_mapGridNode.find(route);
		if (iter == m_mapGridNode.end())
			assert(1);
		else
		{
			m_pCurrentNode = iter->second;
			m_lstRoute.push_back(route);

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
}

void CScene_Robby::OnEnterStage(Vec2 _vPos)
{
	// 현재 좌표 설정 -> 전투 승리 시, 설정 위해서
	GameData* data = (GameData*)CDataMgr::GetInstance()->FindData(0);
	data->m_currentGridPos = _vPos;


	// 카메라 효과 :: 1초 후, 페이드아웃 -> 턴넘김 -> 페이드인
	CCamera::GetInstance()->WhiteScreen(0.5f);
	CCamera::GetInstance()->FadeOut(1.0f);
	CCamera::GetInstance()->Event(0.01f);
	CCamera::GetInstance()->BlackScreen(1.0f);
	CCamera::GetInstance()->FadeIn(1.0f);

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
		// 선택 상태로 만들기
		m_mapBtnUI[_vPos]->SetSelect(true);

		m_CameraPos = Vec2(m_mapBtnUI[_vPos]->GetPos().x, m_CameraPos.y);
		CCamera::GetInstance()->SetLookAt(m_CameraPos);
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
		// 게임 데이터 로드
		GameData* data = (GameData*)CDataMgr::GetInstance()->FindData(0);

		// 데이터 매니저에 저장되있는 배열을 가져옴
		m_vecStage = data->m_vecMap;

		// 맵 정보를 토대로 노드 생성
		m_vecNodes = CMapGenerator::CreateStartPath(m_vecStage, m_mapGridNode);

		// UI 추가
		Vec2 vResolution = CCore::GetInstance()->GetResolution(); // 화면 크기 가져오기

		CPanelUI_Back* pPanelUI = new CPanelUI_Back;
		pPanelUI->SetName(L"ParentUI");
		pPanelUI->SetScale(Vec2(ROBBY_SETTINGS::PANEL_WIDTH, ROBBY_SETTINGS::PANEL_HEIGHT));
		pPanelUI->SetPos(Vec2(0, 0));
		m_mainUI = pPanelUI;

		// 패널 내에서 사용 가능한 너비와 높이
		int available_width = ROBBY_SETTINGS::PANEL_WIDTH - ROBBY_SETTINGS::WIDTH_MARGIN * 2;
		int available_height = ROBBY_SETTINGS::PANEL_HEIGHT - ROBBY_SETTINGS::HEIGHT_MARGIN * 2;

		// 일정한 간격으로 오브젝트를 배치하기 위한 간격을 계산
		int horizontal_gap = available_width / ROBBY_SETTINGS::EXTEND_WIDTH;
		int vertical_gap = available_height / ROBBY_SETTINGS::EXTEND_HEIGHT;

		// 2차원 배열을 순회하면서 오브젝트를 생성
		for (int i = 0; i < ROBBY_SETTINGS::EXTEND_WIDTH; ++i)
		{
			for (int j = 0; j < ROBBY_SETTINGS::EXTEND_HEIGHT; ++j)
			{
				int value = m_vecStage[j][i];  // 적절한 값으로 설정

				// 노드나 화살표를 생성해야 하는 경우
				if (value >= 1) {
					// 오브젝트의 x, y 좌표를 계산
					int x = ROBBY_SETTINGS::WIDTH_MARGIN + (i * horizontal_gap);
					int y = ROBBY_SETTINGS::HEIGHT_MARGIN + (j * vertical_gap);

					// 오브젝트를 생성 및 설정
					CBtnUI_Stage* pBtnUI = new CBtnUI_Stage(value);
					pBtnUI->SetName((value >= 4) ? L"ChildUI_Node" : L"ChildUI_Arrow");
					pBtnUI->SetGridPos(Vec2(i, j));
					pBtnUI->SetPos(Vec2(x, y));
					m_mapBtnUI[Vec2(i, j)] = pBtnUI;

					// 패널에 오브젝트를 추가
					pPanelUI->AddChild(pBtnUI);
				}
			}
		}
		CreateObj(pPanelUI, GROUP_TYPE::UI);

		// 백그라운드
		CBackground* pBackground = new CBackground(L"texture\\Background\\stage_background.png");
		CreateObj(pBackground, GROUP_TYPE::BACKGROUND);

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
		m_CameraPos = Vec2(float(vResolution.x / 4), float(ROBBY_SETTINGS::PANEL_HEIGHT / 2));
		
		enter = true;
	}

	// 노드 설정
	NodeInit();

	// 카메라 설정
	CCamera::GetInstance()->SetLookAt(m_CameraPos);
}

void CScene_Robby::Exit()
{
}

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

// ���� 1ȸ �ʵ� ����
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
	// ���� ���� ��, �Լ� ����
	GameData* data = (GameData*)CDataMgr::GetInstance()->FindData(0);

	for (auto& route : data->m_vecRoute)
	{
		// ���� x�� ���� ���ϰ� ����
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
			// ���� ���·� �����
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
	// ���� ��ǥ ���� -> ���� �¸� ��, ���� ���ؼ�
	GameData* data = (GameData*)CDataMgr::GetInstance()->FindData(0);
	data->m_currentGridPos = _vPos;


	// ī�޶� ȿ�� :: 1�� ��, ���̵�ƿ� -> �ϳѱ� -> ���̵���
	CCamera::GetInstance()->WhiteScreen(0.5f);
	CCamera::GetInstance()->FadeOut(1.0f);
	CCamera::GetInstance()->Event(0.01f);
	CCamera::GetInstance()->BlackScreen(1.0f);
	CCamera::GetInstance()->FadeIn(1.0f);

	// ���� x�� ���� ���ϰ� ����
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
		// ���� ���·� �����
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
	
	// ȭ�� ��ȯ
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
	// ���� 1ȸ�� ����
	if (!enter)
	{
		// ���� ������ �ε�
		GameData* data = (GameData*)CDataMgr::GetInstance()->FindData(0);

		// ������ �Ŵ����� ������ִ� �迭�� ������
		m_vecStage = data->m_vecMap;

		// �� ������ ���� ��� ����
		m_vecNodes = CMapGenerator::CreateStartPath(m_vecStage, m_mapGridNode);

		// UI �߰�
		Vec2 vResolution = CCore::GetInstance()->GetResolution(); // ȭ�� ũ�� ��������

		CPanelUI_Back* pPanelUI = new CPanelUI_Back;
		pPanelUI->SetName(L"ParentUI");
		pPanelUI->SetScale(Vec2(ROBBY_SETTINGS::PANEL_WIDTH, ROBBY_SETTINGS::PANEL_HEIGHT));
		pPanelUI->SetPos(Vec2(0, 0));
		m_mainUI = pPanelUI;

		// �г� ������ ��� ������ �ʺ�� ����
		int available_width = ROBBY_SETTINGS::PANEL_WIDTH - ROBBY_SETTINGS::WIDTH_MARGIN * 2;
		int available_height = ROBBY_SETTINGS::PANEL_HEIGHT - ROBBY_SETTINGS::HEIGHT_MARGIN * 2;

		// ������ �������� ������Ʈ�� ��ġ�ϱ� ���� ������ ���
		int horizontal_gap = available_width / ROBBY_SETTINGS::EXTEND_WIDTH;
		int vertical_gap = available_height / ROBBY_SETTINGS::EXTEND_HEIGHT;

		// 2���� �迭�� ��ȸ�ϸ鼭 ������Ʈ�� ����
		for (int i = 0; i < ROBBY_SETTINGS::EXTEND_WIDTH; ++i)
		{
			for (int j = 0; j < ROBBY_SETTINGS::EXTEND_HEIGHT; ++j)
			{
				int value = m_vecStage[j][i];  // ������ ������ ����

				// ��峪 ȭ��ǥ�� �����ؾ� �ϴ� ���
				if (value >= 1) {
					// ������Ʈ�� x, y ��ǥ�� ���
					int x = ROBBY_SETTINGS::WIDTH_MARGIN + (i * horizontal_gap);
					int y = ROBBY_SETTINGS::HEIGHT_MARGIN + (j * vertical_gap);

					// ������Ʈ�� ���� �� ����
					CBtnUI_Stage* pBtnUI = new CBtnUI_Stage(value);
					pBtnUI->SetName((value >= 4) ? L"ChildUI_Node" : L"ChildUI_Arrow");
					pBtnUI->SetGridPos(Vec2(i, j));
					pBtnUI->SetPos(Vec2(x, y));
					m_mapBtnUI[Vec2(i, j)] = pBtnUI;

					// �гο� ������Ʈ�� �߰�
					pPanelUI->AddChild(pBtnUI);
				}
			}
		}
		CreateObj(pPanelUI, GROUP_TYPE::UI);

		// ��׶���
		CBackground* pBackground = new CBackground(L"texture\\Background\\stage_background.png");
		CreateObj(pBackground, GROUP_TYPE::BACKGROUND);

		// �� �� ���� ���� �������� ����
		for (int i = 0; i < m_vecStage.size(); i++)
		{
			auto iter = m_mapBtnUI.find(Vec2(0, i));

			// �� �˻� ����� ������, ���� ���� ����
			if (iter != m_mapBtnUI.end())
			{
				iter->second->SetSelect(true);
			}
		}

		// ī�޶� ����
		m_CameraPos = Vec2(float(vResolution.x / 4), float(ROBBY_SETTINGS::PANEL_HEIGHT / 2));
		
		enter = true;
	}

	// ��� ����
	NodeInit();

	// ī�޶� ����
	CCamera::GetInstance()->SetLookAt(m_CameraPos);
}

void CScene_Robby::Exit()
{
}

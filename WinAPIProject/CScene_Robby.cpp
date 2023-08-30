#include "pch.h"
#include "CScene_Robby.h"

#include "CCore.h"
#include "CKeyMgr.h"
#include "CTimeMgr.h"
#include "CCamera.h"

#include "CPanelUI_Back.h"
#include "CBtnUI_Stage.h"

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
}

CScene_Robby::~CScene_Robby()
{
	delete m_MapGenerator;
}

void CScene_Robby::SetCurrentNode(Vec2 _vPos)
{
}

void CScene_Robby::Render(HDC hdc)
{
	CScene::Render(hdc);
}

void CScene_Robby::Update()
{
	CScene::Update();
	
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
	// �� ����
	m_vecStage = m_MapGenerator->CreateRandomMap();
	m_vecNodes = m_MapGenerator->CreateStartPos(m_vecStage, m_mapGridNode);

	// UI �߰�
	Vec2 vResolution = CCore::GetInstance()->GetResolution(); // ȭ�� ũ�� ��������

	const int WidthMargin = 50;
	const int HeightMargin = 50;
	const int extendedWidth = (WIDTH * 2) - 1;
	const int extendedHeight = (HEIGHT * 2) - 1;
	const int PanelWidth = 2400;
	const int PanelHeight = 640;

	CPanelUI_Back* pPanelUI = new CPanelUI_Back;
	pPanelUI->SetName(L"ParentUI");
	pPanelUI->SetScale(Vec2(PanelWidth, PanelHeight));
	pPanelUI->SetPos(Vec2(0,0));
	m_mainUI = pPanelUI;

	// �г� ������ ��� ������ �ʺ�� ����
	int available_width = PanelWidth - WidthMargin * 2;
	int available_height = PanelHeight - HeightMargin * 2;

	// ������ �������� ������Ʈ�� ��ġ�ϱ� ���� ������ ����մϴ�.
	int horizontal_gap = available_width / extendedWidth;
	int vertical_gap = available_height / extendedHeight;

	// 2���� �迭�� ��ȸ�ϸ鼭 ������Ʈ�� �����մϴ�.
	for (int i = 0; i < extendedWidth; ++i) 
	{
		for (int j = 0; j < extendedHeight; ++j) 
		{
			int value = m_vecStage[j][i];  // ������ ������ ����

			// ��峪 ȭ��ǥ�� �����ؾ� �ϴ� ���
			if (value >= 1) {
				// ������Ʈ�� x, y ��ǥ�� ���
				int x = WidthMargin + (i * horizontal_gap);
				int y = HeightMargin + (j * vertical_gap);

				// ������Ʈ�� ���� �� ����
				CBtnUI_Stage* pBtnUI = new CBtnUI_Stage(value);
				pBtnUI->SetName((value >= 4) ? L"ChildUI_Node" : L"ChildUI_Arrow");
				pBtnUI->SetGridPos(Vec2(i, j));
				pBtnUI->SetPos(Vec2(x, y));
				m_mapBtnUI[Vec2(i, j)] = pBtnUI;
				
				// ��ư�� �Լ� ������ ���� -> ��ư Ŭ���� �������� ����
				/*((CBtnUI_Stage*)pBtnUI)->SetClickedCallBack(this, (SCENE_BTNCLICK)&CScene_Robby::SetCurrentNode);*/

				// �гο� ������Ʈ�� �߰�
				pPanelUI->AddChild(pBtnUI);
			}
		}
	}
	AddObject(pPanelUI, GROUP_TYPE::UI);

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
	m_CameraPos = Vec2(float(vResolution.x / 4), float(PanelHeight / 2));
	CCamera::GetInstance()->SetLookAt(m_CameraPos);
}

void CScene_Robby::Exit()
{
}

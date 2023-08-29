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
	, m_mapGridBtn{}
{
	m_MapGenerator = new CMapGenerator;
}

CScene_Robby::~CScene_Robby()
{
	delete m_MapGenerator;
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

		if(m_CameraPos.x > vResolution.x / 2)
		{
			m_CameraPos.x -= 1000.f * fDT;
			CCamera::GetInstance()->SetLookAt(m_CameraPos);
		}
	}
	if (KEY_HOLD(KEY::RIGHT))
	{
		Vec2 vResolution = CCore::GetInstance()->GetResolution();

		if (m_CameraPos.x < vResolution.x * 2 - (vResolution.x * 2 - 2400.f) * 4)
		{
			m_CameraPos.x += 1000.f * fDT;
			CCamera::GetInstance()->SetLookAt(m_CameraPos);
		}
	}
}

void CScene_Robby::Enter()
{
	const int WidthMargin = 50;
	const int HeightMargin = 50;

	// 맵 생성
	m_vecStage = m_MapGenerator->CreateRandomMap();
	m_vecNodes = m_MapGenerator->CreateStartPos(m_vecStage, m_mapGridBtn);

	// UI 추가
	Vec2 vResolution = CCore::GetInstance()->GetResolution();

	CPanelUI_Back* pPanelUI = new CPanelUI_Back;
	pPanelUI->SetName(L"ParentUI");
	pPanelUI->SetScale(Vec2(2400.f, 630.f));
	pPanelUI->SetPos(Vec2((vResolution.x * 2 - pPanelUI->GetScale().x) / 2, (vResolution.y - pPanelUI->GetScale().y) / 2));
	m_mainUI = pPanelUI;
	/*AddObject(pPanelUI, GROUP_TYPE::UI);*/

	int available_width = int(pPanelUI->GetScale().x - WidthMargin * 2);
	int available_height = int(pPanelUI->GetScale().y - HeightMargin * 2);

	int horizontal_gap = (available_width - (WIDTH * 64)) / (WIDTH - 1);
	int vertical_gap = (available_height - (HEIGHT * 64)) / (HEIGHT - 1);

	for (int i = 0; i < WIDTH; i++) {
		for (int j = 0; j < HEIGHT; j++) {
			int value = m_vecStage[j * 2][i * 2];  // _vecMap이 2배 크기라고 가정
			if (value >= 4) {
				int x = WidthMargin + (i * (64 + horizontal_gap));
				int y = HeightMargin + (j * (64 + vertical_gap));

				CBtnUI_Stage* pBtnUI = m_mapGridBtn[Vec2(i, j)];
				if (pBtnUI)
				{
					pBtnUI->SetName(L"ChildUI");
					pBtnUI->SetScale(Vec2(64.f, 64.f));
					pBtnUI->SetPos(Vec2(x, y));

					AddObject(pBtnUI, GROUP_TYPE::UI);
					/*pPanelUI->AddChild(pBtnUI);*/
				}
			}
		}
	}

	m_CameraPos = Vec2(vResolution.x / 2, vResolution.y / 2);
	CCamera::GetInstance()->SetLookAt(m_CameraPos);
}

void CScene_Robby::Exit()
{
}

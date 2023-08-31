#include "pch.h"
#include "CScene_Start.h"

#include "CCore.h"
#include "CKeyMgr.h"
#include "CResMgr.h"
#include "CSoundMgr.h"

#include "CSound.h"

#include "CBackground.h"
#include "CBtnUI_Start.h"
#include "CPanelUI_Number.h"

CScene_Start::CScene_Start()
	: m_pBGM(nullptr)
{
	// ����� ���
	m_pBGM = CResMgr::GetInstance()->LoadSound(L"BGM", L"sound\\reminiscence.wav");
	CSoundMgr::GetInstance()->RegisterToBGM(m_pBGM);
	m_pBGM->PlayToBGM(true);
}

CScene_Start::~CScene_Start()
{

}

void CScene_Start::Update()
{
	CScene::Update();	// �θ��� �Լ��� �̿��� ���� ����, �������̵� �� ������ +@ �ϱ� ���ؼ�

	if (KEY_TAP(KEY::ENTER))
	{
		ChangeScene(SCENE_TYPE::TOOL);
	}
}


void CScene_Start::Enter()
{
	Vec2 vResolution = CCore::GetInstance()->GetResolution();

	// ��׶���
	CBackground* pBackground = new CBackground(L"texture\\Background\\title_background.png");
	CreateObj(pBackground, GROUP_TYPE::BACKGROUND);

	// Ÿ��Ʋ ����
	vector<wstring> tile{ L"texture\\UI\\title.png", L"texture\\UI\\title.png" };
	CBtnUI_Start* pTilte = new CBtnUI_Start(tile, false);
	pTilte->SetPos(Vec2(vResolution.x / 2, 200.f));
	CreateObj(pTilte, GROUP_TYPE::UI);

	// ���� ��ư
	vector<wstring> start{ L"texture\\UI\\start1.png", L"texture\\UI\\start2.png" };
	CBtnUI_Start* pStart = new CBtnUI_Start(start, false);
	pStart->SetPos(Vec2(vResolution.x / 2, 550.f));
	pStart->SetScale(Vec2(300, 80));
	CreateObj(pStart, GROUP_TYPE::UI);

	// �ε� ��ư
	vector<wstring> load{ L"texture\\UI\\load1.png", L"texture\\UI\\load2.png" };
	CBtnUI_Start* pLoad = new CBtnUI_Start(load, true);
	pLoad->SetPos(Vec2(vResolution.x / 2, 650.f));
	pLoad->SetScale(Vec2(300, 80));
	CreateObj(pLoad, GROUP_TYPE::UI);
}

void CScene_Start::Exit()
{
	DeleteAll();
}


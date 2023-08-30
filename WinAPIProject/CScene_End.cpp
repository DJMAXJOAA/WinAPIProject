#include "pch.h"
#include "CScene_End.h"

#include "CCore.h"
#include "CKeyMgr.h"

#include "CBackground.h"
#include "CBtnUI_Start.h"

CScene_End::CScene_End()
{
}

CScene_End::~CScene_End()
{
}

void CScene_End::Update()
{
	CScene::Update();	// �θ��� �Լ��� �̿��� ���� ����, �������̵� �� ������ +@ �ϱ� ���ؼ�

	if (KEY_TAP(KEY::ENTER))
	{
		ChangeScene(SCENE_TYPE::TOOL);
	}
}

void CScene_End::Enter()
{
	Vec2 vResolution = CCore::GetInstance()->GetResolution();

	// ��׶���
	CBackground* pBackground = new CBackground(L"texture\\Background\\end_background.png");
	CreateObj(pBackground, GROUP_TYPE::BACKGROUND);
}

void CScene_End::Exit()
{
	DeleteAll();
}

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
	CScene::Update();	// 부모쪽 함수를 이용할 수도 있음, 오버라이딩 한 이유는 +@ 하기 위해서

	if (KEY_TAP(KEY::ENTER))
	{
		ChangeScene(SCENE_TYPE::TOOL);
	}
}

void CScene_End::Enter()
{
	Vec2 vResolution = CCore::GetInstance()->GetResolution();

	// 백그라운드
	CBackground* pBackground = new CBackground(L"texture\\Background\\end_background.png");
	CreateObj(pBackground, GROUP_TYPE::BACKGROUND);
}

void CScene_End::Exit()
{
	DeleteAll();
}

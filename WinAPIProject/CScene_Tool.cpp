#include "pch.h"
#include "CScene_Tool.h"
#include "resource.h"

#include "CCore.h"
#include "CKeyMgr.h"
#include "CResMgr.h"
#include "CDataMgr.h"
#include "CSceneMgr.h"
#include "CUIMgr.h"
#include "CPathMgr.h"

#include "CPlayer.h"
#include "CTile.h"
#include "CBlock.h"
#include "CToolTest.h"
#include "CPanelUI.h"
#include "CBtnUI.h"

#include "CTexture.h"
#include "CAnimator.h"
#include "CAnimation.h"

#include "AnimationData.h"

void ChangeScene(DWORD_PTR, DWORD_PTR);
Vec2 ToolPanelPos(300, 300);

CScene_Tool::CScene_Tool()
	: m_pUI(nullptr)
	, m_pDisplay(nullptr)
{
}

CScene_Tool::~CScene_Tool()
{
}

void CScene_Tool::SetOffset(Vec2 _value)
{
	vector<tAnimFrame>& offset = m_pDisplay->GetAnimator()->GetAnimation()->GetAllFrame();
	for (int i = 0; i < (int)offset.size(); i++)
	{
		offset[i].vOffset = _value;
	}

	AnimationData* data = (AnimationData*)CDataMgr::GetInstance()->FindData(m_pDisplay->GetAnimator()->GetAnimation()->GetID());
	data->m_AniInfo.vOffset.x = float(_value.x * -1.0);
	data->m_AniInfo.vOffset.y = float(_value.y * -1.0);
	DEBUG2(data->m_AniInfo.vOffset.x, data->m_AniInfo.vOffset.y);
}

void CScene_Tool::AddOffset(Vec2 _value)
{
	vector<tAnimFrame>& offset = m_pDisplay->GetAnimator()->GetAnimation()->GetAllFrame();
	for (int i = 0; i < (int)offset.size(); i++)
	{
		offset[i].vOffset += _value;
	}
	AnimationData* data = (AnimationData*)CDataMgr::GetInstance()->FindData(m_pDisplay->GetAnimator()->GetAnimation()->GetID());
	data->m_AniInfo.vOffset.x += float(_value.x * -1.0);
	data->m_AniInfo.vOffset.y += float(_value.y * -1.0);
	DEBUG2(data->m_AniInfo.vOffset.x, data->m_AniInfo.vOffset.y);

}

void CScene_Tool::PrevFrame()
{
	m_pDisplay->ChangeAnimation(m_pDisplay->GetAnimator()->GetAnimation()->GetID() - 1);
}

void CScene_Tool::NextFrame()
{
	m_pDisplay->ChangeAnimation(m_pDisplay->GetAnimator()->GetAnimation()->GetID() + 1);
}

void CScene_Tool::SaveAnimation()
{
	AnimationData* data = (AnimationData*)CDataMgr::GetInstance()->FindData(m_pDisplay->GetAnimator()->GetAnimation()->GetID());
	
	data->SaveData();
}

void CScene_Tool::Update()
{
	CScene::Update();	// �θ��� �Լ��� �̿��� ���� ����, �������̵� �� ������ +@ �ϱ� ���ؼ�

	if (KEY_TAP(KEY::ENTER))
	{
		ChangeScene(SCENE_TYPE::BATTLE);
	}
	if (KEY_TAP(KEY::ESC))
	{
		SaveAnimation();
	}
	//if (KEY_TAP(KEY::RBTN))
	//{
	//	Vec2 vLookAt = CCamera::GetInstance()->GetRealPos(MOUSE_POS);
	//	CCamera::GetInstance()->SetLookAt(vLookAt);
	//}
	if (KEY_TAP(KEY::Z))
	{
		PrevFrame();
	}
	if (KEY_TAP(KEY::X))
	{
		NextFrame();
	}

	if (KEY_TAP(KEY::LBTN))
	{
		Vec2 mousePos = MOUSE_POS - ToolPanelPos;
		SetOffset(mousePos);
	}
	if (KEY_TAP(KEY::LEFT))
	{
		AddOffset(Vec2(-1.0f, 0.f));
	}
	if (KEY_TAP(KEY::RIGHT))
	{
		AddOffset(Vec2(1.0f, 0.f));
	}
	if (KEY_TAP(KEY::UP))
	{
		AddOffset(Vec2(0.f, -1.0f));
	}
	if (KEY_TAP(KEY::DOWN))
	{
		AddOffset(Vec2(0.f, 1.0f));
	}
}



void CScene_Tool::Enter()
{
	// UI �߰�
	Vec2 vResolution = CCore::GetInstance()->GetResolution();

	// �ǹ� ���� �г�
	CToolTest* toolTest = new CToolTest;
	toolTest->SetPos(ToolPanelPos);
	m_pDisplay = toolTest;
	AddObject(toolTest, GROUP_TYPE::DEFAULT);

	CPanelUI* pPanelUI = new CPanelUI;
	pPanelUI->SetName(L"ParentUI");
	pPanelUI->SetScale(Vec2(500.f, 300.f));
	pPanelUI->SetPos(Vec2(vResolution.x - pPanelUI->GetScale().x, 0.f));

	CBtnUI* pBtnUI = new CBtnUI;
	pBtnUI->SetName(L"ChildUI");
	pBtnUI->SetScale(Vec2(100.f, 30.f));
	pBtnUI->SetPos(Vec2(0.f, 0.f));
	/*((CBtnUI*)pBtnUI)->SetClickedCallBack(ChangeScene, 0, 0);*/
	/*((CBtnUI*)pBtnUI)->SetClickedCallBack(this, (SCENE_MEMFUNC)&CScene_Tool::SaveTileData);*/
	pPanelUI->AddChild(pBtnUI);

	// UI�� ���� UI�ϳ��� �� -> ��� ChildUI�� �Բ� ��
	AddObject(pPanelUI, GROUP_TYPE::UI);

	//// ���纻 ui �����
	//CUI* pClonePanel = pPanelUI->Clone();
	//pClonePanel->SetPos(pClonePanel->GetPos() + Vec2(-300.f, 0.f));
	//((CBtnUI*)pClonePanel->GetChildUI()[0])->SetClickedCallBack(ChangeScene, 0, 0);		// ���纻���� CallBack�Լ� ���� -> �Ʒ� ����
	//AddObject(pClonePanel, GROUP_TYPE::UI);

	//m_pUI = pClonePanel;

	int startX = (int)(vResolution.x / 2);
	int startY = (int)(vResolution.y / 4);

	// Player �߰�

	for (int y = 0; y < 1; ++y) {
		for (int x = 0; x < 1; ++x) {
			int drawX = startX + (x - y) * (TILE_WIDTH / 2);
			int drawY = startY + (x + y) * (TILE_HEIGHT / 2) - (TILE_HEIGHT / 2);

			CPlayer* pObj = new CPlayer;
			pObj->SetPos(Vec2(drawX, drawY));
			AddObject(pObj, GROUP_TYPE::PLAYER);

			CTile* pTile = new CTile;
			pTile->SetPos(Vec2(drawX, drawY));
			AddObject(pTile, GROUP_TYPE::TILE);

			CBlock* pBlcok = new CBlock(MAP_TYPE::SNOW);
			pBlcok->SetPos(Vec2(drawX, drawY));
			AddObject(pBlcok, GROUP_TYPE::BLOCK);
		}
	}


	CCamera::GetInstance()->SetLookAt(vResolution / 2.f);
}

void CScene_Tool::Exit()
{
	DeleteAll();

}


//============================================================================================================
void ChangeScene(DWORD_PTR, DWORD_PTR)
{
	ChangeScene(SCENE_TYPE::START);
}

// �޴��ٿ��� �ִϸ������� Ű���� �޾Ƽ�, ������Ʈ�� ���Ž�Ű��
INT_PTR CALLBACK ChangeAnimator(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK)
		{
			UINT iKey = GetDlgItemInt(hDlg, IDC_EDIT1, nullptr, false);

			// ���� ���� �����´�
			CScene* pCurScene = CSceneMgr::GetInstance()->GetCurScene();

			// �ٿ� ĳ���� -> ���� �� �� ���� �ƴ�
			CScene_Tool* pToolScene = dynamic_cast<CScene_Tool*>(pCurScene);
			assert(pToolScene);
			
			// �ִϸ����� ����
			CToolTest* temp = (CToolTest*)pToolScene->GetGroupObject(GROUP_TYPE::DEFAULT)[0];
			temp->ChangeAnimator(iKey);

			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		else if (LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}

		break;
	}
	return (INT_PTR)FALSE;
}
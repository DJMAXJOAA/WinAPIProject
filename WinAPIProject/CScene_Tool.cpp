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

#include "CTile.h"
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
	DEBUG2(offset[0].vOffset.x, offset[0].vOffset.y);
	for (int i = 0; i < (int)offset.size(); i++)
	{
		offset[i].vOffset = _value;
	}

	AnimationData* data = (AnimationData*)CDataMgr::GetInstance()->FindData(m_pDisplay->GetAnimator()->GetAnimation()->GetID());
	data->m_AniInfo.vOffset = _value;
}

void CScene_Tool::AddOffset(Vec2 _value)
{
	vector<tAnimFrame>& offset = m_pDisplay->GetAnimator()->GetAnimation()->GetAllFrame();
	DEBUG2(offset[0].vOffset.x, offset[0].vOffset.y);
	for (int i = 0; i < (int)offset.size(); i++)
	{
		offset[i].vOffset += _value;
	}
	AnimationData* data = (AnimationData*)CDataMgr::GetInstance()->FindData(m_pDisplay->GetAnimator()->GetAnimation()->GetID());
	data->m_AniInfo.vOffset += _value;
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

void CScene_Tool::SaveTileData()
{
	// ���� ��θ� �����ϰ�, savetile�� �ҷ�����
	wchar_t szName[256] = {};

	OPENFILENAME ofn = {};

	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = CCore::GetInstance()->GetMainhWnd();
	ofn.lpstrFile = szName;
	ofn.nMaxFile = sizeof(szName);
	ofn.lpstrFilter = L"ALL\0*.*\0Tile\0*.tile\0";			// ALL -> ��� ������ �ٺ���, �߰� ���� ���� ���� (���� ������ All ���� tile)
	ofn.nFilterIndex = 0;
	ofn.lpstrFileTitle = nullptr;
	ofn.nMaxFileTitle = 0;

	wstring strTileFolder = CPathMgr::GetInstance()->GetContentPath();
	strTileFolder += L"tile";

	ofn.lpstrInitialDir = strTileFolder.c_str();				// ���� ���� �ʱ� ���(content �����Ĥ� �ִ°� ������)
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

	// Modal ����� Dialog â ���� -> ��� ����� â�� ���ִ� ���� �ٸ� �����찡 �۵��� ����
	if (GetSaveFileName(&ofn))
	{
		// Ȯ��â�� ������ ��ȯ���� �����ؼ�, ���� ����ȴ�
		// szName�� ���ϰ�η� ���� -> ������

		SaveTile(szName);
	}
}

void CScene_Tool::LoadTileData()
{
	// ���� ��θ� �����ϰ�, savetile�� �ҷ�����
	wchar_t szName[256] = {};

	OPENFILENAME ofn = {};

	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = CCore::GetInstance()->GetMainhWnd();
	ofn.lpstrFile = szName;
	ofn.nMaxFile = sizeof(szName);
	ofn.lpstrFilter = L"ALL\0*.*\0Tile\0*.tile\0";			// ALL -> ��� ������ �ٺ���, �߰� ���� ���� ���� (���� ������ All ���� tile)
	ofn.nFilterIndex = 0;
	ofn.lpstrFileTitle = nullptr;
	ofn.nMaxFileTitle = 0;

	wstring strTileFolder = CPathMgr::GetInstance()->GetContentPath();
	strTileFolder += L"tile";

	ofn.lpstrInitialDir = strTileFolder.c_str();				// ���� ���� �ʱ� ���(content �����Ĥ� �ִ°� ������)
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

	// Modal ����� Dialog â ���� -> ��� ����� â�� ���ִ� ���� �ٸ� �����찡 �۵��� ����
	if (GetOpenFileName(&ofn))
	{
		wstring strRelativePath = CPathMgr::GetInstance()->GetRelativePath(szName);
		LoadTile(strRelativePath);
	}
}

void CScene_Tool::SaveTile(const wstring& _strRelativePath)
{
	// Ŀ�� ������Ʈ -> ���� �ݾ����� �ʰ�, ���� �����͵��� �ݾ��ִ°�
	// ���� ��Ʈ���� ������ ���� �����͵�
	FILE* pFile = nullptr;
	_wfopen_s(&pFile, _strRelativePath.c_str(), L"wb");		// ���� Ŀ�� ������Ʈ, ���, �����б�(w -> ����, r -> �б�, wb, rb -> ���̳ʸ��� �����б�)
	assert(pFile);

	//����������===========================================================================================

	const vector<CObject*>& vecTile = GetGroupObject(GROUP_TYPE::TILE);

	for (size_t i = 0; i < vecTile.size(); i++)
	{
		((CTile*)vecTile[i])->Save(pFile);
	}

	//����������===========================================================================================

	fclose(pFile);
}

void CScene_Tool::Update()
{
	CScene::Update();	// �θ��� �Լ��� �̿��� ���� ����, �������̵� �� ������ +@ �ϱ� ���ؼ�

	// �ִϸ��̼� ����
	static int animator_count = 0;

	if (KEY_TAP(KEY::ENTER))
	{
		ChangeScene(SCENE_TYPE::START);
	}
	if (KEY_TAP(KEY::SPACE))
	{
		SaveAnimation();
	}
	//if (KEY_TAP(KEY::RBTN))
	//{
	//	Vec2 vLookAt = CCamera::GetInstance()->GetRealPos(MOUSE_POS);
	//	CCamera::GetInstance()->SetLookAt(vLookAt);
	//}
	if (KEY_TAP(KEY::Q))
	{
		PrevFrame();
	}
	if (KEY_TAP(KEY::W))
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
	((CBtnUI*)pBtnUI)->SetClickedCallBack(this, (SCENE_MEMFUNC)&CScene_Tool::SaveTileData);
	pPanelUI->AddChild(pBtnUI);

	// UI�� ���� UI�ϳ��� �� -> ��� ChildUI�� �Բ� ��
	AddObject(pPanelUI, GROUP_TYPE::UI);

	//// ���纻 ui �����
	//CUI* pClonePanel = pPanelUI->Clone();
	//pClonePanel->SetPos(pClonePanel->GetPos() + Vec2(-300.f, 0.f));
	//((CBtnUI*)pClonePanel->GetChildUI()[0])->SetClickedCallBack(ChangeScene, 0, 0);		// ���纻���� CallBack�Լ� ���� -> �Ʒ� ����
	//AddObject(pClonePanel, GROUP_TYPE::UI);

	//m_pUI = pClonePanel;

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
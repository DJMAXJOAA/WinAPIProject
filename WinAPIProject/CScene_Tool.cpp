#include "pch.h"
#include "CScene_Tool.h"
#include "resource.h"

#include "CCore.h"
#include "CKeyMgr.h"
#include "CResMgr.h"
#include "CSceneMgr.h"
#include "CUIMgr.h"
#include "CPathMgr.h"

#include "CTile.h"
#include "CTexture.h"
#include "CPanelUI.h"
#include "CBtnUI.h"

void ChangeScene(DWORD_PTR, DWORD_PTR);

CScene_Tool::CScene_Tool()
	: m_pUI(nullptr)
{
}

CScene_Tool::~CScene_Tool()
{
}

void CScene_Tool::SetTileIndex()
{
	// ���� ��ġ�� �˾ƾߴ�

	if (KEY_TAP(KEY::LBTN))
	{
		Vec2 vMousePos = MOUSE_POS;
		vMousePos = CCamera::GetInstance()->GetRealPos(vMousePos);

		// ������ Ư�� �ε����� �ٷ� �����ϱ� ����, ���� private ������ Ÿ�� ������ ����
		// �׷��� 2�� for������ ã�� �ʿ� ����, �ٷ� ������ �����ϰ� �Ѵ�
		int iTileX = GetTileX();
		int iTileY = GetTileY();

		int iCol = (int)vMousePos.x / TILE_SIZE;
		int iRow = (int)vMousePos.y / TILE_SIZE;

		int iIndex = iRow * iTileX + iCol;

		if (vMousePos.x < 0.f || iTileX <= iCol
			|| vMousePos.y < 0.f || iTileY <= iRow)
		{
			return;
		}

		const vector<CObject*>& vecTile = GetGroupObject(GROUP_TYPE::TILE);
		((CTile*)vecTile[iIndex])->AddImageIndex();
	}
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

	UINT xCount = GetTileX();
	UINT yCount = GetTileY();

	fwrite(&xCount, sizeof(UINT), 1, pFile);			// ���� �ּ�, ������, ��� ����(�迭�̸� 1�̻� ��), ������
	fwrite(&yCount, sizeof(UINT), 1, pFile);

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

	if (KEY_TAP(KEY::ENTER))
	{
		ChangeScene(SCENE_TYPE::START);
	}
	if (KEY_TAP(KEY::RBTN))
	{
		Vec2 vLookAt = CCamera::GetInstance()->GetRealPos(MOUSE_POS);
		CCamera::GetInstance()->SetLookAt(vLookAt);
	}
	if (KEY_TAP(KEY::LSHIFT))
	{
		/*CUIMgr::GetInstance()->SetFocusedUI(m_pUI);*/
		SaveTileData();
	}
	if (KEY_TAP(KEY::CTRL))
	{
		LoadTileData();
	}

	SetTileIndex();
}


void CScene_Tool::Enter()
{
	CreateTile(5, 5);

	// UI �߰�
	Vec2 vResolution = CCore::GetInstance()->GetResolution();

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

// TILE COUNT WINDOW PROC
// �޴��� ��ü�� �� �������� Ȱ��ȭ�ǰ� �����ؾ� �Ѵ�
INT_PTR CALLBACK TileCountProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK)
		{
			UINT iXCount = GetDlgItemInt(hDlg, IDC_EDIT1, nullptr, false);
			UINT iYCount = GetDlgItemInt(hDlg, IDC_EDIT2, nullptr, false);

			// ���� ���� �����´�
			CScene* pCurScene = CSceneMgr::GetInstance()->GetCurScene();

			// �ٿ� ĳ���� -> ���� �� �� ���� �ƴ�
			CScene_Tool* pToolScene = dynamic_cast<CScene_Tool*>(pCurScene);
			assert(pToolScene);

			pToolScene->DeleteGroup(GROUP_TYPE::TILE);
			pToolScene->CreateTile(iXCount, iYCount);

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
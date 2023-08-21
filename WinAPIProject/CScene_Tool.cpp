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
	// 저장 경로를 지정하고, savetile을 불러오기
	wchar_t szName[256] = {};

	OPENFILENAME ofn = {};

	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = CCore::GetInstance()->GetMainhWnd();
	ofn.lpstrFile = szName;
	ofn.nMaxFile = sizeof(szName);
	ofn.lpstrFilter = L"ALL\0*.*\0Tile\0*.tile\0";			// ALL -> 모든 파일이 다보임, 추가 필터 설정 가능 (파일 형식이 All 도는 tile)
	ofn.nFilterIndex = 0;
	ofn.lpstrFileTitle = nullptr;
	ofn.nMaxFileTitle = 0;

	wstring strTileFolder = CPathMgr::GetInstance()->GetContentPath();
	strTileFolder += L"tile";

	ofn.lpstrInitialDir = strTileFolder.c_str();				// 파일 열때 초기 경로(content 폴더ㅔㅇ 넣는게 좋을듯)
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

	// Modal 방식의 Dialog 창 띄우기 -> 모달 방식의 창은 떠있는 동안 다른 윈도우가 작동을 멈춤
	if (GetSaveFileName(&ofn))
	{
		// 확인창을 누르면 반환값이 존재해서, 파일 저장된다
		// szName에 파일경로로 저장 -> 절대경로

		SaveTile(szName);
	}
}

void CScene_Tool::LoadTileData()
{
	// 저장 경로를 지정하고, savetile을 불러오기
	wchar_t szName[256] = {};

	OPENFILENAME ofn = {};

	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = CCore::GetInstance()->GetMainhWnd();
	ofn.lpstrFile = szName;
	ofn.nMaxFile = sizeof(szName);
	ofn.lpstrFilter = L"ALL\0*.*\0Tile\0*.tile\0";			// ALL -> 모든 파일이 다보임, 추가 필터 설정 가능 (파일 형식이 All 도는 tile)
	ofn.nFilterIndex = 0;
	ofn.lpstrFileTitle = nullptr;
	ofn.nMaxFileTitle = 0;

	wstring strTileFolder = CPathMgr::GetInstance()->GetContentPath();
	strTileFolder += L"tile";

	ofn.lpstrInitialDir = strTileFolder.c_str();				// 파일 열때 초기 경로(content 폴더ㅔㅇ 넣는게 좋을듯)
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

	// Modal 방식의 Dialog 창 띄우기 -> 모달 방식의 창은 떠있는 동안 다른 윈도우가 작동을 멈춤
	if (GetOpenFileName(&ofn))
	{
		wstring strRelativePath = CPathMgr::GetInstance()->GetRelativePath(szName);
		LoadTile(strRelativePath);
	}
}

void CScene_Tool::SaveTile(const wstring& _strRelativePath)
{
	// 커널 오브젝트 -> 직접 닫아주지 않고, 파일 포인터들을 닫아주는거
	// 파일 스트림이 각각의 파일 포인터들
	FILE* pFile = nullptr;
	_wfopen_s(&pFile, _strRelativePath.c_str(), L"wb");		// 파일 커널 오브젝트, 경로, 쓰기읽기(w -> 쓰기, r -> 읽기, wb, rb -> 바이너리로 쓰기읽기)
	assert(pFile);

	//데이터저장===========================================================================================

	const vector<CObject*>& vecTile = GetGroupObject(GROUP_TYPE::TILE);

	for (size_t i = 0; i < vecTile.size(); i++)
	{
		((CTile*)vecTile[i])->Save(pFile);
	}

	//데이터저장===========================================================================================

	fclose(pFile);
}

void CScene_Tool::Update()
{
	CScene::Update();	// 부모쪽 함수를 이용할 수도 있음, 오버라이딩 한 이유는 +@ 하기 위해서

	// 애니메이션 변수
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
	// UI 추가
	Vec2 vResolution = CCore::GetInstance()->GetResolution();

	// 피벗 설정 패널
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

	// UI는 메인 UI하나만 들어감 -> 대신 ChildUI도 함께 들어감
	AddObject(pPanelUI, GROUP_TYPE::UI);

	//// 복사본 ui 만들기
	//CUI* pClonePanel = pPanelUI->Clone();
	//pClonePanel->SetPos(pClonePanel->GetPos() + Vec2(-300.f, 0.f));
	//((CBtnUI*)pClonePanel->GetChildUI()[0])->SetClickedCallBack(ChangeScene, 0, 0);		// 복사본에만 CallBack함수 구현 -> 아래 구현
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

// 메뉴바에서 애니메이터의 키값을 받아서, 오브젝트에 갱신시키기
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

			// 현재 씬을 가져온다
			CScene* pCurScene = CSceneMgr::GetInstance()->GetCurScene();

			// 다운 캐스팅 -> 실패 시 툴 씬이 아님
			CScene_Tool* pToolScene = dynamic_cast<CScene_Tool*>(pCurScene);
			assert(pToolScene);
			
			// 애니메이터 변경
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
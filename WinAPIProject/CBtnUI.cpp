#include "pch.h"
#include "CBtnUI.h"

#include "CResMgr.h"

#include "CSound.h"

#include "CScene.h"
#include "CObject.h"

CBtnUI::CBtnUI(bool _bCamAff)
	: CUI(_bCamAff)
	, m_pFunc(nullptr)
	, m_param1(0)
	, m_param2(0)
	, m_pSceneFunc(nullptr)
	, m_pSceneInst(nullptr)
	, m_pBtnSound(nullptr)
{
	m_pBtnSound = CResMgr::GetInstance()->LoadSound(L"BtnSound", L"sound\\button.wav");
}

CBtnUI::~CBtnUI()
{
}

void CBtnUI::MouseOn()
{
}

void CBtnUI::MouseLbtnDown()
{
}

void CBtnUI::MouseLbtnUp()
{
}

void CBtnUI::MouseLbtnClicked()
{
	//if (m_pFunc != nullptr)
	//{
	//	m_pFunc(m_param1, m_param2);
	//}

	//if (m_pSceneInst && m_pSceneFunc)
	//{
	//	// ���� ��ü�� ���� �Լ��� ���� -> ((*��ü).*�Լ�())(���������� ����);
	//	((*m_pSceneInst).*m_pSceneFunc)();
	//}
	m_pBtnSound->Play(false);
}

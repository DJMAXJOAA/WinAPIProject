#include "pch.h"
#include "CBtnUI_Start.h"

#include "CPathMgr.h"
#include "CKeyMgr.h"
#include "CDataMgr.h"

#include "GameData.h"

CBtnUI_Start::CBtnUI_Start(vector<wstring> _filenames, bool _bLoad)
	: CBtnUI(false)
	, m_vecImage{}
	, m_pImage(nullptr)
	, m_bLoad(_bLoad)
{
	for (auto& file : _filenames)
	{
		wstring strFilePath = CPathMgr::GetInstance()->GetContentPath();
		strFilePath += file;

		m_vecImage.push_back(new Image(strFilePath.c_str()));
	}
	m_pImage = m_vecImage[0];

	// ¿À·ù °ËÃâ
	assert(m_pImage);
}

void CBtnUI_Start::MouseOnCheck()
{
	Vec2 vScale = GetScale() / 2;
	Vec2 vMousePos = MOUSE_POS;

	Vec2 vFinalPos = GetFinalPos();

	if (IsCamAffected())
	{
		vMousePos = CCamera::GetInstance()->GetRealPos(vMousePos);
	}

	if (vFinalPos.x - vScale.x <= vMousePos.x && vMousePos.x <= vFinalPos.x + vScale.x
		&& vFinalPos.y - vScale.y <= vMousePos.y && vMousePos.y <= vFinalPos.y + vScale.y)
	{
		SetMouseOn(true);
	}
	else
	{
		SetMouseOn(false);
	}
}

CBtnUI_Start::~CBtnUI_Start()
{
	SafeDeleteVec(m_vecImage);
}

void CBtnUI_Start::MouseOn()
{

}

void CBtnUI_Start::MouseLbtnDown()
{

}

void CBtnUI_Start::MouseLbtnUp()
{

}

void CBtnUI_Start::MouseLbtnClicked()
{
	// ·Îµå
	if (m_bLoad)
	{
		GameData* data = (GameData*)CDataMgr::GetInstance()->FindData(0);
		

		ChangeScene(SCENE_TYPE::ROBBY);
	}
	// °Á °×½ÃÀÛ
	else
	{
		GameData* data = (GameData*)CDataMgr::GetInstance()->FindData(0);
		data->Init();

		ChangeScene(SCENE_TYPE::ROBBY);
	}
	
}

void CBtnUI_Start::Render(HDC hdc)
{
	Graphics graphics(hdc);

	int imgWidth = m_pImage->GetWidth();
	int imgHeight = m_pImage->GetHeight();

	int drawX = (int)GetPos().x - (imgWidth / 2);
	int drawY = (int)GetPos().y - (imgHeight / 2);
	
	graphics.DrawImage(m_pImage, drawX, drawY);
}

void CBtnUI_Start::Update()
{
	CUI::Update();

	if (IsMouseOn())
	{
		m_pImage = m_vecImage[1];
	}
	else
	{
		m_pImage = m_vecImage[0];
	}
}

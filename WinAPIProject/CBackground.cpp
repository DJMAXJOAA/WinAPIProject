#include "pch.h"
#include "CBackground.h"

#include "CCore.h"

#include "CResMgr.h"
#include "CPathMgr.h"

#include "CTexture.h"

CBackground::CBackground(wstring _filename)
	: m_image(nullptr)
{
	wstring strFilePath = CPathMgr::GetInstance()->GetContentPath();
	strFilePath += _filename;

	m_image = new Image(strFilePath.c_str());
	
	// 오류 검출
	assert(m_image);
}

CBackground::~CBackground()
{
	delete m_image;
}

void CBackground::Render(HDC hdc)
{
	Graphics graphics(hdc);

	graphics.DrawImage(m_image, GetPos().x, GetPos().y);
}

void CBackground::Update()
{

}
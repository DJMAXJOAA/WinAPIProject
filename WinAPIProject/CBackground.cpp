#include "pch.h"
#include "CBackground.h"

#include "CPathMgr.h"

CBackground::CBackground(wstring _filename)
	: m_image(nullptr)
{
	wstring strFilePath = CPathMgr::GetInstance()->GetContentPath();
	strFilePath += _filename;

	m_image = new Image(strFilePath.c_str());
	
	// ���� ����
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

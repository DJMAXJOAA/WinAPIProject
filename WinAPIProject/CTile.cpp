#include "pch.h"
#include "CTile.h"

#include "CTexture.h"

CTile::CTile()
	: m_pTileTex(nullptr)
	, m_iImgIndex(9)
{
	
}

CTile::~CTile()
{
}

void CTile::Save(FILE* _pFile)
{
	fwrite(&m_iImgIndex, sizeof(int), 1, _pFile);
}

void CTile::Load(FILE* _pFile)
{
	fread(&m_iImgIndex, sizeof(int), 1, _pFile);
}

void CTile::Update()
{
}

void CTile::Render(HDC hdc)
{
	if (m_pTileTex == nullptr || m_iImgIndex == -1)
		return;

	UINT iWidth = m_pTileTex->Width();
	UINT iHeight = m_pTileTex->Height();

	Vec2 vRenderPos = CCamera::GetInstance()->GetRenderPos(GetPos());
	Vec2 vScale = GetScale();
}

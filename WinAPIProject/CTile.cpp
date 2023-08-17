#include "pch.h"
#include "CTile.h"

#include "CTexture.h"

CTile::CTile()
	: m_pTileTex(nullptr)
	, m_iImgIndex(9)
{
	SetScale(Vec2(TILE_SIZE, TILE_SIZE));
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

	UINT iMaxCol = iWidth / TILE_SIZE;
	UINT iMaxRow = iHeight / TILE_SIZE;

	UINT iCurRow = (UINT)m_iImgIndex / iMaxCol;
	UINT iCurCol = (UINT)m_iImgIndex % iMaxCol;

	if (iMaxRow <= iCurRow)
		assert(nullptr);

	Vec2 vRenderPos = CCamera::GetInstance()->GetRenderPos(GetPos());
	Vec2 vScale = GetScale();

	BitBlt(hdc
	, (int)vRenderPos.x
	, (int)vRenderPos.y
	, (int)vScale.x
	, (int)vScale.y
	, m_pTileTex->GetDC()
	, iCurCol * TILE_SIZE
	, iCurRow * TILE_SIZE
	, SRCCOPY);
}

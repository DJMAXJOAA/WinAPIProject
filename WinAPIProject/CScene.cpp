#include "pch.h"
#include "CScene.h"

#include "CResMgr.h"
#include "CKeyMgr.h"
#include "CPathMgr.h"

#include "CObject.h"
#include "CTile.h"
#include "CTexture.h"

void CScene::Update()
{
	for (UINT i = 0; i < (UINT)GROUP_TYPE::END; i++)
	{
		for (size_t j = 0; j < m_arrObj[i].size(); j++)
		{
			// 죽어있지 않으면, 업데이트
			if (!m_arrObj[i][j]->IsDead())
			{
				m_arrObj[i][j]->Update();
			}
		}
	}
}

void CScene::FinalUpdate()
{
	for (UINT i = 0; i < (UINT)GROUP_TYPE::END; i++)
	{
		for (size_t j = 0; j < m_arrObj[i].size(); j++)
		{
			m_arrObj[i][j]->FinalUpdate();
		}
	}
}

void CScene::Render(HDC hdc)
{
	for (UINT i = 0; i < (UINT)GROUP_TYPE::END; i++)
	{
		vector<CObject*>::iterator iter = m_arrObj[i].begin();

		for(; iter != m_arrObj[i].end();)
		{
			// 죽어있지 않으면, 렌더링
			if (!(*iter)->IsDead())
			{
				(*iter)->Render(hdc);
				++iter;	// 삭제한 오브젝트인데 증가시키면 두번증가되니까 정상렌더링 된경우만 증가
			}
			else
			{
				iter = m_arrObj[i].erase(iter); // 마지막 과정에서 삭제시키면, 담프레임에 없어짐
			}
		}
	}
}

CScene::CScene()
	: m_iTileX(0)
	, m_iTileY(0)
{
}

CScene::~CScene()
{
	// 씬 모든 오브젝트 삭제
	for (UINT i = 0; i < (UINT)GROUP_TYPE::END; i++)
	{
		for (size_t j = 0; j < m_arrObj[i].size(); j++)
		{
			// m_arrObj[i]의 그룹의 벡터 j 물체 삭제
			delete m_arrObj[i][j];
		}
	}
}

void CScene::DeleteGroup(GROUP_TYPE _eTarget)
{
	// 자주 사용하는 코드라서, func.h에 템플릿으로 구현
	SafeDeleteVec(m_arrObj[(UINT)_eTarget]);
}

void CScene::DeleteAll()
{
	// 모든 타입의 오브젝트 제거
	for (UINT i = 0; i < (UINT)GROUP_TYPE::END; i++)
	{
		DeleteGroup((GROUP_TYPE)i);
	}
}

void CScene::CreateTile(UINT _iXCount, UINT _iYCount)
{
	DeleteGroup(GROUP_TYPE::TILE);

	m_iTileX = _iXCount;
	m_iTileY = _iYCount;

	CTexture* pTileTex = CResMgr::GetInstance()->LoadTexture(L"Tile", L"texture\\TILE.bmp");

	for (UINT i = 0; i < _iYCount; ++i)
	{
		for (UINT j = 0; j < _iXCount; ++j)
		{
			CTile* pTile = new CTile();

			pTile->SetPos(Vec2((float)(j * TILE_SIZE), (float)(i * TILE_SIZE)));
			pTile->SetTexture(pTileTex);

			AddObject(pTile, GROUP_TYPE::TILE);
		}
	}
}

void CScene::LoadTile(const wstring& _strRelativePath)
{
	wstring strFilePath = CPathMgr::GetInstance()->GetContentPath();
	strFilePath += _strRelativePath;	// 진짜 경로

	// 커널 오브젝트 -> 직접 닫아주지 않고, 파일 포인터들을 닫아주는거
	// 파일 스트림이 각각의 파일 포인터들
	FILE* pFile = nullptr;
	_wfopen_s(&pFile, strFilePath.c_str(), L"rb");		// 파일 커널 오브젝트, 경로, 쓰기읽기(w -> 쓰기, r -> 읽기, wb, rb -> 바이너리로 쓰기읽기)
	assert(pFile);

	//데이터 불러오기===========================================================================================

	UINT xCount;
	UINT yCount;

	fread(&xCount, sizeof(UINT), 1, pFile);			// 시작 주소, 사이즈, 요소 갯수(배열이면 1이상도 ㅇ), 저장경로
	fread(&yCount, sizeof(UINT), 1, pFile);

	CreateTile(xCount, yCount);

	const vector<CObject*>& vecTile = GetGroupObject(GROUP_TYPE::TILE);

	for (size_t i = 0; i < vecTile.size(); i++)
	{
		((CTile*)vecTile[i])->Load(pFile);
	}

	//데이터 불러오기===========================================================================================

	fclose(pFile);
}

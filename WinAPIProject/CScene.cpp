
#include "pch.h"
#include "CScene.h"

#include "CResMgr.h"
#include "CKeyMgr.h"
#include "CPathMgr.h"

#include "CObject.h"
#include "CTexture.h"

void CScene::Update()
{
	for (UINT i = 0; i < (UINT)GROUP_TYPE::END; i++)
	{
		for (size_t j = 0; j < m_arrObj[i].size(); j++)
		{
			// �׾����� ������, ������Ʈ
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
			// �׾����� ������, ������
			if (!(*iter)->IsDead())
			{
				(*iter)->Render(hdc);
				++iter;	// ������ ������Ʈ�ε� ������Ű�� �ι������Ǵϱ� ���󷻴��� �Ȱ�츸 ����
			}
			else
			{
				iter = m_arrObj[i].erase(iter); // ������ �������� ������Ű��, �������ӿ� ������
			}
		}
	}
}

CScene::CScene()
{
}

CScene::~CScene()
{
	// �� ��� ������Ʈ ����
	for (UINT i = 0; i < (UINT)GROUP_TYPE::END; i++)
	{
		for (size_t j = 0; j < m_arrObj[i].size(); j++)
		{
			// m_arrObj[i]�� �׷��� ���� j ��ü ����
			delete m_arrObj[i][j];
		}
	}
}

void CScene::DeleteGroup(GROUP_TYPE _eTarget)
{
	// ���� ����ϴ� �ڵ��, func.h�� ���ø����� ����
	SafeDeleteVec(m_arrObj[(UINT)_eTarget]);
}

void CScene::DeleteAll()
{
	// ��� Ÿ���� ������Ʈ ����
	for (UINT i = 0; i < (UINT)GROUP_TYPE::END; i++)
	{
		DeleteGroup((GROUP_TYPE)i);
	}
}

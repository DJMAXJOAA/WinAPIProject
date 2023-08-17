#include "pch.h"
#include "CCollisionMgr.h"

#include "CSceneMgr.h"

#include "CScene.h"
#include "CObject.h"

#include "CCollider.h"

CCollisionMgr::CCollisionMgr()
	: m_arrCheck{}
{
}

CCollisionMgr::~CCollisionMgr()
{
}

void CCollisionMgr::Init()
{
	
}

void CCollisionMgr::Update()
{
	// ��Ʈ���� ON �Ǿ��ִ� �κи� üũ�ϰ� �Ѵ�(��ü �ٵ����� �δ��� �ʹ� ������)
	for (UINT iRow = 0; iRow < (UINT)GROUP_TYPE::END; iRow++)
	{
		// ���ۺκ��� iRow������, �ֳĸ� �ߺ�üũ ���ҷ��� -> ���� �׻� �� ����
		for (UINT iCol = iRow; iCol < (UINT)GROUP_TYPE::END; iCol++) 
		{
			if (m_arrCheck[iRow] & (1 << iCol)) // ��Ʈ true�ΰ� üũ -> �׷쳢�� ���Ϸ���
			{
				CollisionGroupUpdate((GROUP_TYPE)iRow, (GROUP_TYPE)iCol);
			}
		}
	}
}

void CCollisionMgr::CheckGroup(GROUP_TYPE _eLeft, GROUP_TYPE _eRight)
{
	// �� ���� ���� �׷��� ������ ���
	// �� ū ���� ��(��Ʈ) �� ����Ѵ�
	UINT iRow = (UINT)_eLeft;
	UINT iCol = (UINT)_eRight; 

	if (iCol < iRow) // ���� �� ������ ������ߵȴ�, �׻� ��(col)�� �� Ŀ�ߴ�
	{
		iRow = (UINT)_eRight;
		iCol = (UINT)_eLeft;
	}

	if (m_arrCheck[iRow] & (1 << iCol))		// �̹� üũ�� �Ǿ�������, üũ�� �����Ѵ�
	{
		m_arrCheck[iRow] &= ~(1 << iCol); 
	}
	else
	{
		m_arrCheck[iRow] |= (1 << iCol);	// ���� ���ϴ� �κ� üũ ����(iCol��ŭ ��Ʈ�� �̵� -> üũ�ڽ�) 
	}
}

void CCollisionMgr::CollisionGroupUpdate(GROUP_TYPE _eLeft, GROUP_TYPE _eRight)
{
	// Update���� ���ϰ�, Ư�� �׷쳢���� �浹�� ���Ѵ�
	CScene* pCurScene = CSceneMgr::GetInstance()->GetCurScene();
	
	// ���۷��� �� ����(���� ����, ���� ����)
	const vector<CObject*>& vecLeft = pCurScene->GetGroupObject(_eLeft);
	const vector<CObject*>& vecRight = pCurScene->GetGroupObject(_eRight);

	// ���� �������� �浹 ������ ���� �ִ� map �����̳�
	map<ULONGLONG, bool>::iterator iter;

	// �浹 ��
	// ���ǻ���1 :: �ڱ� �ڽŰ� �� ���� �ʱ�
	// ���ǻ���2 :: �ݶ��̴��� ���� ������Ʈ�� ������ �ʱ�
	for (size_t i = 0; i < vecLeft.size(); i++)
	{
		if (vecLeft[i]->GetCollider() == nullptr) // �ݶ��̴� ���� ������Ʈ���� ��
			continue;

		for (size_t j = 0; j < vecRight.size(); j++)
		{
			// �ݶ��̴� ���� ������Ʈ �н� + �ڱ� �ڽ� �н�
			if (vecRight[j]->GetCollider() == nullptr || vecLeft[i] == vecRight[j])
				continue;

			CCollider* pLeftCol = vecLeft[i]->GetCollider();
			CCollider* pRightCol = vecRight[j]->GetCollider();

			// ����ü�� ���� �Ӽ��� Ȱ���ؼ� ������ ���ο� Ű�� �����
			// �� �浹ü ���� ���̵�
			COLLIDER_ID ID{};
			ID.iLeft_id = pLeftCol->GetID();
			ID.iRight_id = pRightCol->GetID();
			iter = m_mapColInfo.find(ID.ID);

			// �˻� ��������(�ʿ� ������ ����, ������ �˻��Ҷ� true false�Ǵ�)
			if (m_mapColInfo.end() == iter)
			{
				m_mapColInfo.insert(make_pair(ID.ID, false));
				iter = m_mapColInfo.find(ID.ID); // ��� �� �ٽ� ��˻�
			}

			// ������ �浹 ���ϴ� �κ� -> ���� �Լ��� ��
			if (IsCollision(pLeftCol, pRightCol))
			{
				if (iter->second)
				{
					// �� �����ӿ��� �浹���̿���(Stay)

					if (vecLeft[i]->IsDead() || vecRight[j]->IsDead())
					{
						// �ٵ� ���� �ϳ� �̻��� ������ ������ -> �׷��� �浹 ��� ó��
						pLeftCol->OnCollisionExit(pRightCol);
						pRightCol->OnCollisionExit(pLeftCol);
						iter->second = false;
					}

					pLeftCol->OnCollision(pRightCol);
					pRightCol->OnCollision(pLeftCol);
				}
				else
				{
					// ó�� �浹�ϴ� ��Ȳ(Enter)

					if (!vecLeft[i]->IsDead() && !vecRight[j]->IsDead())
					{
						// �Ѵ� ����־�� ���� ����, �ϳ��� ������ �� �浹 ���� ó��
						pLeftCol->OnCollisionEnter(pRightCol);
						pRightCol->OnCollisionEnter(pLeftCol);
						iter->second = true;
					}
				}
			}
			else
			{
				if (iter->second)
				{
					// �� ��� ��Ȳ(Exit)
					pLeftCol->OnCollisionExit(pRightCol);
					pRightCol->OnCollisionExit(pLeftCol);
					iter->second = false;
				}
			}
		}
	}
}

bool CCollisionMgr::IsCollision(CCollider* _pLeftCol, CCollider* _pRightCol)
{
	// �浹�� ������ �˻�
	Vec2 vLeftPos = _pLeftCol->GetFinalPos();
	Vec2 vLeftScale = _pLeftCol->GetScale();

	Vec2 vRightPos = _pRightCol->GetFinalPos();
	Vec2 vRightScale = _pRightCol->GetScale();

	if (abs(vRightPos.x - vLeftPos.x) < (vLeftScale.x + vRightScale.x) / 2.f
		&& abs(vRightPos.y - vLeftPos.y) < (vLeftScale.y + vRightScale.y) / 2.f)
	{
		return true;
	}

	return false;
}

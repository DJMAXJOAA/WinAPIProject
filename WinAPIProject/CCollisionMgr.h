#pragma once

class CCollider;

// map�� Ű������ ��, �ݶ��̴������� �����ϴ� ������ Ű��(��ġ�� �ʴ�)
// ����ü�� ���� ���� �����ϴ� �Ӽ��� ���� map�� ������ �� �ִ� ���ο� �浹 ������ ����� �����Ѵ�
union COLLIDER_ID
{
	struct
	{
		UINT iLeft_id;
		UINT iRight_id;
	};
	ULONGLONG ID;
};

class CCollisionMgr
{
	SINGLE(CCollisionMgr);
private:
	map<ULONGLONG, bool>	m_mapColInfo;	// �浹ü ���� ���� ������ �浹 ���� (����ü�� �̿��� Ű��->������ ��, bool��)
	UINT					m_arrCheck[(UINT)GROUP_TYPE::END];	// �׷찣 �浹 üũ

private:
	CCollisionMgr();
	~CCollisionMgr();

public:
	void Init();
	void Update();
	void Reset() { memset(m_arrCheck, 0, sizeof(UINT) * (UINT)GROUP_TYPE::END); } // ��Ʈ���� ���ϴµ� ��Ʈ �ʱ�ȭ
	void CheckGroup(GROUP_TYPE _eLeft, GROUP_TYPE _eRight);

private:
	void CollisionGroupUpdate(GROUP_TYPE _eLeft, GROUP_TYPE _eRight);
	bool IsCollision(CCollider* _pLeftCol, CCollider* _pRightCol);
};


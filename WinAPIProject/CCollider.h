#pragma once

class CObject;

// ���� 25~26������ ó������(���ž) 
class CCollider
{
private:
	// �ݶ��̴��� �����ڰ� �Ҹ������� g_iNextID�� ������Ű��, m_iID�� ���� �־��ش�.
	static UINT g_iNextID;	// ���� ����� Ŭ���� ����� ���� ��, ������ ������ ����Ǽ� ������� ����
	
	CObject* m_pOwner;		// �ݶ��̴��� �پ��ִ� ������Ʈ�� ������ �˾ƾ� �Ѵ�(�׷��� ����ٴϴϱ�)
	Vec2	 m_vOffsetPos;	// ���� ��ǥ(������ ������Ʈ�� ��ǥ)������ ������� �Ÿ�
	Vec2	 m_vFinalPos;	// FinalUpdate()�� ���� ���� ���� �ݶ��̴� ��ġ(������+��ǥ��) -> �� ������ ����ؼ� ����
	Vec2	 m_vScale;		// �浹ü�� ũ��

	UINT	 m_iID;			// ����� ���� ID�� (��ġ�� �ȵ�)
	UINT	 m_iCol;

public:
	CCollider();
	CCollider(const CCollider& _origin); // ���� ���縦 ���� �����Ǵ� ���� ������
	~CCollider();

	CCollider& operator = (CCollider& _origin) = delete;

	friend class CObject;

public:
	void SetOffsetPos(Vec2 _vPos) { m_vOffsetPos = _vPos; }
	void SetScale(Vec2 _vScale) { m_vScale = _vScale; }
	
public:
	Vec2 GetOffsetPos() { return m_vOffsetPos; }
	Vec2 GetScale() { return m_vScale; }
	Vec2 GetFinalPos() { return m_vFinalPos; }

	UINT GetID() { return m_iID; }

	CObject* GetObj() { return m_pOwner; }

public:
	void FinalUpdate();
	void Render(HDC hdc);	// �׽�Ʈ �Ҷ� �ѳ��ٰ�, ������ ����

public:
	void OnCollision(CCollider* _pOther);		// �浹 ���϶� ȣ��
	void OnCollisionEnter(CCollider* _pOther);	// �浹 ���������� ȣ��
	void OnCollisionExit(CCollider* _pOther);	// �浹 ���������� ȣ��
};
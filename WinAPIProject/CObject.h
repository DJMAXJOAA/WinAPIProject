#pragma once

// ī�޶�� �׻� �����ϴϱ� CCamera.h�� ������Ʈ ����������� ���Խ�Ű��
// ������� ��������� ���� ������ ������, �׻� ���Ǵ� ��������̶� ����
#include "CCamera.h"

class CCollider;
class CAnimator;

// ��üȭ �� �� ���� �߻� Ŭ����
class CObject
{
private:
	wstring		m_strName;		// ������Ʈ �ĺ� �̸�

	Vec2		m_vPos;
	Vec2		m_vScale;

	// Componet
	CCollider*	m_pCollider;	// nullptr�̶�� �ݶ��̴��� �ʿ� ���� ������Ʈ��(����̳� ui ��)
	CAnimator*  m_pAnimator;

	bool		m_bAlive;		// Ȱ��ȭ, ��Ȱ��ȭ -> ��Ȱ��ȭ�Ǹ� ���� �����ӿ� ����

public:
	CObject();
	CObject(const CObject& _origin); // ���� ����
	virtual ~CObject();

	friend class CEventMgr;

public:
	void SetName(const wstring& _strName) { m_strName = _strName; }
	void SetPos(Vec2 vPos) { m_vPos = vPos; }
	void SetScale(Vec2 vScale) { m_vScale = vScale; }

private:
	void SetDead() { m_bAlive = false; }	// �̺�Ʈ �Ŵ����� ����, �ڽĵ� ���� �Ұ�

public:
	wstring GetName() { return m_strName; }
	Vec2 GetPos() { return m_vPos; }
	Vec2 GetScale() { return m_vScale; }

	CCollider* GetCollider() { return m_pCollider; }
	CAnimator* GetAnimator() { return m_pAnimator; }

	bool IsDead() { return !m_bAlive; }

public:
	void CreateCollider();
	virtual void OnCollision(CCollider* _pOther) {};
	virtual void OnCollisionEnter(CCollider* _pOther) {};
	virtual void OnCollisionExit(CCollider* _pOther) {};

public:
	void CreateAnimator();

public:
	virtual void Update() = 0; // ���� ���� �Լ� -> ������Ʈ���� Update�� ��� ����� �ٸ� (������)
	virtual void FinalUpdate();
	virtual void Render(HDC hdc);

	void ComponetRender(HDC hdc);

	virtual CObject* Clone() = 0;	// ����
};
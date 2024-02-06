#pragma once

// ī�޶�� �׻� �����ϴϱ� CCamera.h�� ������Ʈ ����������� ���Խ�Ű��
// ������� ��������� ���� ������ ������, �׻� ���Ǵ� ��������̶� ����
#include "CCamera.h"

class CCollider;
class CAnimator;
class CAnimatorGdiPlus;
class CHPBar;

// ��üȭ �� �� ���� �߻� Ŭ����
class CObject
{
private:
	wstring		m_strName;		// ������Ʈ �ĺ� �̸�
	Vec2		m_vPos;			// ������Ʈ ��ǥ
	Vec2		m_vScale;		// ������Ʈ ũ��
	Vec2		m_vGridPos;		// ���� ��ǥ (�ʿ�ø� ���)
	bool		m_bAlive;		// Ȱ��ȭ, ��Ȱ��ȭ -> ��Ȱ��ȭ�Ǹ� ���� �����ӿ� ����

	// Componet
	CCollider*			m_pCollider;
	CAnimator*			m_pAnimator;
	CAnimatorGdiPlus*	m_pAnimatorGdiPlus;
	CHPBar*				m_pHealthBar;

public:
	CObject();
	CObject(const CObject& _origin); // ���� ����
	virtual ~CObject();

	friend class CEventMgr;

public:
	void SetName(const wstring& _strName) { m_strName = _strName; }
	void SetPos(Vec2 _vPos) { m_vPos = _vPos; }
	void SetScale(Vec2 vScale) { m_vScale = vScale; }
	void SetGridPos(Vec2 _vPos) { m_vGridPos = _vPos; }

private:
	// �̺�Ʈ �Ŵ����� ����
	void SetDead() { m_bAlive = false; }

public:
	virtual void AnimationEvent() {};	// ������ �̺�Ʈ �����ӿ� ȣ�� (0�̸� ȣ�� X)
	virtual void AnimationEnd() {};		// �ִϸ��̼� �ݺ��� false�� ���¿���, ������ �������̸� ȣ��

public:
	wstring GetName() { return m_strName; }
	Vec2 GetPos() { return m_vPos; }
	Vec2 GetScale() { return m_vScale; }
	Vec2 GetGridPos() { return m_vGridPos; }

	CCollider* GetCollider() { return m_pCollider; }
	CAnimator* GetAnimator() { return m_pAnimator; }
	CAnimatorGdiPlus* GetAnimatorGdiPlus() { return m_pAnimatorGdiPlus; }

	bool IsDead() { return !m_bAlive; }

public:
	void CreateCollider();
	virtual void OnCollision(CCollider* _pOther) {};
	virtual void OnCollisionEnter(CCollider* _pOther) {};
	virtual void OnCollisionExit(CCollider* _pOther) {};

public:
	void CreateAnimator(int _key);
	void SetAnimator(int _key, bool _repeat = true);

public:
	void CreateAnimatorGdiPlus(int _key);
	void SetAnimatorGdiPlus(int _key, bool _repeat = true);

public:
	void CreateHealthBar(bool IsItMonster, int _width = 100, int _height = 10, Vec2 _offset = Vec2(0, -80));

public:
	virtual void Update() = 0; // ���� ���� �Լ� -> ������Ʈ���� Update�� ��� ����� �ٸ� (������)
	virtual void FinalUpdate();
	virtual void Render(HDC hdc);

	void ComponetRender(HDC hdc);

	virtual CObject* Clone() = 0;	// ����
};
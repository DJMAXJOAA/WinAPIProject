#pragma once

// 카메라는 항상 존재하니까 CCamera.h는 오브젝트 헤더에서부터 포함시키기
// 헤더에서 헤더참조는 별로 좋지는 않지만, 항상 사용되는 헤더파일이라서 포함
#include "CCamera.h"

class CCollider;
class CAnimator;
class CAnimatorGdiPlus;

// 객체화 할 수 없는 추상 클래스
class CObject
{
private:
	wstring		m_strName;		// 오브젝트 식별 이름

	Vec2		m_vPos;
	Vec2		m_vScale;
	Vec2		m_vGridPos;		// 격자 좌표 (필요시만 사용)

	// Componet
	CCollider*			m_pCollider;	// nullptr이라면 콜라이더가 필요 없는 오브젝트들(배경이나 ui 등)
	CAnimator*			m_pAnimator;
	CAnimatorGdiPlus*	m_pAnimatorGdiPlus;

	bool		m_bAlive;		// 활성화, 비활성화 -> 비활성화되면 다음 프레임에 삭제

public:
	CObject();
	CObject(const CObject& _origin); // 복사 생성
	virtual ~CObject();

	friend class CEventMgr;

public:
	void SetName(const wstring& _strName) { m_strName = _strName; }
	void SetPos(Vec2 _vPos) { m_vPos = _vPos; }
	void SetScale(Vec2 vScale) { m_vScale = vScale; }
	void SetGridPos(Vec2 _vPos) { m_vGridPos = _vPos; }

private:
	// 이벤트 매니저만 접근
	void SetDead() { m_bAlive = false; }

public:
	virtual void AnimationEvent() {};	// 이벤트가 마지막 프레임이면 이벤트 호출

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
	void SetAnimator(int _key, bool _repeat = true);
	void CreateAnimator(int _key);

public:
	void SetAnimatorGdiPlus(int _key, bool _repeat = true);
	void CreateAnimatorGdiPlus(int _key);

public:
	virtual void Update() = 0; // 순수 가상 함수 -> 오브젝트들의 Update는 모두 기능이 다름 (다형성)
	virtual void FinalUpdate();
	virtual void Render(HDC hdc);

	void ComponetRender(HDC hdc);

	virtual CObject* Clone() = 0;	// 복제
};
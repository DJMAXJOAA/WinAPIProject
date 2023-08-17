#include "pch.h"
#include "CObject.h"

#include "CKeyMgr.h"
#include "CTimeMgr.h"

#include "CCollider.h"
#include "CAnimator.h"

CObject::CObject()
	: m_vPos{}
	, m_vScale{}
	, m_pCollider(nullptr)
	, m_pAnimator(nullptr)
	, m_bAlive(true)
{
}

CObject::CObject(const CObject& _origin)
	: m_strName(_origin.m_strName)
	, m_vPos(_origin.m_vPos)
	, m_vScale(_origin.m_vScale)
	, m_pCollider(nullptr) // ���� �ݶ��̴� ����
	, m_pAnimator(nullptr)
	, m_bAlive(true)
{
	if(_origin.m_pCollider)
	{
		m_pCollider = new CCollider(*_origin.m_pCollider); // �ݶ��̴� ���� ����
		m_pCollider->m_pOwner = this;
	}
	if (_origin.m_pAnimator)
	{
		m_pAnimator = new CAnimator(*_origin.m_pAnimator); // �ݶ��̴� ���� ����
		m_pAnimator->m_pOwner = this;
	}
}

CObject::~CObject()
{	  m_bAlive;	
	// ������Ʈ ������ �־����� �������ֱ�
	if (m_pCollider != nullptr)
		delete m_pCollider;

	if (m_pAnimator != nullptr)
		delete m_pAnimator;
}

void CObject::CreateCollider()
{
	m_pCollider = new CCollider;
	m_pCollider->m_pOwner = this;
}

void CObject::CreateAnimator()
{
	m_pAnimator = new CAnimator;
	m_pAnimator->m_pOwner = this;
}

void CObject::FinalUpdate()
{
	if (m_pAnimator != nullptr)
		m_pCollider->FinalUpdate();

	if (m_pCollider != nullptr)
		m_pCollider->FinalUpdate();
}

void CObject::Render(HDC hdc)
{
	Vec2 vRenderPos = CCamera::GetInstance()->GetRenderPos(m_vPos);

	Rectangle(hdc, (int)(vRenderPos.x - m_vScale.x / 2.f), (int)(vRenderPos.y - m_vScale.y / 2.f),
				   (int)(vRenderPos.x + m_vScale.x / 2.f), (int)(vRenderPos.y + m_vScale.y / 2.f));

	// �ݶ��̴��� �ִ� �� ��ü�� Render ������ �κп� �־��ش�
	ComponetRender(hdc);
}

void CObject::ComponetRender(HDC hdc)
{
	// �浹ü�� �ִ� ���, �������̵��� ������Ʈ�� Render() �κп��� �޼��带 ȣ��
	if (m_pAnimator != nullptr)
	{
		m_pAnimator->Render(hdc);
	}
	if (m_pCollider != nullptr)
	{
		m_pCollider->Render(hdc);
	}
}


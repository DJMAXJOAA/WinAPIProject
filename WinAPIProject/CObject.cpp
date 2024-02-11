#include "pch.h"
#include "CObject.h"

#include "CDataMgr.h"

#include "CCollider.h"
#include "CAnimator.h"
#include "CAnimation.h"
#include "CAnimatorGdiPlus.h"
#include "CAnimationGdiPlus.h"
#include "CHPBar.h"

#include "AnimationData.h"
#include "AnimatorData.h"

CObject::CObject()
	: m_vPos{}
	, m_vScale{}
	, m_vGridPos{}
	, m_pCollider(nullptr)
	, m_pAnimator(nullptr)
	, m_pAnimatorGdiPlus(nullptr)
	, m_pHealthBar(nullptr)
	, m_bAlive(true)
{
}

CObject::CObject(const CObject& _origin)
	: m_strName(_origin.m_strName)
	, m_vPos(_origin.m_vPos)
	, m_vGridPos(_origin.m_vGridPos)
	, m_vScale(_origin.m_vScale)
	, m_pCollider(nullptr)
	, m_pAnimator(nullptr)
	, m_pAnimatorGdiPlus(nullptr)
	, m_pHealthBar(nullptr)
	, m_bAlive(true)
{
	if(_origin.m_pCollider)
	{
		m_pCollider = new CCollider(*_origin.m_pCollider);
		m_pCollider->m_pOwner = this;
	}
	if (_origin.m_pAnimator)
	{
		m_pAnimator = new CAnimator(*_origin.m_pAnimator);
		m_pAnimator->m_pOwner = this;
	}
	if (_origin.m_pAnimatorGdiPlus)
	{
		m_pAnimatorGdiPlus = new CAnimatorGdiPlus(*_origin.m_pAnimatorGdiPlus);
		m_pAnimatorGdiPlus->m_pOwner = this;
	}
	if (_origin.m_pHealthBar)
	{
		m_pHealthBar = new CHPBar(*_origin.m_pHealthBar);
		m_pHealthBar->m_pOwner = this;
	}
}

CObject::~CObject()
{	  m_bAlive;	
	// ������Ʈ ������ �־����� �������ֱ�
	if (m_pCollider != nullptr)
		delete m_pCollider;

	if (m_pAnimator != nullptr)
		delete m_pAnimator;

	if (m_pAnimatorGdiPlus != nullptr)
		delete m_pAnimatorGdiPlus;

	if (m_pHealthBar != nullptr)
		delete m_pHealthBar;
}

void CObject::CreateCollider()
{
	m_pCollider = new CCollider;
	m_pCollider->m_pOwner = this;
}

void CObject::SetAnimator(int _key, bool _repeat)
{
	// �Ľ̵� �ִϸ����� �����͸� �ҷ��ɴϴ�.
	AnimatorData* data = (AnimatorData*)CDataMgr::GetInstance()->FindData(_key);
	assert(data);

	if (m_pAnimator != nullptr)
	{
		delete m_pAnimator;
	}

	// �ִϸ����͸� �����մϴ�.
	CreateAnimator(_key);

	// �ִϸ����� ������ ����� �ִϸ��̼� �ε����� Ŭ������ �����մϴ�.
	for (auto& animation : data->m_vecAnimation)
	{
		AnimationData* aniData = (AnimationData*)CDataMgr::GetInstance()->FindData(animation);

		GetAnimator()->CreateAnimation(animation);
		CAnimation* pAnim = GetAnimator()->FindAnimation(animation);
		for (int i = 0; i < pAnim->GetMaxFrame(); i++)
		{
			pAnim->GetFrame(i).vOffset = aniData->m_AniInfo.vOffset;
		}
	}
	GetAnimator()->Play(data->m_vecAnimation.front(), _repeat);
}

void CObject::CreateAnimator(int _key)
{
	m_pAnimator = new CAnimator;
	m_pAnimator->m_iID = _key;
	m_pAnimator->m_pOwner = this;
}

void CObject::SetAnimatorGdiPlus(int _key, bool _repeat)
{
	AnimatorData* data = (AnimatorData*)CDataMgr::GetInstance()->FindData(_key);
	assert(data);

	if (m_pAnimatorGdiPlus != nullptr)
	{
		delete m_pAnimatorGdiPlus;
	}

	CreateAnimatorGdiPlus(_key);

	for (auto& animation : data->m_vecAnimation)
	{
		AnimationData* aniData = (AnimationData*)CDataMgr::GetInstance()->FindData(animation);

		GetAnimatorGdiPlus()->CreateAnimation(animation);
		CAnimationGdiPlus* pAnim = GetAnimatorGdiPlus()->FindAnimation(animation);
		for (int i = 0; i < pAnim->GetMaxFrame(); i++)
		{
			pAnim->GetFrame(i).vOffset = aniData->m_AniInfo.vOffset;
		}
	}
	GetAnimatorGdiPlus()->Play(data->m_vecAnimation.front(), _repeat);
}

void CObject::CreateAnimatorGdiPlus(int _key)
{
	m_pAnimatorGdiPlus = new CAnimatorGdiPlus;
	m_pAnimatorGdiPlus->m_iID = _key;
	m_pAnimatorGdiPlus->m_pOwner = this;
}

void CObject::CreateHealthBar(bool _bTF, int _width, int _height, Vec2 _offset)
{
	m_pHealthBar = new CHPBar(_width, _height, _offset, _bTF);
	m_pHealthBar->m_pOwner = this;
}

void CObject::FinalUpdate()
{
	if (m_pAnimator != nullptr)
		m_pAnimator->FinalUpdate();

	if (m_pAnimatorGdiPlus != nullptr)
		m_pAnimatorGdiPlus->FinalUpdate();

	if (m_pCollider != nullptr)
		m_pCollider->FinalUpdate();

	if (m_pHealthBar != nullptr)
		m_pHealthBar->FinalUpdate();
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
	if (m_pAnimatorGdiPlus != nullptr)
	{
		m_pAnimatorGdiPlus->Render(hdc);
	}
	if (m_pCollider != nullptr)
	{
		m_pCollider->Render(hdc);
	}
	if (m_pHealthBar != nullptr)
	{
		m_pHealthBar->Render(hdc);
	}
}


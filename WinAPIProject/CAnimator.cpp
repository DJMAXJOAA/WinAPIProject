#include "pch.h"
#include "CAnimator.h"

#include "CObject.h"
#include "CTexture.h"
#include "CAnimation.h"


CAnimator::CAnimator()
	: m_pOwner(nullptr)
	, m_pCurAnim(nullptr)
	, m_bRepeat(false)
{
}

CAnimator::~CAnimator()
{
	SafeDeleteMap(m_mapAnim);
}

void CAnimator::CreateAnimation(const wstring& _strName, CTexture* _pTex, Vec2 _vLeftTop, Vec2 _vSliceSize, Vec2 _vStep, float _fDuration, UINT _iFrameCount)
{
	CAnimation* pAnim = FindAnimation(_strName);
	assert(pAnim == nullptr); // 이미 같은 이름의 애니메이션이 존재하면 안되니까, 오류 검출 부분

	// 존재 안하면 애니메이션 새로 하나 만듬
	pAnim = new CAnimation;

	pAnim->SetName(_strName);
	pAnim->m_pAnimator = this;

	pAnim->Create(_pTex, _vLeftTop, _vSliceSize, _vStep, _fDuration, _iFrameCount);

	m_mapAnim.insert(make_pair(_strName, pAnim));
}

CAnimation* CAnimator::FindAnimation(const wstring& _strName)
{
	map<wstring, CAnimation*>::iterator iter = m_mapAnim.find(_strName);
	if (iter == m_mapAnim.end())
		return nullptr;
	
	return iter->second;
}

void CAnimator::Play(const wstring& _strName, bool _bRepeat)
{
	m_pCurAnim = FindAnimation(_strName);
	m_bRepeat = _bRepeat;
}

void CAnimator::Update()
{
	if (m_pCurAnim != nullptr)
	{
		m_pCurAnim->Update();

		if (m_bRepeat && m_pCurAnim->isFinish())
		{
			m_pCurAnim->SetFrame(0);
		}
	}
}

void CAnimator::FinalUpdate()
{
}

void CAnimator::Render(HDC hdc)
{
	if (m_pCurAnim != nullptr)
		m_pCurAnim->Render(hdc);
}

#include "pch.h"
#include "CAnimatorGdiPlus.h"

#include "CDataMgr.h"
#include "CResMgr.h"
#include "CPathMgr.h"

#include "CObject.h"
#include "CGdiPlus.h"
#include "CAnimationGdiPlus.h"

#include "AnimationData.h"

CAnimatorGdiPlus::CAnimatorGdiPlus()
	: m_pOwner(nullptr)
	, m_pCurAnim(nullptr)
	, m_bRepeat(false)
{
}

CAnimatorGdiPlus::~CAnimatorGdiPlus()
{
	SafeDeleteMap(m_mapAnim);
}

void CAnimatorGdiPlus::CreateAnimation(const wstring& _strName, CGdiPlus* _pBitmap, Vec2 _vLeftTop, Vec2 _vSliceSize, Vec2 _vStep, float _fDuration, UINT _iFrameCount, int _id)
{
    CAnimationGdiPlus* pAnim = FindAnimation(_strName);
    assert(pAnim == nullptr);

    pAnim = new CAnimationGdiPlus;
    pAnim->SetName(_strName);
    pAnim->m_pAnimator = this;

    pAnim->Create(_pBitmap, _vLeftTop, _vSliceSize, _vStep, _fDuration, _iFrameCount);

    m_mapAnim.insert(make_pair(_strName, pAnim));
}

void CAnimatorGdiPlus::CreateAnimation(int _key)
{
    AnimationData* data = (AnimationData*)CDataMgr::GetInstance()->FindData(_key);
    CGdiPlus* pBitmap = CResMgr::GetInstance()->LoadGdiPlus(data->m_strAnimationName, data->m_strTexturePath);

    CAnimationGdiPlus* pAnim = FindAnimation(data->m_strAnimationName);
    assert(pAnim == nullptr); // �̹� ���� �̸��� �ִϸ��̼��� �����ϸ� �ȵǴϱ�, ���� ���� �κ�

    // ���� ���ϸ� �ִϸ��̼� ���� �ϳ� ����
    pAnim = new CAnimationGdiPlus;
    pAnim->m_iID = data->GetKey();

    pAnim->SetName(data->m_strAnimationName);
    pAnim->m_pAnimator = this;

    pAnim->Create(pBitmap, data->m_AniInfo.vLeftTop, data->m_AniInfo.vSliceSize, data->m_AniInfo.vStep, data->m_AniInfo.fDuration, data->m_AniInfo.iFrame);

    m_mapAnim.insert(make_pair(data->m_strAnimationName, pAnim));
}

CAnimationGdiPlus* CAnimatorGdiPlus::FindAnimation(const wstring& _strName)
{
    map<wstring, CAnimationGdiPlus*>::iterator iter = m_mapAnim.find(_strName);
    if (iter == m_mapAnim.end())
        return nullptr;

    return iter->second;
}

CAnimationGdiPlus* CAnimatorGdiPlus::FindAnimation(int _key)
{
    AnimationData* data = (AnimationData*)CDataMgr::GetInstance()->FindData(_key);
    map<wstring, CAnimationGdiPlus*>::iterator iter = m_mapAnim.find(data->m_strAnimationName);
    if (iter == m_mapAnim.end())
        return nullptr;

    return iter->second;
}

void CAnimatorGdiPlus::Play(const wstring& _strName, bool _bRepeat)
{
    m_pCurAnim = FindAnimation(_strName);
    m_bRepeat = _bRepeat;
}

void CAnimatorGdiPlus::Play(int _key, bool _bRepeat)
{
    m_pCurAnim = FindAnimation(_key);
    m_bRepeat = _bRepeat;
}

void CAnimatorGdiPlus::Update()
{
    if (m_pCurAnim != nullptr)
    {
        m_pCurAnim->Update();

        if (m_bRepeat && m_pCurAnim->isFinish())
        {
            if (m_bRepeat) m_pCurAnim->SetFrame(0);
            else AnimationFinish(m_pOwner);
        }
    }
}

void CAnimatorGdiPlus::FinalUpdate()
{
}

void CAnimatorGdiPlus::Render(HDC hdc)
{
    if (m_pCurAnim != nullptr)
        m_pCurAnim->Render(hdc);
}

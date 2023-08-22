#include "pch.h"
#include "CAnimationGDIplus.h"

#include "CCore.h"
#include "CTimeMgr.h"
#include "CCamera.h"

#include "CAnimatorGdiPlus.h"
#include "CGdiPlus.h"
#include "CObject.h"

#include "CTexture.h"

CAnimationGdiPlus::CAnimationGdiPlus()
    : m_pAnimator(nullptr)
    , m_pBitmap(nullptr)
    , m_pGraphics(nullptr)
    , m_iCurFrame(0)
    , m_fAccTime(0.f)
    , m_bFinish(false)
    , m_iID(0)
{
    m_pGraphics = new Graphics(CCore::GetInstance()->GetMemTex()->GetDC());
}

CAnimationGdiPlus::~CAnimationGdiPlus()
{
    if (m_pGraphics)
    {
        delete m_pGraphics;
        m_pGraphics = nullptr;
    }
}

void CAnimationGdiPlus::Update()
{
    if (m_bFinish) // 끝난거면 재생 안하게 설정하기
        return;

    // 시간에 따라 애니메이션 변화되게 설정
    m_fAccTime += fDT;	// 누적시간, 흘러간 시간

    if (m_vecFrame[m_iCurFrame].fDuration < m_fAccTime)
    {
        ++m_iCurFrame;

        // 사이즈랑 동일하면 다시 돌아가게
        if (m_vecFrame.size() <= m_iCurFrame)
        {
            m_iCurFrame = -1;
            m_bFinish = true;
            m_fAccTime = 0.f;
            return;
        }

        m_fAccTime = m_fAccTime - m_vecFrame[m_iCurFrame].fDuration;
    }
}

void CAnimationGdiPlus::Render(HDC hdc)
{
    if (m_bFinish)
        return;

    CObject* pObj = m_pAnimator->GetObj();
    Vec2 vPos = pObj->GetPos();

    vPos += m_vecFrame[m_iCurFrame].vOffset;

    vPos = CCamera::GetInstance()->GetRenderPos(vPos);

    Rect destRect((int)(vPos.x - m_vecFrame[m_iCurFrame].vSlice.x / 2.f),
        (int)(vPos.y - m_vecFrame[m_iCurFrame].vSlice.y / 2.f),
        (int)m_vecFrame[m_iCurFrame].vSlice.x,
        (int)m_vecFrame[m_iCurFrame].vSlice.y);

    m_pGraphics->DrawImage(m_pBitmap->GetBitmap(), destRect,
        (int)m_vecFrame[m_iCurFrame].vLeftTop.x,
        (int)m_vecFrame[m_iCurFrame].vLeftTop.y,
        (int)m_vecFrame[m_iCurFrame].vSlice.x,
        (int)m_vecFrame[m_iCurFrame].vSlice.y,
        Gdiplus::UnitPixel);
}

void CAnimationGdiPlus::Create(CGdiPlus* _pBitmap, Vec2 _vLeftTop, Vec2 _vSliceSize, Vec2 _vStep, float _fDuration, UINT _iFrameCount)
{
    m_pBitmap = _pBitmap;

    tAnimFrame frm = {};
    for (int i = 0; i < (int)_iFrameCount; ++i)
    {
        frm.fDuration = _fDuration;
        frm.vSlice = _vSliceSize;
        frm.vLeftTop = _vLeftTop + _vStep * i;

        m_vecFrame.push_back(frm);
    }
}

#include "pch.h"
#include "CAnimationGDIplus.h"

#include "CTimeMgr.h"
#include "CCamera.h"

#include "CAnimatorGdiPlus.h"
#include "CGdiPlus.h"
#include "CObject.h"

CAnimationGdiPlus::CAnimationGdiPlus()
    : m_pAnimator(nullptr)
    , m_pBitmap(nullptr)
    , m_iCurFrame(0)
    , m_fAccTime(0.f)
    , m_bFinish(false)
    , m_iID(0)
{
}

CAnimationGdiPlus::~CAnimationGdiPlus()
{
}

void CAnimationGdiPlus::Update()
{
}

void CAnimationGdiPlus::Render(HDC hdc)
{
    if (m_bFinish)
        return;

    CObject* pObj = m_pAnimator->GetObj();
    Vec2 vPos = pObj->GetPos();

    vPos += m_vecFrame[m_iCurFrame].vOffset;

    vPos = CCamera::GetInstance()->GetRenderPos(vPos);

    Graphics graphics(hdc);
    Rect destRect((int)(vPos.x - m_vecFrame[m_iCurFrame].vSlice.x / 2.f),
        (int)(vPos.y - m_vecFrame[m_iCurFrame].vSlice.y / 2.f),
        (int)m_vecFrame[m_iCurFrame].vSlice.x,
        (int)m_vecFrame[m_iCurFrame].vSlice.y);

    graphics.DrawImage(m_pBitmap->GetBitmap(), destRect,
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

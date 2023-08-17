#include "pch.h"
#include "CAnimation.h"

#include "CTimeMgr.h"
#include "CCamera.h"

#include "CAnimator.h"
#include "CTexture.h"
#include "CObject.h"

CAnimation::CAnimation()
	: m_pAnimator(nullptr)
	, m_pTex(nullptr)
	, m_iCurFrame(0)
	, m_fAccTime(0.f)
	, m_bFinish(false)
{
}

CAnimation::~CAnimation()
{
}

void CAnimation::Update()
{
	if (m_bFinish) // �����Ÿ� ��� ���ϰ� �����ϱ�
		return;

	// �ð��� ���� �ִϸ��̼� ��ȭ�ǰ� ����
	m_fAccTime += fDT;	// �����ð�, �귯�� �ð�

	if (m_vecFrame[m_iCurFrame].fDuration < m_fAccTime)
	{
		++m_iCurFrame;
		
		// ������� �����ϸ� �ٽ� ���ư���
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

void CAnimation::Render(HDC hdc)
{
	if (m_bFinish) // �����Ÿ� ��� ���ϰ� �����ϱ�
		return;

	CObject* pObj = m_pAnimator->GetObj();
	Vec2 vPos = pObj->GetPos();

	vPos += m_vecFrame[m_iCurFrame].vOffset; // offset��ŭ �߰� �̵���ġ (�ǹ� ����)

	// ������ ��ǥ�� ����
	vPos = CCamera::GetInstance()->GetRenderPos(vPos);

	// ������DC, ������������ǥ, ������ǥ�� �ʺ�-����, �ؽ���dc, 
	// �����ؽ��Ŀ��� ����������ǥ, �����ؽ��� �ʺ�-����, �÷�Ű)
	TransparentBlt(hdc
		, (int)(vPos.x - m_vecFrame[m_iCurFrame].vSlice.x / 2.f)
		, (int)(vPos.y - m_vecFrame[m_iCurFrame].vSlice.y / 2.f)
		, (int)m_vecFrame[m_iCurFrame].vSlice.x
		, (int)m_vecFrame[m_iCurFrame].vSlice.y
		, m_pTex->GetDC()
		, (int)m_vecFrame[m_iCurFrame].vLeftTop.x
		, (int)m_vecFrame[m_iCurFrame].vLeftTop.y
		, (int)m_vecFrame[m_iCurFrame].vSlice.x
		, (int)m_vecFrame[m_iCurFrame].vSlice.y
		, RGB(255, 0, 255)
	);
}

void CAnimation::Create(CTexture* _pTex, Vec2 _vLeftTop, Vec2 _vSliceSize, Vec2 _vStep, float _fDuration, UINT _iFrameCount)
{
	m_pTex = _pTex;

	tAnimFrame frm = {};
	for (int i = 0; i < (int)_iFrameCount; ++i)
	{
		frm.fDuration = _fDuration;
		frm.vSlice = _vSliceSize;
		frm.vLeftTop = _vLeftTop + _vStep * i;

		m_vecFrame.push_back(frm);
	}
}

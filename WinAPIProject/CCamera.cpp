#include "pch.h"
#include "CCamera.h"
#include <random>

#include "CCore.h"
#include "CKeyMgr.h"
#include "CTimeMgr.h"
#include "CResMgr.h"
#include "CEventMgr.h"

#include "CObject.h"
#include "CTexture.h"

std::default_random_engine generator;
std::uniform_real_distribution<float> distribution(-1.0, 1.0);

CCamera::CCamera()
	: m_pTargetObj(nullptr)
	, m_fTime(1.0f)

	, m_fVTime(0)
	, m_fSpeed(0)
	, m_fAccTime(0)
	, m_fAccSpeed(0)

	, m_iVibrate(0)
	, m_iVibeCount(0)
	, m_iCurVibrate(0)

	, m_bVibrating(false)
	, m_pVeilTex(nullptr)
{
}

CCamera::~CCamera()
{
}

void CCamera::VibrateCamera()
{
	// ��ǥ ����Ƚ�� ä������
	if (m_iVibrate <= m_iCurVibrate)
	{
		// ���� ��ġ�� �����ֱ� -> ȭ�� ���� ����
		m_bVibrating = false;
		m_fAccTime = m_fTime;
	}
	else
	{
		if (m_fVTime <= m_fAccTime)
		{
			switch (m_iVibeCount)
			{
			case 0:
			{
				m_vCurLookAt = m_vPrevLookAt + m_vRandom * m_fSpeed * fDT;
				m_iVibeCount++;
				m_fAccTime = 0.f;
			}
			break;
			case 1:
			{
				m_vCurLookAt = m_vPrevLookAt + m_vRandom * -2.f * m_fSpeed * fDT;
				m_iVibeCount++;
				m_fAccTime = 0.f;
			}
			break;
			case 2:
			{
				m_vCurLookAt = m_vPrevLookAt + m_vRandom * m_fSpeed * fDT;
				m_iVibeCount = 0;
				m_iCurVibrate++;
				m_fAccTime = 0.f;

				m_vRandom.x = distribution(generator);
				m_vRandom.y = distribution(generator);
				m_vRandom.Normalize();
			}
			break;
			}
		}
	}
}

void CCamera::SetLookAt(Vec2 _vLook)
{
	// ī�޶� �̵��� �ɸ��� �ð��� ������ �ִ� ���
	m_vLookAt = _vLook; 
	float fMoveDist = (m_vLookAt - m_vPrevLookAt).Length(); 
	m_fSpeed = fMoveDist / m_fTime * 2.f;
	m_fAccTime = 0.f;
}

void CCamera::SetVibrateCamera(float _fPower, int _iVibrate, float _fTime)
{
	// ���� ����, ���� �ð�
	m_fVTime = _fTime;
	m_iVibrate = _iVibrate;
	m_iCurVibrate = 0;
	m_iVibeCount = 0;
	m_fSpeed = _fPower * 100.f;

	m_vRandom.x = distribution(generator);
	m_vRandom.y = distribution(generator);
	m_vRandom.Normalize();

	m_bVibrating = true;
}

void CCamera::Init()
{
	Vec2 vResolution = CCore::GetInstance()->GetResolution();

	// �̹� ���������� RGB(0,0,0) ������
	m_pVeilTex = CResMgr::GetInstance()->CreateTexture(L"CameraVeil", (UINT)vResolution.x, (UINT)vResolution.y);
}

void CCamera::Update()
{
	if (m_pTargetObj)
	{
		if (m_pTargetObj->IsDead())
		{
			m_pTargetObj = nullptr;
		}
		else
		{
			m_vLookAt = m_pTargetObj->GetPos();
		}
	}
	CalDiif();
}

void CCamera::Render(HDC hdc)
{
	// ���̵���, ���̵�ƿ� ��⿭�� ������
	if (m_listCamEffect.empty())
		return;

	tCamEffect& effect = m_listCamEffect.front();
	effect.fCurTime += fDT;

	float fRatio = 0.f;	// ����Ʈ ���� ����
	fRatio = effect.fCurTime / effect.fDuration;

	// ������ 0�� 1 ���̷� ���� ����
	if (fRatio < 0.f)
		fRatio = 0.f;
	if (fRatio > 1.f)
		fRatio = 1.f;

	static int iAlpha;
	if (CAM_EFFECT::FADE_OUT == effect.eEffect)
	{
		iAlpha = (int)(255.f * fRatio);
	}
	else if (CAM_EFFECT::FADE_IN == effect.eEffect)
	{
		iAlpha = (int)(255.f * (1.f - fRatio));
	}
	else if (CAM_EFFECT::BLACK == effect.eEffect)
	{
		iAlpha = (int)255.f;
	}
	else if (CAM_EFFECT::EVENT == effect.eEffect)
	{
		// ����
	}

	BLENDFUNCTION bf = {};
	bf.BlendOp = AC_SRC_OVER;
	bf.BlendFlags = 0;
	bf.AlphaFormat = 0;		// AC_SRC_ALPHA �̰� �ƴ� 0���� ���� -> ����ä���̶� ����ϸ� �ȵ�
	bf.SourceConstantAlpha = iAlpha;

	AlphaBlend(hdc
		, 0
		, 0
		, m_pVeilTex->Width()
		, m_pVeilTex->Height()
		, m_pVeilTex->GetDC()
		, 0, 0, m_pVeilTex->Width(), m_pVeilTex->Height()
		, bf);

	// ����ð��� ����Ʈ �ִ� ���� �ð��� �Ѿ��
	if (effect.fDuration < effect.fCurTime)
	{
		if (CAM_EFFECT::EVENT == effect.eEffect)
		{
			CameraEvent();
		}
		m_listCamEffect.pop_front();
		return;
	}
}

void CCamera::CalDiif()
{
	// ���� LookAt�� ���� Look�� ���̰� �����ؼ� ������ LookAt ���ϱ�
	m_fAccTime += fDT;
	m_fAccSpeed = m_fSpeed * (m_fAccTime / m_fTime); // ���ӵ� ����
	if (m_bVibrating)
	{
		VibrateCamera();
	}
	else
	{
		// ��ǥ �ð��� �Ѿ�����
		if (m_fTime <= m_fAccTime)
		{
			// ���� ��ġ�� �����ֱ� -> ȭ�� ���� ����
			m_vCurLookAt = m_vLookAt;
		}
		else
		{
			Vec2 vLookDir = m_vLookAt - m_vPrevLookAt;

			if (!vLookDir.IsZero())
			{
				m_vCurLookAt = m_vPrevLookAt + vLookDir.Normalize() * (m_fSpeed - m_fAccSpeed) * fDT;
			}
		}
	}

	Vec2 vResolution = CCore::GetInstance()->GetResolution();
	Vec2 vCenter = vResolution / 2;

	m_vDiff = m_vCurLookAt - vCenter;
	m_vPrevLookAt = m_vCurLookAt;
}

void CCamera::CameraEvent()
{
	tEvent evn = {  };
	evn.eEvent = EVENT_TYPE::CAMERA_EVENT;

	CEventMgr::GetInstance()->AddEvent(evn);
}

#pragma once

class CObject;

class CTexture;

enum class CAM_EFFECT
{
	FADE_IN,
	FADE_OUT,
	BLACK,
	EVENT,
	NONE,
};

struct tCamEffect
{
	CAM_EFFECT	eEffect;		// 카메라 효과
	float		fDuration;		// 효과 최대 진행시간
	float		fCurTime;		// 카메라 효과 현재 진행시간
};

class CCamera
{
	SINGLE(CCamera);
private:
	Vec2		m_vLookAt;		// 카메라가 보려고 하는 목표 카메라 위치 (오브젝트의 위치와는 다름, 실제좌표와 출력좌표)
	Vec2		m_vCurLookAt;	// 현재 프레임이 보고 있던 위치
	Vec2		m_vPrevLookAt;	// 이전 프레임이 보고 있던 위치

	CObject*	m_pTargetObj;	// 카메라가 특정 대상을 따라감
	Vec2		m_vDiff;		// 해상도 중심 위치와, 카메라 LookAt간의 차이값 계산값

	float		m_fTime;		// 카메라가 타겟 위치까지 가기위해 걸리는 최종 시간
	float		m_fSpeed;		// 타겟을 따라가는 속도
	float		m_fAccSpeed;	// 평균 가속도
	float		m_fAccTime;		// 타겟 따라간 누적 시간

	Vec2		m_vRandom;		// 랜덤 진동 위치
	bool		m_bVibrating;	// 카메라 진동중
	float		m_fVTime;		// 진동 간격
	int			m_iVibrate;		// 목표 진동 횟수
	int			m_iCurVibrate;	// 현재 진동 횟수
	int			m_iVibeCount;	// 1단계, 2단계, 3단계

	list<tCamEffect>	m_listCamEffect;	// 페이드인, 페이드아웃이 연속적으로 진행될 수 있게 배열에 저장해두기
	CTexture*	m_pVeilTex;		// 가림막 텍스쳐 (검은색 -> fade in, fade out)

public:
	CCamera();
	~CCamera();

public:
	bool IsVibrating() { return m_bVibrating; }
	Vec2 GetLookAt() { return m_vCurLookAt; }
	// 오브젝트 실제 좌표와 렌더링 좌표의 차이를 보정해줘야한다
	// 마우스 위치 계산할때는 렌더링 좌표 기준으로 계산하니까, 오브젝트와 반대로 계산
	Vec2 GetRenderPos(Vec2 _vObjPos) { return _vObjPos - m_vDiff; }
	Vec2 GetRealPos(Vec2 _vRenderPos) { return _vRenderPos + m_vDiff; } 

public:
	void VibrateCamera();

public:
	void SetLookAt(Vec2 _vLook);
	void SetTarget(CObject* _pTarget) { m_pTargetObj = _pTarget; }
	void SetVibrateCamera(float _fPower, int _iVibrate, float _fTime);

public:
	void FadeIn(float _fDuration)
	{
		tCamEffect ef = {};
		ef.eEffect = CAM_EFFECT::FADE_IN;
		ef.fDuration = _fDuration;
		ef.fCurTime = 0.f;
		if (ef.fDuration == 0.f)
			assert(nullptr);

		m_listCamEffect.push_back(ef);
	}

	void FadeOut(float _fDuration) 
	{ 
		tCamEffect ef = {};
		ef.eEffect = CAM_EFFECT::FADE_OUT;
		ef.fDuration = _fDuration;
		ef.fCurTime = 0.f;
		if (ef.fDuration == 0.f)
			assert(nullptr);
		
		m_listCamEffect.push_back(ef);
	}

	void BlackScreen(float _fDuration)
	{
		tCamEffect ef = {};
		ef.eEffect = CAM_EFFECT::BLACK;
		ef.fDuration = _fDuration;
		ef.fCurTime = 0.f;
		if (ef.fDuration == 0.f)
			assert(nullptr);

		m_listCamEffect.push_back(ef);
	}

	void Event(float _fDuration)
	{
		tCamEffect ef = {};
		ef.eEffect = CAM_EFFECT::EVENT;
		ef.fDuration = _fDuration;
		ef.fCurTime = 0.f;
		if (ef.fDuration == 0.f)
			assert(nullptr);

		m_listCamEffect.push_back(ef);
	}

public:
	void Init();
	void Update();
	void Render(HDC hdc);

private:
	void CalDiif();
	void CameraEvent();
};


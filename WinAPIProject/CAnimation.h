#pragma once
#include "tAnimFrame.h"

class CAnimator;
class CTexture;

class CAnimation
{
private:
	wstring				m_strName;
	CAnimator*			m_pAnimator;
	CTexture*			m_pTex;			// 애니메이션이 참조하는 텍스쳐
	vector<tAnimFrame>	m_vecFrame;		// 모든 프레임 정보가 담긴 배열
	int					m_iCurFrame;	// 현재 어떤 프레임?
	float				m_fAccTime;		// 시간 누적

	bool				m_bFinish;		// 애니메이션 끝났는지 체크
	bool				m_bFlip;		// 애니메이션 좌우 반전

	int					m_iID;			// 데이터 아이디

public:
	CAnimation();
	~CAnimation();

	friend class CAnimator;
	friend class CResMgr;

public:
	int GetID() { return m_iID; }
	const wstring& GetName() { return m_strName; }
	tAnimFrame& GetFrame(int _iIndex) { return m_vecFrame[_iIndex]; } // 피벗 수정용, 호출한 쪽에서 수정이 가능하게
	vector<tAnimFrame>& GetAllFrame() { return m_vecFrame; }
	int GetMaxFrame() { return (int)m_vecFrame.size(); }
	CTexture* GetTexture() { return m_pTex; }

	bool isFinish() { return m_bFinish; }
	bool isFliped() const { return m_bFlip; }

public:
	void SetFrame(int _iFrameIndex) { m_iCurFrame = _iFrameIndex; m_bFinish = false; m_fAccTime = 0.f; }
	void SetFlip(bool _bFlip) { m_bFlip = _bFlip; }

private:
	void SetName(const wstring& _strName) { m_strName = _strName; }

public:
	void Update();
	void Render(HDC hdc);
	void Create(CTexture* _pTex, Vec2 _vLeftTop, Vec2 _vSliceSize, Vec2 _vStep, float _fDuration, UINT _iFrameCount);
};


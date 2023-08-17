#pragma once

class CAnimator;
class CTexture;

struct tAnimFrame
{
	Vec2	vLeftTop;	// 좌상단 좌표 정보
	Vec2	vSlice;		// vLeftTop 기준으로 얼마큼 크기만큼 자를건지?
	Vec2	vOffset;	// 피벗 위치 (실제 좌표)
	float	fDuration;	// 프레임 지속시간
};

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

public:
	CAnimation();
	~CAnimation();

	friend class CAnimator;

public:
	const wstring& GetName() { return m_strName; }
	tAnimFrame& GetFrame(int _iIndex) { return m_vecFrame[_iIndex]; } // 피벗 수정용, 호출한 쪽에서 수정이 가능하게
	int GetMaxFrame() { return (int)m_vecFrame.size(); }

	bool isFinish() { return m_bFinish; }

public:
	void SetFrame(int _iFrameIndex) { m_iCurFrame = _iFrameIndex; m_bFinish = false; m_fAccTime = 0.f; }

private:
	void SetName(const wstring& _strName) { m_strName = _strName; }

public:
	void Update();
	void Render(HDC hdc);
	void Create(CTexture* _pTex, Vec2 _vLeftTop, Vec2 _vSliceSize, Vec2 _vStep, float _fDuration, UINT _iFrameCount);
};


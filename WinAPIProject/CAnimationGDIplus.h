#pragma once

class CAnimator;
class CGdiPlus;

struct tAnimFrame
{
	Vec2	vLeftTop;	// 좌상단 좌표 정보
	Vec2	vSlice;		// vLeftTop 기준으로 얼마큼 크기만큼 자를건지?
	Vec2	vOffset;	// 피벗 위치 (실제 좌표)
	float	fDuration;	// 프레임 지속시간
};

class CAnimationGdiPlus
{
private:
	wstring				m_strName;
	CAnimator*			m_pAnimator;
	CGdiPlus*			m_pBitmap;			// 애니메이션이 참조하는 GDI+
	vector<tAnimFrame>	m_vecFrame;		// 모든 프레임 정보가 담긴 배열
	int					m_iCurFrame;	// 현재 어떤 프레임?
	float				m_fAccTime;		// 시간 누적

	bool				m_bFinish;		// 애니메이션 끝났는지 체크

	int					m_iID;			// 데이터 아이디

public:
	CAnimationGdiPlus();
	~CAnimationGdiPlus();

	friend class CAnimator;
	friend class CResMgr;

public:
	int GetID() { return m_iID; }
	const wstring& GetName() { return m_strName; }
	tAnimFrame& GetFrame(int _iIndex) { return m_vecFrame[_iIndex]; } // 피벗 수정용, 호출한 쪽에서 수정이 가능하게
	vector<tAnimFrame>& GetAllFrame() { return m_vecFrame; }
	int GetMaxFrame() { return (int)m_vecFrame.size(); }
	CGdiPlus* GetBitmap() { return m_pBitmap; }

	bool isFinish() { return m_bFinish; }

public:
	void SetFrame(int _iFrameIndex) { m_iCurFrame = _iFrameIndex; m_bFinish = false; m_fAccTime = 0.f; }

private:
	void SetName(const wstring& _strName) { m_strName = _strName; }

public:
	void Update();
	void Render(HDC hdc);
	void Create(CGdiPlus* _pTex, Vec2 _vLeftTop, Vec2 _vSliceSize, Vec2 _vStep, float _fDuration, UINT _iFrameCount);
};


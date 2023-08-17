#pragma once

class CObject;
class CTexture;
class CAnimation;

class CAnimator
{
private:
	map<wstring, CAnimation*>	m_mapAnim;		// 모든 애니메이션 목록
	CAnimation*					m_pCurAnim;		// 현재 재생중인 애니메이션
	CObject*					m_pOwner;		// 애니메이션 소유 오브젝트

	bool						m_bRepeat;		// 반복재생 여부

public:
	CAnimator();
	~CAnimator();

	friend class CObject;

public:
	CObject* GetObj() { return m_pOwner; }

public:
	void CreateAnimation(const wstring& _strName, CTexture* _pTex, Vec2 _vLeftTop, Vec2 _vSliceSize, Vec2 _vStep, float _fDuration, UINT _iFrameCount);
	CAnimation* FindAnimation(const wstring& _strName);
	void Play(const wstring& s, bool _bRepeat);

public:
	void Update();
	void FinalUpdate();
	void Render(HDC hdc);
};


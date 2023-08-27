#pragma once

class CObject;
class CTexture;
class CAnimation;

class CAnimator
{
private:
	CObject*					m_pOwner;		// �ִϸ��̼� ���� ������Ʈ
	bool						m_bRepeat;		// �ݺ���� ����
	bool						m_eventTrigger;	// �̺�Ʈ �ߺ� ���� ����
	int							m_iID;			// ������ ���̵�

	map<wstring, CAnimation*>	m_mapAnim;				// ��� �ִϸ��̼� ���
	CAnimation*					m_pCurAnim;				// ���� ������� �ִϸ��̼�

public:
	CAnimator();
	CAnimator(const CAnimator& _origin);
	~CAnimator();

	friend class CObject;

public:
	void AnimationEvent(CObject* _pObj);
	void AnimationEnd(CObject* _pObj);

public:
	int GetID() { return m_iID; }
	CAnimation* GetAnimation() { return m_pCurAnim; }
	CObject* GetObj() { return m_pOwner; }

public:
	void CreateAnimation(const wstring& _strName, CTexture* _pTex, Vec2 _vLeftTop, Vec2 _vSliceSize, Vec2 _vStep, float _fDuration, UINT _iFrameCount, int _id, int _iEventFrame);
	void CreateAnimation(int _key);
	CAnimation* FindAnimation(const wstring& _strName);
	CAnimation* FindAnimation(int _key);
	CAnimation* FindAnimationType(const wstring& _strType);
	void Play(const wstring& s, bool _bRepeat);
	void Play(int _key, bool _bRepeat);
	void PlayType(const wstring& _strName, bool _bRepeat);

public:
	void Update();
	void FinalUpdate();
	void Render(HDC hdc);
};


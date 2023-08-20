#pragma once

class CObject;
class CTexture;
class CAnimation;

class CAnimator
{
private:
	map<wstring, CAnimation*>	m_mapAnim;		// ��� �ִϸ��̼� ���
	CAnimation*					m_pCurAnim;		// ���� ������� �ִϸ��̼�
	CObject*					m_pOwner;		// �ִϸ��̼� ���� ������Ʈ

	bool						m_bRepeat;		// �ݺ���� ����

public:
	CAnimator();
	~CAnimator();

	friend class CObject;

public:
	CObject* GetObj() { return m_pOwner; }

public:
	void CreateAnimation(const wstring& _strName, CTexture* _pTex, Vec2 _vLeftTop, Vec2 _vSliceSize, Vec2 _vStep, float _fDuration, UINT _iFrameCount);
	void CreateAnimation(int _key);
	CAnimation* FindAnimation(const wstring& _strName);
	CAnimation* FindAnimation(int _key);
	void Play(const wstring& s, bool _bRepeat);
	void Play(int _key, bool _bRepeat);

public:
	void Update();
	void FinalUpdate();
	void Render(HDC hdc);
};


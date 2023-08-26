#pragma once

class CObject;
class CGdiPlus;
class CAnimationGdiPlus;

class CAnimatorGdiPlus
{
private:
	CObject*					m_pOwner;		// �ִϸ��̼� ���� ������Ʈ
	bool						m_bRepeat;		// �ݺ���� ����
	bool						m_eventTrigger;	// �̺�Ʈ �ߺ� ���� ����
	int							m_iID;			// ������ ���̵�

	map<wstring, CAnimationGdiPlus*>	m_mapAnim;
	CAnimationGdiPlus*					m_pCurAnim;

public:
	CAnimatorGdiPlus();
	CAnimatorGdiPlus(const CAnimatorGdiPlus& _origin);
	~CAnimatorGdiPlus();

	friend class CObject;

public:
	void AnimationEvent(CObject* _pObj);

public:
	int GetID() { return m_iID; }
	CAnimationGdiPlus* GetAnimation() { return m_pCurAnim; }
	CObject* GetObj() { return m_pOwner; }

public:
	void CreateAnimation(const wstring& _strName, CGdiPlus* _pTex, Vec2 _vLeftTop, Vec2 _vSliceSize, Vec2 _vStep, float _fDuration, UINT _iFrameCount, int _id, int _eventID);
	void CreateAnimation(int _key);
	CAnimationGdiPlus* FindAnimation(const wstring& _strName);
	CAnimationGdiPlus* FindAnimation(int _key);
	void Play(const wstring& _strName, bool _bRepeat);
	void Play(int _key, bool _bRepeat);

public:
	void Update();
	void FinalUpdate();
	void Render(HDC hdc);
};


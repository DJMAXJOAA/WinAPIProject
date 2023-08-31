#pragma once
#include "tAnimFrame.h"

class CAnimatorGdiPlus;
class CGdiPlus;

class CAnimationGdiPlus
{
private:
	wstring				m_strName;
	CAnimatorGdiPlus*	m_pAnimator;
	CGdiPlus*			m_pBitmap;		// �ִϸ��̼��� �����ϴ� GDI+
	vector<tAnimFrame>	m_vecFrame;		// ��� ������ ������ ��� �迭
	int					m_iCurFrame;	// ���� � ������?
	int					m_iEventFrame;	// �̺�Ʈ�� ��� �����ӿ� ȣ��Ǵ���?
	float				m_fAccTime;		// �ð� ����

	bool				m_bFinish;		// �ִϸ��̼� �������� üũ

	int					m_iID;			// ������ ���̵�

public:
	CAnimationGdiPlus();
	CAnimationGdiPlus(const CAnimationGdiPlus& _origin);
	~CAnimationGdiPlus();

	friend class CAnimatorGdiPlus;
	friend class CResMgr;

public:
	int GetID() { return m_iID; }
	const wstring& GetName() { return m_strName; }
	tAnimFrame& GetFrame(int _iIndex) { return m_vecFrame[_iIndex]; } // �ǹ� ������, ȣ���� �ʿ��� ������ �����ϰ�
	vector<tAnimFrame>& GetAllFrame() { return m_vecFrame; }
	int GetMaxFrame() { return (int)m_vecFrame.size(); }
	int GetCurrentFrame() { return m_iCurFrame; }
	int GetEventFrame() { return m_iEventFrame; }
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


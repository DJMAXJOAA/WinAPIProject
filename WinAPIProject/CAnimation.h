#pragma once
#include "tAnimFrame.h"

class CAnimator;
class CTexture;

class CAnimation
{
private:
	wstring				m_strName;
	CAnimator*			m_pAnimator;
	CTexture*			m_pTex;			// �ִϸ��̼��� �����ϴ� �ؽ���
	vector<tAnimFrame>	m_vecFrame;		// ��� ������ ������ ��� �迭
	int					m_iCurFrame;	// ���� � ������?
	float				m_fAccTime;		// �ð� ����

	bool				m_bFinish;		// �ִϸ��̼� �������� üũ
	bool				m_bFlip;		// �ִϸ��̼� �¿� ����

	int					m_iID;			// ������ ���̵�

public:
	CAnimation();
	CAnimation(const CAnimation& _origin);
	~CAnimation();

	friend class CAnimator;
	friend class CResMgr;

public:
	int GetID() { return m_iID; }
	const wstring& GetName() { return m_strName; }
	tAnimFrame& GetFrame(int _iIndex) { return m_vecFrame[_iIndex]; } // �ǹ� ������, ȣ���� �ʿ��� ������ �����ϰ�
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


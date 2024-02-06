#pragma once
#include "tAnimFrame.h"

class CAnimator;
class CTexture;

class CAnimation
{
private:
	wstring				m_strName;
	wstring				m_strType;

	CAnimator*			m_pAnimator;	// �� �ִϸ�����
	CTexture*			m_pTex;			// �ִϸ��̼��� �����ϴ� �ؽ���
	vector<tAnimFrame>	m_vecFrame;		// ��� ������ ������ ��� �迭

	int					m_iCurFrame;	// ���� � ������?
	int					m_iEventFrame;	// �̺�Ʈ�� ��� �����ӿ� ȣ��Ǵ���?
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
	const wstring& GetType() { return m_strType; }
	tAnimFrame& GetFrame(int _iIndex) { return m_vecFrame[_iIndex]; } // �ǹ� ������, ȣ���� �ʿ��� ������ �����ϰ�
	vector<tAnimFrame>& GetAllFrame() { return m_vecFrame; }
	int GetMaxFrame() { return (int)m_vecFrame.size(); }
	int GetCurrentFrame() { return m_iCurFrame; }
	int GetEventFrame() { return m_iEventFrame; }
	CTexture* GetTexture() { return m_pTex; }

	bool isFinish() { return m_bFinish; }
	bool isFliped() const { return m_bFlip; }

public:
	void SetFrame(int _iFrameIndex) { m_iCurFrame = _iFrameIndex; m_bFinish = false; m_fAccTime = 0.f; }
	void SetFlip(bool _bFlip) { m_bFlip = _bFlip; }

private:
	void SetName(const wstring& _strName) { m_strName = _strName; }
	void SetType(const wstring& _strType) { m_strType = _strType; }

public:
	void Update();
	void Render(HDC hdc);
	void Create(CTexture* _pTex, Vec2 _vLeftTop, Vec2 _vSliceSize, Vec2 _vStep, float _fDuration, UINT _iFrameCount);
};


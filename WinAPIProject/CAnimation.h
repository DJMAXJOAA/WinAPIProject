#pragma once

class CAnimator;
class CTexture;

struct tAnimFrame
{
	Vec2	vLeftTop;	// �»�� ��ǥ ����
	Vec2	vSlice;		// vLeftTop �������� ��ŭ ũ�⸸ŭ �ڸ�����?
	Vec2	vOffset;	// �ǹ� ��ġ (���� ��ǥ)
	float	fDuration;	// ������ ���ӽð�
};

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

public:
	CAnimation();
	~CAnimation();

	friend class CAnimator;

public:
	const wstring& GetName() { return m_strName; }
	tAnimFrame& GetFrame(int _iIndex) { return m_vecFrame[_iIndex]; } // �ǹ� ������, ȣ���� �ʿ��� ������ �����ϰ�
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


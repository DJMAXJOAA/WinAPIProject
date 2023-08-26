#pragma once
#include "CObject.h"

class CToolTest1 :
	public CObject
{
private:
	int		m_iID;

public:
	CToolTest1();
	CToolTest1(int iAnimator, int iAnimation);
	virtual ~CToolTest1();

	CLONE(CToolTest1)

public:
	void ChangeAnimation(int _key);
	void ChangeAnimator(int _key);

public:
	virtual void Render(HDC hdc);
	virtual void Update();
};


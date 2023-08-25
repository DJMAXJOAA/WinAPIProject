#pragma once
#include "CObject.h"
class CToolTest2 :
    public CObject
{
private:
	int		m_iID;

public:
	CToolTest2();
	CToolTest2(int iAnimator, int iAnimation);
	virtual ~CToolTest2();

	CLONE(CToolTest2);

public:
	void ChangeAnimation(int _key);
	void ChangeAnimator(int _key);

public:
	virtual void Render(HDC hdc);
	virtual void Update();
};


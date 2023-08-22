#pragma once
#include "CObject.h"

class CToolTest :
	public CObject
{
private:
	int		m_iID;

public:
	CToolTest();
	virtual ~CToolTest();

public:
	void ChangeAnimation(int _key);
	void ChangeAnimator(int _key);

	CLONE(CToolTest);
public:
	virtual void Render(HDC hdc);
	virtual void Update();
};


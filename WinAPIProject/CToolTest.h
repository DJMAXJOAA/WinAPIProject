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

	CLONE(CToolTest);

public:
	virtual void AnimationFinishEvent() override {}

public:
	void ChangeAnimation(int _key);
	void ChangeAnimator(int _key);

public:
	virtual void Render(HDC hdc);
	virtual void Update();
};


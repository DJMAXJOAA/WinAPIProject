#pragma once
#include "CObject.h"

class CBackground :
	public CObject
{
private:
	Image*		m_image;

	CLONE(CBackground)

public:
	CBackground(wstring _filename);
	~CBackground();

public:
	virtual void Render(HDC hdc) override;
	virtual void Update() override;
};


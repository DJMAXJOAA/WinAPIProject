#pragma once
#include "CData.h"

class AnimatorData
	: public CData
{
private:
	wstring				m_strAnimatorName;	// 애니메이터 이름
	vector<wstring>		m_strAnimation;		// 가져올 애니메이션 이름 명단

public:
	AnimatorData(int _key);
	~AnimatorData();

public:
	virtual void LoadData() override;
};


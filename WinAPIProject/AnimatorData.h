#pragma once
#include "CData.h"

class AnimatorData
	: public CData
{
public:
	wstring			m_strAnimatorName;	// 애니메이터 이름
	vector<int>		m_vecAnimation;		// 가져올 애니메이션 ID 명단

public:
	AnimatorData(int _key);
	~AnimatorData();

public:
	virtual void ParseData(const json& item) override;
};


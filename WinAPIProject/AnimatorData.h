#pragma once
#include "CData.h"

class AnimatorData
	: public CData
{
public:
	wstring			m_strAnimatorName;	// �ִϸ����� �̸�
	vector<int>		m_vecAnimation;		// ������ �ִϸ��̼� ID ���

public:
	AnimatorData(int _key);
	~AnimatorData();

public:
	virtual void ParseData(const json& item) override;
};


#pragma once
#include "CData.h"

class AnimatorData
	: public CData
{
private:
	string				m_strAnimatorName;	// �ִϸ����� �̸�
	vector<string>		m_strAnimation;		// ������ �ִϸ��̼� �̸� ���

public:
	AnimatorData(int _key);
	~AnimatorData();

public:
	virtual void LoadData() override;
};


#pragma once
#include "CData.h"

class AnimatorData
	: public CData
{
private:
	wstring				m_strAnimatorName;	// �ִϸ����� �̸�
	vector<wstring>		m_strAnimation;		// ������ �ִϸ��̼� �̸� ���

public:
	AnimatorData(int _key);
	~AnimatorData();

public:
	virtual void LoadData() override;
};


#pragma once
#include "CData.h"

class ItemData :
	public CData
{
	string		m_strIconKey;	// �ؽ��� Ű��
	float		m_fHP;
	float		m_fAtt;			// ������ ����� ������

public:
	ItemData(int _key);
	~ItemData();

public:
	virtual void LoadData() override;
};


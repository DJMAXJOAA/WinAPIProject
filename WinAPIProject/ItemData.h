#pragma once
#include "CData.h"

class ItemData :
	public CData
{
	wstring		m_itemName;		// �� �̸�
	wstring		m_strIconKey;	// �ؽ��� Ű��
	GRADE		m_itemGrade;	// ���
	ITEM_TYPE	m_itemType;		// ������ Ÿ�� (�ߺ� ���� �Ұ�)
	float		m_fHP;			// ü�� ������
	float		m_fAtt;			// ���ݷ� ������

public:
	ItemData(int _key);
	~ItemData();

public:
	virtual void ParseData(const json& item) override;
};


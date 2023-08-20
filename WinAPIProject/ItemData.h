#pragma once
#include "CData.h"

class ItemData :
	public CData
{
	wstring		m_itemName;		// 템 이름
	wstring		m_strIconKey;	// 텍스쳐 키값
	float		m_fHP;
	float		m_fAtt;			// 아이템 착용시 증감값

public:
	ItemData(int _key);
	~ItemData();

public:
	virtual void ParseData(const json& item) override;
};


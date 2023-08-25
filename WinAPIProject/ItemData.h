#pragma once
#include "CData.h"

class ItemData :
	public CData
{
	wstring		m_itemName;		// 템 이름
	wstring		m_strIconKey;	// 텍스쳐 키값
	GRADE		m_itemGrade;	// 등급
	ITEM_TYPE	m_itemType;		// 아이템 타입 (중복 착용 불가)
	float		m_fHP;			// 체력 증감값
	float		m_fAtt;			// 공격력 증감값

public:
	ItemData(int _key);
	~ItemData();

public:
	virtual void ParseData(const json& item) override;
};


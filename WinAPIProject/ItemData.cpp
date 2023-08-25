#include "pch.h"
#include "ItemData.h"

ItemData::ItemData(int _key)
	: CData(_key)
{
	LoadData(L"data\\300.json");
}

ItemData::~ItemData()
{

}

void ItemData::ParseData(const json& item)
{
    m_itemName = string_to_wstring(item["Name"].get<string>());
    m_strIconKey = string_to_wstring(item["Path"].get<string>());

    string grade = item["Grade"].get<string>();
    if (grade == "common")
        m_itemGrade = GRADE::COMMON;
    else if (grade == "rare")
        m_itemGrade = GRADE::RARE;
    else if (grade == "epic")
        m_itemGrade = GRADE::EPIC;
    else if (grade == "unique")
        m_itemGrade = GRADE::UNIQUE;

    string itemType = item["ItemInfo"]["ItemType"].get<string>();
    if (itemType == "Armor")
        m_itemType = ITEM_TYPE::ARMOR;
    else if (itemType == "Boots")
        m_itemType = ITEM_TYPE::BOOTS;
    else if (itemType == "Bottom")
        m_itemType = ITEM_TYPE::BOTTOM;
    else if (itemType == "Gloves")
        m_itemType = ITEM_TYPE::GLOVES;
    else if (itemType == "Helmet")
        m_itemType = ITEM_TYPE::HELMET;
    else if (itemType == "Axe")
        m_itemType = ITEM_TYPE::AXE;
    else if (itemType == "Bow")
        m_itemType = ITEM_TYPE::BOW;
    else if (itemType == "Spear")
        m_itemType = ITEM_TYPE::SPEAR;
    else if (itemType == "Staff")
        m_itemType = ITEM_TYPE::STAFF;
    else if (itemType == "Sword")
        m_itemType = ITEM_TYPE::SWORD;

    m_fHP = item["ItemInfo"]["HP"].get<float>();
    m_fAtt = item["ItemInfo"]["Att"].get<float>();
}
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
}
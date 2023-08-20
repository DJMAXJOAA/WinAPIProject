#include "pch.h"
#include "MonsterData.h"

MonsterData::MonsterData(int _key)
	: CData(_key)
{
	LoadData(L"data\\400.json");
}

MonsterData::~MonsterData()
{
}

void MonsterData::ParseData(const json& item)
{
}
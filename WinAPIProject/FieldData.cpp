#include "pch.h"
#include "FieldData.h"

#include "CDataUtils.h"

FieldData::FieldData(int _key)
	: CData(_key)
{
	LoadData(L"data\\600.json");
}

FieldData::~FieldData()
{
}

void FieldData::ParseData(const json& item)
{
	string mapType = item["MapType"].get<string>();
	if (mapType == "common")
		m_MapType = FIELD_TYPE::COMMON;
	else if (mapType == "elite")
		m_MapType = FIELD_TYPE::ELITE;
	else if (mapType == "boss")
		m_MapType = FIELD_TYPE::BOSS;

	string blockType = item["BlockType"].get<string>();
	if (blockType == "brick")
		m_BlockType = BLOCK_TYPE::BRICK;
	else if (blockType == "dirt")
		m_BlockType = BLOCK_TYPE::DIRT;
	else if (blockType == "grass")
		m_BlockType = BLOCK_TYPE::GRASS;
	else if (blockType == "stone")
		m_BlockType = BLOCK_TYPE::STONE;
	else if (blockType == "snow")
		m_BlockType = BLOCK_TYPE::SNOW;

	vector<int> monsterID = item["MonsterID"].get<vector<int>>();
	vector<int> numOfMonster = item["NumberOfMonster"].get<vector<int>>();
	for (int i = 0; i < monsterID.size(); i++)
		m_vecMonsters.push_back(make_pair(monsterID[i], numOfMonster[i]));
	m_iDifficulty = item["DifficultyLevel"].get<int>();
}

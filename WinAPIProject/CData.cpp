#include <codecvt>
#include <locale>
#include "pch.h"
#include "CData.h"

#include "CPathMgr.h"

static std::random_device rd;
static std::mt19937 rng(rd());

CData::CData(int _iID)
	: m_iID(_iID)
	, m_bValid(true)
{

}

CData::~CData()
{
}

void CData::LoadData(const wstring& _filename)
{
	json j;

	wstring strFilePath = CPathMgr::GetInstance()->GetContentPath();
	strFilePath += _filename;

	ifstream inFile(strFilePath);
	if (!inFile.is_open())
	{
		assert(0);
	}
	inFile >> j;
	inFile.close();

	int targetID = GetKey();
	for (auto& item : j)
	{
		if (item["ID"].get<int>() == targetID)
		{
			ParseData(item);
			m_bValid = true;
			return;
		}
	}
	m_bValid = false;
}
#pragma once

#include "json.hpp"
#include<fstream>

using namespace nlohmann;
using std::locale;
using std::ifstream;
using std::ofstream;

class CData
{
private:
	int		m_iID;		// 데이터 id 정보
	bool	m_bValid;	// 데이터 존재 여부

public:
	CData(int _iID);
	virtual ~CData();

public:
	int GetKey() { return m_iID; }
	bool GetValid() { return m_bValid; }

public:
	virtual void LoadData(const wstring& _filename);	// json을 불러와서 데이터 파싱
	virtual CData* FindData() { return this; }
	virtual void ParseData(const json& item) = 0;
};

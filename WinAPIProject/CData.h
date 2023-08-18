#pragma once

#include "json.hpp"
using namespace nlohmann;
#include<fstream>
using std::locale;
using std::ifstream;
using std::ofstream;
#include<ctime>
#include<random>
#include<functional>
using std::function;

class CData
{
private:
	int		m_iID;		// 데이터 id 정보

public:
	CData(int _iID);
	virtual ~CData();

public:
	int GetKey() { return m_iID; }

public:
	virtual CData* FindData() { return this; }
	virtual void LoadData() = 0;	// json을 불러와서 데이터 파싱
};

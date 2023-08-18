#pragma once

#include<fstream>
using std::locale;
using std::ifstream;
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
	virtual CData* FindData() { return this; }
	virtual void LoadData() = 0;		// 데이터 읽기 및 파싱
};

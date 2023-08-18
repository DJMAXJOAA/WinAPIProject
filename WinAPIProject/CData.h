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
	int		m_iID;		// ������ id ����

public:
	CData(int _iID);
	virtual ~CData();

public:
	int GetKey() { return m_iID; }

public:
	virtual CData* FindData() { return this; }
	virtual void LoadData() = 0;	// json�� �ҷ��ͼ� ������ �Ľ�
};

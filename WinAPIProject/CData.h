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
	int		m_iID;		// ������ id ����

public:
	CData(int _iID);
	virtual ~CData();

public:
	virtual CData* FindData() { return this; }
	virtual void LoadData() = 0;		// ������ �б� �� �Ľ�
};

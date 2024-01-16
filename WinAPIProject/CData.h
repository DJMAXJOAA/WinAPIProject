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
	int		m_iID;		// ������ id ����
	bool	m_bValid;	// ������ ���� ����

public:
	CData(int _iID);
	virtual ~CData();

public:
	int GetKey() { return m_iID; }
	bool GetValid() { return m_bValid; }

public:
	virtual void LoadData(const wstring& _filename);	// json�� �ҷ��ͼ� ������ �Ľ�
	virtual CData* FindData() { return this; }
	virtual void ParseData(const json& item) = 0;
};

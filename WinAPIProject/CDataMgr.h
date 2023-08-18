#pragma once
#include <unordered_map>
using std::unordered_map;

class CData;

class CDataMgr
{
	SINGLE(CDataMgr);
private:
	unordered_map<int, CData*>		m_mapDataList;	// ������ �˻��� ���� �ؽø�

private:
	CDataMgr();
	~CDataMgr();

public:
	CData* CreateData(int _key);
	CData* FindData(int _key);
};


#pragma once

class CData;

class CDataMgr
{
	SINGLE(CDataMgr);
private:
	map<int, CData*>		m_mapDataList;	// ������ �˻��� ���� �ؽø�

private:
	CDataMgr();
	~CDataMgr();

public:
	CData* CreateData(int _key);
	CData* FindData(int _key);
};


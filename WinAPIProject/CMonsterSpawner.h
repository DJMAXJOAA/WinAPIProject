#pragma once

class CMonster;
class CMonsterFactory;

class CMonsterSpawner
{
private:
	map<wstring, CMonster*>		m_mapMonsterData;		// Ŭ�п� ���� ������(�˻���)
	vector<CMonsterFactory*>	m_vecSpawnPattern;		// ������������ ���� ���� ����
	
public:
	CMonsterSpawner();
	~CMonsterSpawner();

public:
	void Init();		// ���� ������ ��üȭ���Ѽ� ����

public:
	void Stage1();
	void Stage2();
};


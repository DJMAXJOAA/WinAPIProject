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
	// ���� ���� -> level�� ���� ���̵� ����, �� ��ȯ ��Ģ�� ������ ���� �߿� �������� ����
	// ����, �� ����
	void SpawnMonster(int _level, FIELD_TYPE _type);	

public:
	void SpawnMonsterPattern(int _level, FIELD_TYPE _type);		// �����Ϳ� ����� ���� ���� ������ �ҷ��ͼ�, ������ ��Ŀ� �°� ���� ����
};


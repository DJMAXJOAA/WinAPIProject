#pragma once

class CMonster;
class FieldData;

class CMonsterSpawner
{
private:
	map<int, CMonster*>			m_mapMonsterData;		// Ŭ�п� ���� ������(�˻���)
	
public:
	CMonsterSpawner();
	~CMonsterSpawner();

public:
	void Init();		// ���� ������ ��üȭ���Ѽ� ����

public:
	// ���� ���� -> level�� ���� ���̵� ����, �� ��ȯ ��Ģ�� ������ ���� �߿� �������� ����
	// ����, �� ����
	void SpawnMonster(FieldData* _data);
};


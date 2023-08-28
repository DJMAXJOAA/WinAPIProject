#pragma once

class CMonster;
class FieldData;

class CMonsterSpawner
{
private:
	map<int, CMonster*>			m_mapMonsterData;	// Ŭ�п� ���� ������(�˻���)
	vector<CMonster*>			m_vecMonsters;		// �ʵ忡 �ִ� ���� �迭
	
public:
	CMonsterSpawner();
	~CMonsterSpawner();

public:
	vector<CMonster*>& GetMonsterList() { return m_vecMonsters; }

public:
	// ���� ���� -> level�� ���� ���̵� ����, �� ��ȯ ��Ģ�� ������ ���� �߿� �������� ����
	// ����, �� ����
	void SpawnMonster(FieldData* _data);

public:
	void Init();		// ���� ������ ��üȭ���Ѽ� ����
	void Update();		// ��� ������ �ִ� ���͵��� �迭���� ����

};


#pragma once

class CMonster;
class FieldData;

class CMonsterSpawner
{
private:
	map<int, CMonster*>			m_mapMonsterData;	// Ŭ�п� ���� ������(�˻���)
	vector<CMonster*>			m_vecMonsters;		// �ʵ忡 �ִ� ���� ����Ʈ
	
public:
	CMonsterSpawner();
	~CMonsterSpawner();

public:
	vector<CMonster*>& GetMonsterList() { return m_vecMonsters; }

public:
	void Init();		// ���� ������ ��üȭ���Ѽ� ����

public:
	// ���� ���� -> level�� ���� ���̵� ����, �� ��ȯ ��Ģ�� ������ ���� �߿� �������� ����
	// ����, �� ����
	void SpawnMonster(FieldData* _data);
};


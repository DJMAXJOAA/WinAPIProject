#pragma once

class CMonster;
class FieldData;

class CMonsterSpawner
{
private:
	map<int, CMonster*>			m_mapMonsterData;	// Ŭ�п� ���� ������(�˻���)
	list<CMonster*>				m_lstMonsters;		// �ʵ忡 �ִ� ���� ����Ʈ
	
public:
	CMonsterSpawner();
	~CMonsterSpawner();

public:
	list<CMonster*>& GetMonsterList() { return m_lstMonsters; }

public:
	// ���� ���� -> level�� ���� ���̵� ����, �� ��ȯ ��Ģ�� ������ ���� �߿� �������� ����
	// ����, �� ����
	void SpawnMonster(FieldData* _data);

public:
	void Init();		// ���� ������ ��üȭ���Ѽ� ����
	void Update();		// ��� ������ �ִ� ���͵��� �迭���� ����

};


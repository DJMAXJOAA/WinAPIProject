#pragma once

class CScene_Battle;
class CMonster;

class CMonsterFactory
{
private:
	// ������Ÿ�� �������� ���� ��ü ���� ����, ������ Ŭ�н�Ű��
	vector<CMonster*>	m_vecMonsterList;

public:
	CMonsterFactory();
	~CMonsterFactory();

public:
	//virtual void SpawnMonster() = 0; // ��ȯ ������ ���� Ŭ�������� ����
};

#pragma once

class CScene_Battle;
class CMonster;
using namespace battle;

class CMonsterFactory
{
private:
	// ������Ÿ�� �������� ���� ��ü ���� ����, ������ Ŭ�н�Ű��
	vector<CMonster*>	m_vecMonsterList;

public:
	CMonsterFactory(CScene_Battle* _pScene);
	~CMonsterFactory();

public:
	//virtual void SpawnMonster() = 0; // ��ȯ ������ ���� Ŭ�������� ����
};

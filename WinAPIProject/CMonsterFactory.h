#pragma once

class CMonster;

class CMonsterFactory
{
public:
	CMonsterFactory();
	~CMonsterFactory();

public:
	// ������ ��ü ����(Clone�ؼ� ���� ����), ������ ��ϵ� ���� -> �� ���� �˱� ���� �����ʰ� ����
	// ���� ���͵��� ���� ���� �׷쿡 ����, ���ͽ����ʴ� ������ ��(������)
	// ��ȯ ������ �ڽ� Ŭ�����鿡�� ����
	virtual void SpawnMonster(map<wstring, CMonster*>& enemyDatas) = 0;
};

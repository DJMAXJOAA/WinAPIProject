#pragma once
#include "CData.h"

class MonsterData :
	public CData
{
public:
	wstring						m_strName;				// ���� �̸�
	int							m_iAnimatorKey;			// �ִϸ��̼� �̸�
	vector<MONSTER_STRATEGY>	m_vecMosterStrategy;	// ���� �� �ִ� ����
	float						m_fHP;					// ü��
	float						m_fAtt;					// ���ݷ�
	int							m_iMove;				// �� �Ͽ� �̵��� �� �ִ� �̵���
	int							m_iRange;				// ���� ��Ÿ�

public:
	MonsterData(int _key);
	~MonsterData();

public:
	virtual void ParseData(const json& item) override;
};


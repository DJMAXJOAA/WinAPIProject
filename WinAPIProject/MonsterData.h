#pragma once
#include "CData.h"

class MonsterData :
	public CData
{
private:
	wstring		m_strAnimationKey;		// �ִϸ��̼� �̸�
	float		m_fHP;
	float		m_fAtt;
	int			m_iMove;		// �� �Ͽ� �̵��� �� �ִ� �̵���
	int			m_iRange;		// ���� ����

public:
	MonsterData(int _key);
	~MonsterData();

public:
	virtual void LoadData() override;
};


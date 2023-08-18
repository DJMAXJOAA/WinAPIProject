#pragma once
#include "CData.h"

class MonsterData :
	public CData
{
private:
	wstring		m_strAnimationKey;		// 애니메이션 이름
	float		m_fHP;
	float		m_fAtt;
	int			m_iMove;		// 한 턴에 이동할 수 있는 이동량
	int			m_iRange;		// 공격 범위

public:
	MonsterData(int _key);
	~MonsterData();

public:
	virtual void LoadData() override;
};


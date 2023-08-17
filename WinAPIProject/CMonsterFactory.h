#pragma once

class CMonster;

enum class MON_TYPE
{
	NORMAL,
	RANGE,
};

class CMonsterFactory
{
private:


private:
	CMonsterFactory();
	~CMonsterFactory();

public:
	static CMonster* CreateMonster(MON_TYPE _eType, Vec2 _vPos);
};


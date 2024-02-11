#pragma once

class CScene_Battle;
class CMonster;

// 각 몬스터의 패턴 상태에 따라 로직이 달라짐
class IMonsterStrategy
{
public:
	virtual void Handle(CScene_Battle* _pScene, CMonster* _pMon) = 0;
};


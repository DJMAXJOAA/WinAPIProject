#pragma once

class CScene_Battle;
class CMonster;

// �� ������ ���� ���¿� ���� ������ �޶���
class IMonsterStrategy
{
public:
	virtual void Handle(CScene_Battle* _pScene, CMonster* _pMon) = 0;
};


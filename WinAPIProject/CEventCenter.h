#pragma once

class CScene_Battle;
class CMonster;

class CEventCenter
{
private:
	CScene_Battle*	m_pScene;

public:
    CEventCenter(CScene_Battle* _pScene);
    ~CEventCenter();

public:
    // 이벤트 호출 관련 처리 함수
    void OnTileSelect(CObject* _pObj);

    void PlayerAttackMonster(float _damage, CMonster* _pMon);
    void PlayerAttackDone();
    void PlayerSkillCasted(float _value);
    void PlayerSkillDone();

    void MonsterAttackPlayer(float _damage);
    void MonsterAttackDone(CMonster* _pMon);

    void MonsterDied(CMonster* _pObj);
    void PlayerDied();
};


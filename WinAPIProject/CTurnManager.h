#pragma once
#include "tile.h"

class CScene_Battle;
using namespace battle;

class CTurnManager
{
private:
    TURN_TYPE               m_CurrentTurn;      // ���� ��

public:
    void ChangeTurn(TURN_TYPE _type);       // ������ ���� �ٲ���ٰ� �̺�Ʈ ������

public:
    void SetCurrentTurn(TURN_TYPE _type) { m_CurrentTurn = _type; }

public:
    void PlayerStartInit();
    void PlayerTileSelectInit(CObject* _pObj);
    void PlayerMoveInit();

public:
    void PlayerStart();

    void PlayerMove();
    void PlayerAttack();

    void PlayerSkillInit();

    void EnemyStart();

public:
    CTurnManager(CScene_Battle* _pScene);
    ~CTurnManager();

public:
    void Update();
};


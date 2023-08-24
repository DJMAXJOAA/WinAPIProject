#pragma once
#include "tile.h"

class CScene_Battle;
class CMonster;

class CTurnManager
{
private:
    TURN_TYPE               m_CurrentTurn;      // ���� ��

    Vec2                    m_vPlayerPos;     // ĳ������ ���� ���� ��ġ
    Vec2                    m_vSelectTile;    // ���� �������� ���� Ÿ�� ��ǥ
    TILE_STATE              m_TileColor;      // ���� �������� Ÿ�� ����
    list<Vec2>              m_lstMoveRoute;   // Ÿ�� �̵� ���
    list<CMonster*>         m_lstTargetEnemy; // ���� ���� ����

public:
    CTurnManager();
    ~CTurnManager();

public:
    TURN_TYPE GetTurnState() { return m_CurrentTurn; }
    list<Vec2>& GetMoveRoute() { return m_lstMoveRoute; }
    Vec2 GetPlayerPos() { return m_vPlayerPos; }
    Vec2 GetSelectTile() { return m_vSelectTile; }

public:
    void SetTurnState(TURN_TYPE _type) { m_CurrentTurn = _type; }
    void SetTileColor(TILE_STATE _color) { m_TileColor = _color; }
    void SetPlayerPos(Vec2 _vPos) { m_vPlayerPos = _vPos; }
    void SetSelectTile(Vec2 _vPos) { m_vSelectTile = _vPos; }

public:
    void ChangeTurn(TURN_TYPE _type) { m_CurrentTurn = _type; ChangedTurn(_type); }

public:
    void RouteInit() { m_lstMoveRoute.clear(); }

public:
    void Update();
};


#pragma once
#include "tile.h"

class CMonster;
class CScene_Battle;

class CTurnCenter
{
private:
    TURN_TYPE       m_CurrentTurn;      // ���� ��

    Vec2            m_vSelectTile;      // ���� �������� ���� Ÿ�� ��ǥ
    TILE_STATE      m_TileColor;        // ���� �������� Ÿ�� ����
    list<Vec2>      m_lstMoveRoute;     // Ÿ�� �̵� ���
    list<CObject*>  m_lstTarget;        // ���� ���� ����
    int             m_iCombo;           // ���� �޺�

public:
    CTurnCenter();
    ~CTurnCenter();

public:
    TURN_TYPE GetTurnState() { return m_CurrentTurn; }
    TILE_STATE GetTileColor() { return m_TileColor; }
    list<Vec2>& GetMoveRoute() { return m_lstMoveRoute; }
    list<CObject*>& GetTargetList() { return m_lstTarget; }
    Vec2 GetSelectTile() { return m_vSelectTile; }
    int GetCombo() { return m_iCombo; }

    void SetTileColor(TILE_STATE _color) { m_TileColor = _color; }
    void SetSelectTile(Vec2 _vPos) { m_vSelectTile = _vPos; }
    void SetCombo(int _iValue) { m_iCombo = _iValue; }

public:
    void ChangeTurn(TURN_TYPE _type);

public:
    void RouteInit() { m_lstMoveRoute.clear(); }

public:
    void Update(CScene_Battle* _pScene);
    void Init();
};


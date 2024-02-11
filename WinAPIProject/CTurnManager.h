#pragma once
#include "tile.h"

class CMonster;

class CLogicCenter
{
private:
    Vec2            m_vSelectTile;      // ���� �������� Ÿ�� GRID ��ǥ
    TILE_STATE      m_CurrentTileColor; // ���� �������� Ÿ�� ����
    list<Vec2>      m_lstMoveRoute;     // �÷��̾��� Ÿ�� �̵� ���
    list<CObject*>  m_lstTarget;        // BFS Ž�� ��� ����
    int             m_iCombo;           // ���� �޺�

public:
    CLogicCenter();
    ~CLogicCenter();

public:
    TILE_STATE GetTileColor() { return m_CurrentTileColor; }
    list<Vec2>& GetMoveRoute() { return m_lstMoveRoute; }
    list<CObject*>& GetTargetList() { return m_lstTarget; }
    Vec2 GetSelectTile() { return m_vSelectTile; }
    int GetCombo() { return m_iCombo; }

    void SetTileColor(TILE_STATE _color) { m_CurrentTileColor = _color; }
    void SetSelectTile(Vec2 _vPos) { m_vSelectTile = _vPos; }
    void SetCombo(int _iValue) { m_iCombo = _iValue; }

public:
    void RouteInit() { m_lstMoveRoute.clear(); }

public:
    void Init();
};


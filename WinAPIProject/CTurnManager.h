#pragma once
#include "tile.h"

class CMonster;

class CLogicCenter
{
private:
    Vec2            m_vSelectTile;      // 현재 선택중인 타일 GRID 좌표
    TILE_STATE      m_CurrentTileColor; // 현재 선택중인 타일 색깔
    list<Vec2>      m_lstMoveRoute;     // 플레이어의 타일 이동 경로
    list<CObject*>  m_lstTarget;        // BFS 탐색 결과 저장
    int             m_iCombo;           // 현재 콤보

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


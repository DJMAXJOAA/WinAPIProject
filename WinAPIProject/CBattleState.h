#pragma once

class CScene_Battle;

class CBattleState
{
public:
	virtual void Handle(CScene_Battle* _pScene) = 0;

public:
	GRID_DIRECTION GetGridDirection(Vec2 _vGridFront, Vec2 _vGridRear, Vec2 _vDeparture, Vec2 _vDestination);
};


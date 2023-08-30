#pragma once

class CScene_Battle;

// 배틀 씬의 상태 로직을 클래스로 나눠놓음
class CBattleState
{
public:
	CBattleState() {};
	virtual ~CBattleState() {};

public:
	virtual void Handle(CScene_Battle* _pScene) = 0;

public:
	GRID_DIRECTION GetGridDirection(Vec2 _vGridFront, Vec2 _vGridRear, Vec2 _vDeparture, Vec2 _vDestination);
};


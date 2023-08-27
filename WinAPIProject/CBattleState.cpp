#include "pch.h"
#include "CBattleState.h"

GRID_DIRECTION CBattleState::GetGridDirection(Vec2 _vGridFront, Vec2 _vGridRear, Vec2 _vDeparture, Vec2 _vDestination)
{
	// 뒤도는 모션 설정
	if (_vGridFront.x >= _vGridRear.x && _vGridFront.y >= _vGridRear.y)
	{
		// 이미지 좌우 반전
		if (_vDeparture.x < _vDestination.x) return GRID_DIRECTION::UP;
		else return GRID_DIRECTION::LEFT;
	}
	// 뒤 안돔
	else
	{
		// 이미지 좌우 반전
		if (_vDeparture.x < _vDestination.x) return GRID_DIRECTION::RIGHT;
		else return GRID_DIRECTION::DOWN;
	}
}

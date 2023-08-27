#include "pch.h"
#include "CBattleState.h"

GRID_DIRECTION CBattleState::GetGridDirection(Vec2 _vGridFront, Vec2 _vGridRear, Vec2 _vDeparture, Vec2 _vDestination)
{
	// �ڵ��� ��� ����
	if (_vGridFront.x >= _vGridRear.x && _vGridFront.y >= _vGridRear.y)
	{
		// �̹��� �¿� ����
		if (_vDeparture.x < _vDestination.x) return GRID_DIRECTION::UP;
		else return GRID_DIRECTION::LEFT;
	}
	// �� �ȵ�
	else
	{
		// �̹��� �¿� ����
		if (_vDeparture.x < _vDestination.x) return GRID_DIRECTION::RIGHT;
		else return GRID_DIRECTION::DOWN;
	}
}

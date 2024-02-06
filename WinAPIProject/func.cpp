#include "pch.h"
#include "func.h"
#include "CObject.h"

#include "CEventMgr.h"

void CreateObj(CObject* _pObj, GROUP_TYPE _eGroup)
{
	tEvent evn = {  };
	evn.eEvent = EVENT_TYPE::CREATE_OBJECT;
	evn.lParam = (DWORD_PTR)_pObj;
	evn.wParam = (DWORD_PTR)_eGroup;

	CEventMgr::GetInstance()->AddEvent(evn);
}

void DeleteObj(CObject* _pObj)
{
	tEvent evn = {  };
	evn.eEvent = EVENT_TYPE::DELETE_OBJECT;
	evn.lParam = (DWORD_PTR)_pObj;

	CEventMgr::GetInstance()->AddEvent(evn);
}

void ChangeScene(SCENE_TYPE _eNext)
{
	tEvent evn = {  };
	evn.eEvent = EVENT_TYPE::SCENE_CHANGE;
	evn.lParam = (DWORD_PTR)_eNext;

	CEventMgr::GetInstance()->AddEvent(evn);
}

bool CompareGridPos(CObject* _pObj1, CObject* _pObj2)
{
	Vec2 pos1 = _pObj1->GetGridPos();
	Vec2 pos2 = _pObj2->GetGridPos();

	// x 좌표 우선 비교
	if (pos1.x < pos2.x) return true;
	if (pos1.x > pos2.x) return false;

	// x 같으면 y 비교
	return pos1.y < pos2.y;
}

GRID_DIRECTION GetGridDirection(Vec2 _vGridFront, Vec2 _vGridRear, Vec2 _vDeparture, Vec2 _vDestination)
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

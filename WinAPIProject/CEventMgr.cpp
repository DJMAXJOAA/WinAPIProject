#include "pch.h"
#include "CEventMgr.h"

#include "CSceneMgr.h"
#include "CUIMgr.h"

#include "CObject.h"
#include "CScene.h"

CEventMgr::CEventMgr()
{
}

CEventMgr::~CEventMgr()
{
}

void CEventMgr::Update()
{
	// 이번 프레임에 삭제 예정인 오브젝트들 싹다 지워주기
	SafeDeleteVec(m_vecDead);

	// 이벤트 처리
	for (size_t i = 0; i < m_vecEvent.size(); i++)
	{
		Excute(m_vecEvent[i]);
	}
	m_vecEvent.clear();
}

void CEventMgr::Excute(const tEvent& _eve)
{
	switch (_eve.eEvent)
	{
	case EVENT_TYPE::CREATE_OBJECT:
	{
		// lParam :: Object Adress
		// wParam :: Group Type
		CObject* pNewObj = (CObject*)_eve.lParam;
		GROUP_TYPE eType = (GROUP_TYPE)_eve.wParam;

		CSceneMgr::GetInstance()->GetCurScene()->AddObject(pNewObj, eType);
	}
		break;
	case EVENT_TYPE::DELETE_OBJECT:
	{
		// lParam :: Object Adress
		// Object를 Dead 상태로 변경 (메모리는 남아있지만, 비활성화)
		// 삭제 예정 오브젝트들을 모아두고, 그 다음 프레임에 일괄 삭제(오브젝트들에게 준비할 시간을 줌)
		CObject* pDeadObj = (CObject*)_eve.lParam;
		pDeadObj->SetDead();			// 다음 프레임때 dead 상태(비활성화)
		m_vecDead.push_back(pDeadObj);	// 다다음 프레임때 삭제
	}
		break;
	case EVENT_TYPE::SCENE_CHANGE:
	{
		// lParam :: Next Scene Type
		CSceneMgr::GetInstance()->ChangeScene((SCENE_TYPE)_eve.lParam);
		CUIMgr::GetInstance()->SetFocusedUI(nullptr);
	}
		break;
	case EVENT_TYPE::END:
		break;
	default:
		break;
	}
}

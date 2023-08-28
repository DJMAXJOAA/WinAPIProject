#include "pch.h"
#include "CEventMgr.h"

#include "CSceneMgr.h"
#include "CUIMgr.h"

#include "CObject.h"
#include "CScene.h"
#include "CScene_Battle.h"
#include "CPlayer.h"

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

	// 먼저 처리할 이벤트를 추가시키고, 클리어
	for (size_t i = 0; i < m_vecEarlyEvent.size(); i++)
	{
		m_vecEvent.insert(m_vecEvent.begin(), m_vecEarlyEvent[i]);
	}
	m_vecEarlyEvent.clear();

	// 후순위 이벤트를 이벤트에 추가시키고, 클리어
	for (size_t i = 0; i < m_vecLateEvent.size(); i++)
	{
		m_vecEvent.push_back(m_vecLateEvent[i]);
	}
	m_vecLateEvent.clear();

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
		break;
	}
	case EVENT_TYPE::DELETE_OBJECT:
	{
		// lParam :: Object Adress
		// Object를 Dead 상태로 변경 (메모리는 남아있지만, 비활성화)
		// 삭제 예정 오브젝트들을 모아두고, 그 다음 프레임에 일괄 삭제(오브젝트들에게 준비할 시간을 줌)
		CObject* pDeadObj = (CObject*)_eve.lParam;
		pDeadObj->SetDead();			// 다음 프레임때 dead 상태(비활성화)
		m_vecDead.push_back(pDeadObj);	// 다다음 프레임때 삭제
		break;
	}
	case EVENT_TYPE::SCENE_CHANGE:
	{
		// lParam :: Next Scene Type
		CSceneMgr::GetInstance()->ChangeScene((SCENE_TYPE)_eve.lParam);
		CUIMgr::GetInstance()->SetFocusedUI(nullptr);
		break;
	}
	case EVENT_TYPE::TILESELECT_TRIGGER:
	{
		// lParam :: Player Tile Select Trigger
		// 마우스 오브젝트가 불렀을텐데, 그게 불렸다는건 애초에 전투 씬에서 불림
		CObject* pObj = (CObject*)_eve.lParam;
		((CScene_Battle*)CSceneMgr::GetInstance()->GetCurScene())->TileSelectTrigger(pObj);
		break;	
	}
	case EVENT_TYPE::TURN_CHANGE:
	{
		// lParam :: Turn Type
		// 턴 종료되었을 때, 씬 배틀에서 알게 하기 위해 함수 호출(타입에 맞는 초기화 함수 호출)
		TURN_TYPE turnType = (TURN_TYPE)_eve.lParam;
		((CScene_Battle*)CSceneMgr::GetInstance()->GetCurScene())->TurnInit(turnType);
		break;
	}
	case EVENT_TYPE::ANIMATION_EVENT:
	{
		// lParam :: Animation Event Trigger
		// Animation Event Frame이 되면, 이벤트 발생 (event frame이 0이면 이벤트가 발생하지 않음)
		CObject* pObj = (CObject*)_eve.lParam;
		pObj->AnimationEvent();
		break;
	}
	case EVENT_TYPE::ANIMATION_END:
	{
		// lParam :: Animation Finish Trigger
		// Animation Repeat가 false인 상황에서, 애니메이션이 종료되면 주인 오브젝트에 이벤트 발생시킴
		CObject* pObj = (CObject*)_eve.lParam;
		pObj->AnimationEnd();
		break;
	}
	case EVENT_TYPE::PLAYER_ATTACK:
	{
		// lParam :: Player Attack Damage
		// wParam :: Attack Target
		// 애니메이션의 특정 프레임(공격 프레임)에서 호출되서, 게임센터에 캐릭터의 데미지를 전송
		float fDamage = (float)_eve.lParam;
		CMonster* pObj = (CMonster*)_eve.wParam;
		((CScene_Battle*)CSceneMgr::GetInstance()->GetCurScene())->PlayerAttackMonster(fDamage, pObj);
		break;
	}
	case EVENT_TYPE::PLAYER_ATTACK_DONE:
	{
		// 애니메이션이 끝나면, 리스트를 지워줌
		((CScene_Battle*)CSceneMgr::GetInstance()->GetCurScene())->PlayerAttackDone();
		break;
	}
	case EVENT_TYPE::PLAYER_SKILL_CAST:
	{
		// 애니메이션이 끝나면, 리스트를 지워줌
		float fValue = (float)_eve.lParam;
		((CScene_Battle*)CSceneMgr::GetInstance()->GetCurScene())->PlayerSkillCasted(fValue);
		break;
	}
	case EVENT_TYPE::PLAYER_SKILL_DONE:
	{
		// 애니메이션이 끝나면, 리스트를 지워줌
		((CScene_Battle*)CSceneMgr::GetInstance()->GetCurScene())->PlayerSkillDone();
		break;
	}
	case EVENT_TYPE::MONSTER_DIED:
	{
		// lParam :: Monster Object
		// 애니메이션이 끝나면, 리스트를 지워줌
		CMonster* pObj = (CMonster*)_eve.lParam;
		((CScene_Battle*)CSceneMgr::GetInstance()->GetCurScene())->MonsterDied(pObj);
		break;
	}
	case EVENT_TYPE::END:
		break;
	default:
		break;
	}
}

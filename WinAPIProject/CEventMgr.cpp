#include "pch.h"
#include "CEventMgr.h"

#include "CSceneMgr.h"
#include "CUIMgr.h"

#include "CObject.h"
#include "CPlayer.h"
#include "CBattleStateMachine.h"
#include "CEventCenter.h"

#include "CScene.h"
#include "CScene_Battle.h"
#include "CScene_Robby.h"

CEventMgr::CEventMgr()
{
}

CEventMgr::~CEventMgr()
{
}

void CEventMgr::Update()
{
	// �̹� �����ӿ� ���� ������ ������Ʈ�� �ϴ� �����ֱ�
	SafeDeleteVec(m_vecDead);

	// ���� ó���� �̺�Ʈ�� �߰���Ű��, Ŭ����
	for (size_t i = 0; i < m_vecEarlyEvent.size(); i++)
	{
		m_vecEvent.insert(m_vecEvent.begin(), m_vecEarlyEvent[i]);
	}
	m_vecEarlyEvent.clear();

	// �ļ��� �̺�Ʈ�� �̺�Ʈ�� �߰���Ű��, Ŭ����
	for (size_t i = 0; i < m_vecLateEvent.size(); i++)
	{
		m_vecEvent.push_back(m_vecLateEvent[i]);
	}
	m_vecLateEvent.clear();

	// �̺�Ʈ ó��
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
		// Object�� Dead ���·� ���� (�޸𸮴� ����������, ��Ȱ��ȭ)
		// ���� ���� ������Ʈ���� ��Ƶΰ�, �� ���� �����ӿ� �ϰ� ����(������Ʈ�鿡�� �غ��� �ð��� ��)
		CObject* pDeadObj = (CObject*)_eve.lParam;
		pDeadObj->SetDead();			// ���� �����Ӷ� dead ����(��Ȱ��ȭ)
		m_vecDead.push_back(pDeadObj);	// �ٴ��� �����Ӷ� ����
		break;
	}
	case EVENT_TYPE::SCENE_CHANGE:
	{
		// lParam :: Next Scene Type
		CSceneMgr::GetInstance()->ChangeScene((SCENE_TYPE)_eve.lParam);
		CUIMgr::GetInstance()->SetFocusedUI(nullptr);
		break;
	}
	case EVENT_TYPE::CAMERA_EVENT:
	{
		// �̺�Ʈ ������, �˾Ƽ� ī�޶� �̺�Ʈ �߻�
		CSceneMgr::GetInstance()->GetCurScene()->CameraEvent();
		break;
	}
	case EVENT_TYPE::TILESELECT_TRIGGER:
	{
		// lParam :: Player Tile Select Trigger
		// ���콺 ������Ʈ�� �ҷ����ٵ�, �װ� �ҷȴٴ°� ���ʿ� ���� ������ �Ҹ�
		CObject* pObj = (CObject*)_eve.lParam;
		((CScene_Battle*)CSceneMgr::GetInstance()->GetCurScene())->GetEventCenter()->OnTileSelect(pObj);
		break;	
	}
	case EVENT_TYPE::TURN_CHANGE:
	{
		// lParam :: Turn Type
		// �� ����Ǿ��� ��, �� ��Ʋ���� �˰� �ϱ� ���� �Լ� ȣ��(Ÿ�Կ� �´� �ʱ�ȭ �Լ� ȣ��)
		TURN_TYPE turnType = (TURN_TYPE)_eve.lParam;
		((CScene_Battle*)CSceneMgr::GetInstance()->GetCurScene())->GetStateMachine()->ChangeState(turnType);
		break;
	}
	case EVENT_TYPE::FIELD_ENTER_STAGE:
	{
		// lParam :: float x
		// wParam :: float y
		// Ư�� ���� ��ǥ�� ��ư�� ���ȴٰ� �̺�Ʈ ȣ��
		Vec2 vGridPos = Vec2((float)_eve.lParam, (float)_eve.wParam);
		((CScene_Robby*)CSceneMgr::GetInstance()->GetCurScene())->OnEnterStage(vGridPos);
		break;
	}
	case EVENT_TYPE::ANIMATION_EVENT:
	{
		// lParam :: Animation Event Trigger
		// Animation Event Frame�� �Ǹ�, �̺�Ʈ �߻� (event frame�� 0�̸� �̺�Ʈ�� �߻����� ����)
		CObject* pObj = (CObject*)_eve.lParam;
		pObj->AnimationEvent();
		break;
	}
	case EVENT_TYPE::ANIMATION_END:
	{
		// lParam :: Animation Finish Trigger
		// Animation Repeat�� false�� ��Ȳ����, �ִϸ��̼��� ����Ǹ� ���� ������Ʈ�� �̺�Ʈ �߻���Ŵ
		CObject* pObj = (CObject*)_eve.lParam;
		pObj->AnimationEnd();
		break;
	}
	case EVENT_TYPE::PLAYER_ATTACK:
	{
		// lParam :: Player Attack Damage
		// wParam :: Attack Target
		// �ִϸ��̼��� Ư�� ������(���� ������)���� ȣ��Ǽ�, ���Ӽ��Ϳ� ĳ������ �������� ����
		float fDamage = (float)_eve.lParam;
		CMonster* pObj = (CMonster*)_eve.wParam;
		((CScene_Battle*)CSceneMgr::GetInstance()->GetCurScene())->GetEventCenter()->PlayerAttackMonster(fDamage, pObj);
		break;
	}
	case EVENT_TYPE::PLAYER_ATTACK_DONE:
	{
		// �ִϸ��̼��� ������, ����Ʈ�� ������
		((CScene_Battle*)CSceneMgr::GetInstance()->GetCurScene())->GetEventCenter()->PlayerAttackDone();
		break;
	}
	case EVENT_TYPE::PLAYER_SKILL_CAST:
	{
		// �ִϸ��̼��� ������, ����Ʈ�� ������
		float fValue = (float)_eve.lParam;
		((CScene_Battle*)CSceneMgr::GetInstance()->GetCurScene())->GetEventCenter()->PlayerSkillCasted(fValue);
		break;
	}
	case EVENT_TYPE::PLAYER_SKILL_DONE:
	{
		// �ִϸ��̼��� ������, ����Ʈ�� ������
		((CScene_Battle*)CSceneMgr::GetInstance()->GetCurScene())->GetEventCenter()->PlayerSkillDone();
		break;
	}
	case EVENT_TYPE::MONSTER_ATTACK:
	{
		// lParam :: Monster Attack Damage
		// �ִϸ��̼��� Ư�� ������(���� ������)���� ȣ��Ǽ�, ���Ӽ��Ϳ� ĳ������ �������� ����
		float fValue = (float)_eve.lParam;
		((CScene_Battle*)CSceneMgr::GetInstance()->GetCurScene())->GetEventCenter()->MonsterAttackPlayer(fValue);
		break;
	}
	case EVENT_TYPE::MONSTER_ATTACK_DONE:
	{
		// lParam :: Monster Object
		// �ִϸ��̼��� ������, ����Ʈ�� ������
		CMonster* pObj = (CMonster*)_eve.lParam;

		((CScene_Battle*)CSceneMgr::GetInstance()->GetCurScene())->GetEventCenter()->MonsterAttackDone(pObj);
		break;
	}
	case EVENT_TYPE::MONSTER_DIED:
	{
		// lParam :: Monster Object
		// �ִϸ��̼��� ������, ����Ʈ�� ������
		CMonster* pObj = (CMonster*)_eve.lParam;
		((CScene_Battle*)CSceneMgr::GetInstance()->GetCurScene())->GetEventCenter()->MonsterDied(pObj);
		break;
	}
	case EVENT_TYPE::PLAYER_DIED:
	{
		// �÷��̾ �׾��ٰ� ������ �˸��� ��
		((CScene_Battle*)CSceneMgr::GetInstance()->GetCurScene())->GetEventCenter()->PlayerDied();
		break;
	}
	case EVENT_TYPE::END:
		break;
	default:
		break;
	}
}

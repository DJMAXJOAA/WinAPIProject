#include "pch.h"
#include "CScene_Battle.h"

#include "CCollisionMgr.h"
#include "CKeyMgr.h"
#include "CDataMgr.h"
#include "CResMgr.h"

#include "CPlayer.h"
#include "CTile.h"
#include "CMouse.h"
#include "CMonster.h"
#include "CEffect.h"
#include "CPanelUI_Number.h"

#include "CSound.h"
#include "CAnimator.h"
#include "CAnimation.h"

#include "CBattleState.h"
#include "CEventCenter.h"
#include "CTurnManager.h"
#include "CTileManager.h"
#include "CMonsterSpawner.h"
#include "CBattleStateMachine.h"
#include "CEventMgr.h"

static std::random_device rd;
static std::mt19937 gen(rd());

static bool cheat;

CScene_Battle::CScene_Battle()
	: m_pPlayer(nullptr)
	, m_iFieldType(0)
	, m_iDifficulty(1)
	, m_FieldType(FIELD_TYPE::COMMON)
	, m_pMonsterSpawner(nullptr)
	, m_pEventCenter(nullptr)
	, m_pTurnCenter(nullptr)
	, m_pTileCenter(nullptr)
	, m_pStateMachine(nullptr)
	, m_pMoneyUI(nullptr)
	, m_pComboUI(nullptr)
	, m_vecSoundEffect((int)SOUND_TYPE::END)
{
	m_vecSoundEffect[(int)SOUND_TYPE::WIN] = CResMgr::GetInstance()->LoadSound(L"winSound", L"sound\\win.wav");
	m_vecSoundEffect[(int)SOUND_TYPE::LOSE] = CResMgr::GetInstance()->LoadSound(L"losdSound", L"sound\\lose.wav");
}

CScene_Battle::~CScene_Battle()
{
}

void CScene_Battle::PlayerAttackMonster(float _damage, CMonster* _pMon)
{
	// 피격 이펙트 생성
	CEffect* pEffect = new CEffect(209, _pMon->GetPos());
	CreateObj(pEffect, GROUP_TYPE::EFFECT);

	_pMon->GetDamaged(_damage);

	printf("CScene_Battle::PlayerAttackMonster :: 적에게 %1.f 데미지로 공격 ->", _damage);
	cout << _pMon << "\n";
}

void CScene_Battle::PlayerAttackDone()
{
	list<CObject*>& list = m_pTurnCenter->GetTargetList();
	m_pPlayer->SetAttacking(false);

	if (list.empty())
	{
		printf("CScene_Battle::PlayerAttackDone :: 리스트가 비었습니다.\n");
		return;
	}
	else
	{
		printf("CScene_Battle::PlayerAttackDone :: 타겟 리스트를 하나 삭제합니다 ->");
		cout << list.front() << "를 삭제했습니다. \n";
		list.pop_front();
		return;
	}
}

void CScene_Battle::PlayerSkillCasted(float _value)
{
	list<CObject*>& list = m_pTurnCenter->GetTargetList();
	if (list.empty())
	{
		printf("CScene_Battle::PlayerSkillCasted :: 리스트가 비었습니다.\n");
		return;
	}
	else
	{
		printf("CScene_Battle::PlayerSkillCasted :: 리스트 대상에게 스킬을 시전합니다.\n");
		printf("CScene_Battle::PlayerSkillCasted :: 적들에게 %1.f 데미지로 스킬 시전 ->", _value);
		for (auto iter = list.begin(); iter != list.end(); iter++)
		{
			// 피격 이펙트 생성
			CEffect* pEffect = new CEffect(209, ((CMonster*)*iter)->GetPos());
			CreateObj(pEffect, GROUP_TYPE::EFFECT);

			((CMonster*)*iter)->GetDamaged(_value);
		}

		list.clear();
		return;
	}
}

void CScene_Battle::PlayerSkillDone()
{
	m_pPlayer->SetAttacking(false);
	ChangeTurn(TURN_TYPE::ENEMY_MOVE);

	printf("CScene_Battle::PlayerSkillDone :: 스킬이 종료되었습니다.\n");
}

void CScene_Battle::MonsterAttackPlayer(float _damage)
{
	// 피격 이펙트 생성
	CEffect* pEffect = new CEffect(209, m_pPlayer->GetPos());
	CreateObj(pEffect, GROUP_TYPE::EFFECT);

	m_pPlayer->GetDamaged(_damage);

	printf("CScene_Battle::MonsterAttackPlayer :: 몬스터가 플레이어에게 %1.f 데미지로 공격 ->", _damage);
	cout << m_pPlayer << "\n";
}

void CScene_Battle::MonsterAttackDone(CMonster* _pMon)
{
	_pMon->AnimationDirection(L"idle", true);
	_pMon->SetActing(true);

	printf("CScene_Battle::MonsterAttackDone :: 공격이 종료되었습니다.\n");
}

void CScene_Battle::MonsterDied(CMonster* _pObj)
{
	int money = _pObj->GetMoney();
	m_pPlayer->SetMoney(m_pPlayer->GetMoney() + money);
	printf("CScene_Battle::MonsterDied :: %d원을 얻어서 돈이 %d원이 됐습니다.\n", money, m_pPlayer->GetMoney());

	Vec2 GridPos = _pObj->GetGridPos();
	vector<vector<TileState>>& vecTiles = m_pTileCenter->GetTiles();

	vecTiles[(int)GridPos.y][(int)GridPos.x].pObj = nullptr;

	printf("CScene_Battle::MonsterDied :: %d, %d 타일 위의 오브젝트를 초기화 시켰습니다.\n", (int)GridPos.x, (int)GridPos.y);
}

void CScene_Battle::PlayerDied()
{
	ChangeTurn(TURN_TYPE::DEFEAT);

	printf("CScene_Battle::PlayerDied :: 플레이어 사망\n");
}

void CScene_Battle::ChangeTurn(TURN_TYPE _type)
{
	tEvent evn = {  };
	evn.eEvent = EVENT_TYPE::TURN_CHANGE;
	evn.lParam = (DWORD_PTR)_type;

	CEventMgr::GetInstance()->AddEventLate(evn);
}

void CScene_Battle::OnTileSelect(CObject* _pObj)
{
	// 조건 :: 마우스를 꾹 누른 상태에서 타일의 콜라이더와 닿은 상태
	// 마우스 꾹 눌린 상태에서, 콜라이더가 닿으면 -> 이벤트 매니저에서 이 함수를 호출시킴
	m_pStateMachine->GetState()->OnTileSelect(this, _pObj);

	// BFS 방문 초기화
	m_pTileCenter->TileVisitedInit();
}

void CScene_Battle::CameraEvent()
{
	ChangeScene(SCENE_TYPE::ROBBY);
}

void CScene_Battle::Update()
{
	InputKey();								// 키 입력
	SortGroupObj();							// 유닛 오브젝트의 렌더링 순서 조절(좌표가 낮을수록 더 낮은 순서로)
	CScene::Update();						// 메인 씬 업데이트 (각자 오브젝트들의 업데이트)

	// 로직 처리 (상태 머신)
	m_pStateMachine->Handle(this);
	
	// 몬스터 관리 배열 업데이트 (사망 예정인 오브젝트들을 삭제)
	m_pMonsterSpawner->Update();

	// UI 업데이트 (임시)
	m_pMoneyUI->SetNumber((int)m_pPlayer->GetMoney());
	m_pComboUI->SetNumber((int)m_pTurnCenter->GetCombo());
}

void CScene_Battle::Enter()
{
	// 매니저를 등록 -> 오브젝트로 등록하는게 아니라서 직접 해제 시켜주어야 한다. Enter, Exit 함수와 관련없음
	// 메인 씬에 있는 그룹 오브젝트들을 참조해서 관리하는 형식이라, 임의로 값들을 해제시키지 않게 주의해야 한다
	m_pTurnCenter = new CLogicCenter;
	m_pEventCenter = new CEventCenter(this);
	m_pMonsterSpawner = new CMonsterSpawner;
	m_pTileCenter = new CTileCenter;
	m_pStateMachine = new CBattleStateMachine(this);
}

void CScene_Battle::Exit()
{
	CCamera::GetInstance()->SetTarget(nullptr);

	m_pTurnCenter->Init();
	m_pTileCenter->Init();
	m_pMonsterSpawner->Init();
	DeleteAll();
	CCollisionMgr::GetInstance()->Reset();

	delete m_pMonsterSpawner;
	delete m_pEventCenter;
	delete m_pTurnCenter;
	delete m_pTileCenter;
	delete m_pStateMachine;
}

void CScene_Battle::InputKey()
{
	// 화면 전환
	if (KEY_TAP(KEY::ENTER))
	{
		ChangeScene(SCENE_TYPE::END);
	}

	// 치트키
	if (KEY_TAP(KEY::P))
	{
		if (cheat == false)
		{
			m_pPlayer->SetHP(m_pPlayer->GetMaxHP());
			m_pPlayer->SetAtt(150.f);
			cheat = true;
		}
		else
		{
			m_pPlayer->SetAtt(40.f);
			cheat = false;
		}
	}

	// 승리
	if (KEY_TAP(KEY::O))
	{
		ChangeTurn(TURN_TYPE::WIN);
	}

	// 시점 조절
	if (KEY_TAP(KEY::RBTN))
	{
		Vec2 vLookAt = CCamera::GetInstance()->GetRealPos(MOUSE_POS);
		CCamera::GetInstance()->SetLookAt(vLookAt);
	}
}

void CScene_Battle::SortGroupObj()
{
	vector<CObject*>& groupObject = GetGroupObject(GROUP_TYPE::UNIT);
	sort(groupObject.begin(), groupObject.end(), CompareGridPos);
}

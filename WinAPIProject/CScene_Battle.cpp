#include "pch.h"
#include "CScene_Battle.h"

#include "CPlayer.h"
#include "CTile.h"
#include "CPanelUI_Number.h"
#include "CSound.h"

#include "CEventCenter.h"
#include "CTurnManager.h"
#include "CTileManager.h"
#include "CMonsterSpawner.h"
#include "CBattleStateMachine.h"

#include "CCollisionMgr.h"
#include "CKeyMgr.h"
#include "CResMgr.h"
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
	, m_pLogicCenter(nullptr)
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

void CScene_Battle::ChangeTurn(TURN_TYPE _type)
{
	tEvent evn = {  };
	evn.eEvent = EVENT_TYPE::TURN_CHANGE;
	evn.lParam = (DWORD_PTR)_type;

	CEventMgr::GetInstance()->AddEventLate(evn);
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
	m_pComboUI->SetNumber((int)m_pLogicCenter->GetCombo());
}

void CScene_Battle::Enter()
{
	// 매니저를 등록 -> 오브젝트로 등록하는게 아니라서 직접 해제 시켜주어야 한다. Enter, Exit 함수와 관련없음
	// 메인 씬에 있는 그룹 오브젝트들을 참조해서 관리하는 형식이라, 임의로 값들을 해제시키지 않게 주의해야 한다
	m_pLogicCenter = new CLogicCenter;
	m_pEventCenter = new CEventCenter(this);
	m_pMonsterSpawner = new CMonsterSpawner;
	m_pTileCenter = new CTileCenter;
	m_pStateMachine = new CBattleStateMachine(this);
}

void CScene_Battle::Exit()
{
	CCamera::GetInstance()->SetTarget(nullptr);

	m_pLogicCenter->Init();
	m_pTileCenter->Init();
	m_pMonsterSpawner->Init();
	DeleteAll();
	CCollisionMgr::GetInstance()->Reset();

	delete m_pMonsterSpawner;
	delete m_pEventCenter;
	delete m_pLogicCenter;
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

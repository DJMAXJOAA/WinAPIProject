#include "pch.h"
#include "CScene_Start.h"

#include "CCore.h"
#include "CPathMgr.h"
#include "CCollisionMgr.h"
#include "CKeyMgr.h"
#include "CSceneMgr.h"
#include "CCamera.h"

#include "CObject.h"
#include "CPlayer.h"
#include "CMonster.h"
#include "CTexture.h"

void CScene_Start::Update()
{
	CScene::Update();	// 부모쪽 함수를 이용할 수도 있음, 오버라이딩 한 이유는 +@ 하기 위해서

	if (KEY_TAP(KEY::ENTER))
	{
		ChangeScene(SCENE_TYPE::TOOL);
	}

	// 마우스 위치 계산할때는 렌더링 좌표 기준으로 계산하니까, 오브젝트와 반대로 계산
	if (!CCamera::GetInstance()->IsVibrating())
	{
		if (KEY_TAP(KEY::LBTN))
		{
			Vec2 vLookAt = CCamera::GetInstance()->GetRealPos(MOUSE_POS);
			CCamera::GetInstance()->SetLookAt(vLookAt);
		}
		if (KEY_TAP(KEY::RBTN))
		{
			CCamera::GetInstance()->SetVibrateCamera(50.f, 10, 0.03f);
		}
	}
}

void CScene_Start::Enter()
{
	// Player 추가
	//CObject* pObj = new CPlayer;
	//pObj->SetPos(Vec2(320.f, 384.f));
	//pObj->SetScale(Vec2(100.f, 100.f));
	//AddObject(pObj, GROUP_TYPE::PLAYER);
	
	/*CCamera::GetInstance()->SetTarget(pObj);*/

	// Player의 복사 생성
	//{
	//	CObject* pOtherPlayer = pObj->Clone();
	//	pOtherPlayer->SetPos(Vec2(420.f, 384.f));
	//	AddObject(pOtherPlayer, GROUP_TYPE::PLAYER);
	//}
	//{
	//	CObject* pOtherPlayer = pObj->Clone();
	//	pOtherPlayer->SetPos(Vec2(370.f, 354.f));
	//	AddObject(pOtherPlayer, GROUP_TYPE::PLAYER);
	//}

	// Monster 추가
	Vec2 vResolution = CCore::GetInstance()->GetResolution();
	CMonster* pMonsterObj = nullptr;

	//pMonsterObj = new CMonster;
	//pMonsterObj->SetName(L"Monster");
	//pMonsterObj->SetPos(Vec2(500.f, 100.f));
	//pMonsterObj->SetScale(Vec2(50.f, 50.f));
	//AddObject(pMonsterObj, GROUP_TYPE::MONSTER);

	// 충돌 처리 -> player 그룹과 monster 그룹 간의 충돌 체크
	//CCollisionMgr::GetInstance()->CheckGroup(GROUP_TYPE::PLAYER, GROUP_TYPE::UNIT);
	//CCollisionMgr::GetInstance()->CheckGroup(GROUP_TYPE::MISSILE_PLAYER, GROUP_TYPE::UNIT);


	// 카메라 시점 지정
	CCamera::GetInstance()->SetLookAt(vResolution / 2.f);

	// 카메라 효과
	CCamera::GetInstance()->FadeOut(0.5f);
	CCamera::GetInstance()->FadeIn(0.5f);
	CCamera::GetInstance()->FadeOut(0.5f);
	CCamera::GetInstance()->FadeIn(0.5f);
}

void CScene_Start::Exit()
{
	DeleteAll();
	CCollisionMgr::GetInstance()->Reset();
}

CScene_Start::CScene_Start()
{
}

CScene_Start::~CScene_Start()
{
}

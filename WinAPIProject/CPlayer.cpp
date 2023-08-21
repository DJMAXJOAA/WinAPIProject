#include "pch.h"
#include "CPlayer.h"

#include "CTimeMgr.h"
#include "CKeyMgr.h"
#include "CSceneMgr.h"
#include "CPathMgr.h"
#include "CResMgr.h"

#include "CTexture.h"
#include "CScene.h"
#include "CMissile.h"

#include "CCollider.h"
#include "CAnimator.h"
#include "CAnimation.h"

CPlayer::CPlayer()
{
	CreateCollider();
	GetCollider()->SetOffsetPos(Vec2(0.f, 0.f));
	GetCollider()->SetScale(Vec2(10.f, 10.f));

	// 텍스쳐 로딩 (애니메이션 설정)
	SetAnimator(200);
	GetAnimator()->Play(113, true);
}

CPlayer::~CPlayer()
{

}

void CPlayer::CreateMissile()
{
	Vec2 vMissilePos = GetPos();
	vMissilePos.y -= GetScale().y / 2.f;

	// Missile Object
	CMissile* pMissile = new CMissile;
	pMissile->SetName(L"Missile_Player");
	pMissile->SetPos(vMissilePos);
	pMissile->SetScale(Vec2(25.f, 25.f));
	pMissile->SetDir(Vec2(-0.f, -1.f)); // 공격 방향

	// 바로 만들어지지 않고, 이벤트 매니저에게 이벤트를 추가시킴
	// 다음 프레임에 만들어질거임
	CreateObj(pMissile, GROUP_TYPE::MISSILE_PLAYER);
}

void CPlayer::Render(HDC hdc)
{
	ComponetRender(hdc);
}

static int test = 100;
void CPlayer::Update()
{
	// 지역변수를 getpos로 복사해와서, 나중에 setpos로 적용 시켜주어야 한다
	Vec2 vPos = GetPos();

	if (KEY_HOLD(KEY::A))
	{
		vPos.x -= 400.f * fDT;
	}
	if (KEY_HOLD(KEY::W))
	{
		vPos.y -= 400.f * fDT;
	}
	if (KEY_HOLD(KEY::D))
	{
		vPos.x += 400.f * fDT;
	}
	if (KEY_HOLD(KEY::S))
	{
		vPos.y += 400.f * fDT;
	}

	if (KEY_TAP(KEY::C))
	{
		GetAnimator()->Play(test--, true);
	}
	if (KEY_TAP(KEY::V))
	{
		GetAnimator()->Play(test++, true);
	}

	if (CKeyMgr::GetInstance()->GetKeyState(KEY::SPACE) == KEY_STATE::TAP)
	{
		CreateMissile();
	}

	SetPos(vPos);

	GetAnimator()->Update();
}
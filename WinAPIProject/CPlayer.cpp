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
	// 콜라이더 얻기 + 콜라이더 설정
	CreateCollider();
	GetCollider()->SetOffsetPos(Vec2(0.f, 0.f));
	GetCollider()->SetScale(Vec2(40.f, 100.f));

	// 텍스쳐 로딩 (애니메이션 설정)
	SetAnimator(200);
	GetAnimator()->Play(L"Player_Idle", true);
	/*CTexture* pTex = CResMgr::GetInstance()->LoadTexture(L"PlayerTex", L"texture\\asdf2.bmp");*/
	//CreateAnimator();
	//GetAnimator()->CreateAnimation(L"Player", pTex, Vec2(0.f, 0.f), Vec2(170.f, 179.f), Vec2(171.f, 0.f), 0.15f, 5); // 854x179, 170x179 5동작 + 가로1칸간격
	//GetAnimator()->Play(L"Player", true);
	//
	//CAnimation* pAnim = GetAnimator()->FindAnimation(L"Player");
	//for (int i = 0; i < pAnim->GetMaxFrame(); i++)
	//{
	//	pAnim->GetFrame(i).vOffset = Vec2(-10.f, -20.f);
	//}
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
	// 컴포넌트 렌더링 함수
	// 콜라이더 + 애니메이션
	
	ComponetRender(hdc);
	//CTexture* pTex = CResMgr::GetInstance()->LoadTexture(L"Plane", L"texture\\effect.bmp");

	//Vec2 vPos = GetPos();
	//vPos = CCamera::GetInstance()->GetRenderPos(vPos);

	//float width = (float)pTex->Width();
	//float height = (float)pTex->Height();

	//BLENDFUNCTION bf = {};
	//bf.BlendOp = AC_SRC_OVER;
	//bf.BlendFlags = 0;
	//bf.SourceConstantAlpha = 255;
	//bf.AlphaFormat = AC_SRC_ALPHA;

	//AlphaBlend(hdc
	//	, (int)(vPos.x - width / 2.f)
	//	, (int)(vPos.y - height / 2.f)
	//	, (int)width, (int)height
	//	, pTex->GetDC()
	//	, 0, 0, (int)width, (int)height
	//	, bf);
}

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

	if (CKeyMgr::GetInstance()->GetKeyState(KEY::SPACE) == KEY_STATE::TAP)
	{
		CreateMissile();
	}

	SetPos(vPos);

	GetAnimator()->Update();
}
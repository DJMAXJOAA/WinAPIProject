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
	// �ݶ��̴� ��� + �ݶ��̴� ����
	CreateCollider();
	GetCollider()->SetOffsetPos(Vec2(0.f, 0.f));
	GetCollider()->SetScale(Vec2(40.f, 100.f));

	// �ؽ��� �ε� (�ִϸ��̼� ����)
	CTexture* pTex = CResMgr::GetInstance()->LoadTexture(L"PlayerTex", L"texture\\asdf2.bmp");
	CreateAnimator();
	GetAnimator()->CreateAnimation(L"Player", pTex, Vec2(0.f, 0.f), Vec2(170.f, 179.f), Vec2(171.f, 0.f), 0.15f, 5); // 854x179, 170x179 5���� + ����1ĭ����
	GetAnimator()->Play(L"Player", true);
	
	CAnimation* pAnim = GetAnimator()->FindAnimation(L"Player");
	for (int i = 0; i < pAnim->GetMaxFrame(); i++)
	{
		pAnim->GetFrame(i).vOffset = Vec2(-10.f, -20.f);
	}
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
	pMissile->SetDir(Vec2(-0.f, -1.f)); // ���� ����

	// �ٷ� ��������� �ʰ�, �̺�Ʈ �Ŵ������� �̺�Ʈ�� �߰���Ŵ
	// ���� �����ӿ� �����������
	CreateObj(pMissile, GROUP_TYPE::MISSILE_PLAYER);
}

void CPlayer::Render(HDC hdc)
{
	// ������Ʈ ������ �Լ�
	// �ݶ��̴� + �ִϸ��̼�
	
	/*ComponetRender(hdc);*/
	CTexture* pTex = CResMgr::GetInstance()->LoadTexture(L"Plane", L"texture\\effect.bmp");

	Vec2 vPos = GetPos();
	vPos = CCamera::GetInstance()->GetRenderPos(vPos);

	float width = (float)pTex->Width();
	float height = (float)pTex->Height();

	BLENDFUNCTION bf = {};
	bf.BlendOp = AC_SRC_OVER;
	bf.BlendFlags = 0;
	bf.SourceConstantAlpha = 255;
	bf.AlphaFormat = AC_SRC_ALPHA;

	AlphaBlend(hdc
		, (int)(vPos.x - width / 2.f)
		, (int)(vPos.y - height / 2.f)
		, (int)width, (int)height
		, pTex->GetDC()
		, 0, 0, (int)width, (int)height
		, bf);
}

void CPlayer::Update()
{
	// ���������� getpos�� �����ؿͼ�, ���߿� setpos�� ���� �����־�� �Ѵ�
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
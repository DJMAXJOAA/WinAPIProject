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
	: m_fSpeed(200.f)
	, m_playerState(PLAYER_STATE::IDEL)
{
	CreateCollider();
	GetCollider()->SetScale(Vec2(15.f, 15.f));

	// �ؽ��� �ε� (�ִϸ��̼� ����)
	SetAnimator(200);
	GetAnimator()->Play((int)PLAYER_STATE::IDEL, true);
}

CPlayer::~CPlayer()
{

}

void CPlayer::Move(Vec2 _vDestination)
{
	Vec2 vPos = GetPos();
	if(vPos.DistanceTo(_vDestination) <= m_fSpeed * fDT)
		return SetPos(_vDestination);

	Vec2 vDirection = (vPos - _vDestination).Normalize() * -1;

	vPos.x += m_fSpeed * vDirection.x * fDT;
	vPos.y += m_fSpeed * vDirection.y * fDT;

	SetPos(vPos);
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
	ComponetRender(hdc);
}

static int test = 100;
void CPlayer::Update()
{
	GetAnimator()->Play((int)m_playerState, true);
	GetAnimator()->GetAnimation()->SetFlip(false);
	GetAnimator()->Update();
}
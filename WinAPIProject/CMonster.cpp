#include "pch.h"
#include "CMonster.h"

#include "CTimeMgr.h"

#include "CCollider.h"

#include "AI.h"

CMonster::CMonster()
	: m_iHP(5)
{
	// 콜라이더 얻기 + 콜라이더 설정
	CreateCollider();
	GetCollider()->SetScale(Vec2(40.f, 40.f));
}

CMonster::~CMonster()
{
}

void CMonster::OnCollisionEnter(CCollider* _pOther)
{
	CObject* pOtherObj = _pOther->GetObj();

	if (pOtherObj->GetName() == L"Missile_Player")
	{
		CCamera::GetInstance()->SetVibrateCamera(30.f, 1, 0.02f);
		m_iHP -= 1;
		if(m_iHP < 0)
 			DeleteObj(this);
	}
}

void CMonster::Update()
{
}

void CMonster::Render(HDC hdc)
{
}

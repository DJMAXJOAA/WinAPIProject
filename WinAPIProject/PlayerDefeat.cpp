#include "pch.h"
#include "PlayerDefeat.h"

#include "CScene_Battle.h"
#include "CPlayer.h"

#include "CSound.h"
#include "CAnimator.h"
#include "CAnimation.h"

#include "CResMgr.h"

void PlayerDefeat::Init(CScene_Battle* _pScene)
{
	// �й� ȿ���� ���
	CResMgr::GetInstance()->LoadSound(L"loseSound", L"sound\\lose.wav")->Play(false);

	// ī�޶� ȿ�� :: 2�� ��, ���̵�ƿ� -> �ϳѱ� -> ���̵���
	CCamera::GetInstance()->WhiteScreen(1.0f);
	CCamera::GetInstance()->FadeOut(1.0f);
	CCamera::GetInstance()->Event(0.01f);
	CCamera::GetInstance()->BlackScreen(2.0f);
	CCamera::GetInstance()->FadeIn(1.0f);

	printf("CScene_Battle::TurnInit :: �������� �й��߽��ϴ�. �ٽ� ���ư��ϴ�.\n");

}

void PlayerDefeat::Handle(CScene_Battle* _pScene)
{
	CPlayer* m_pPlayer = _pScene->GetPlayer();
	
	m_pPlayer->GetAnimator()->PlayType(L"front_damaged", true);
}

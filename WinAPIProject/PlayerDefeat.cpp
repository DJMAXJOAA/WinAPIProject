#include "pch.h"
#include "PlayerDefeat.h"

#include "CScene_Battle.h"
#include "CPlayer.h"

#include "CSound.h"
#include "CAnimator.h"
#include "CAnimation.h"

void PlayerDefeat::Init(CScene_Battle* _pScene)
{
	// 상태 변경
	_pScene->SetBattleState(TURN_TYPE::DEFEAT);

	// 패배 효과음 재생
	_pScene->GetSoundEffect()[(int)SOUND_TYPE::LOSE]->Play(false);

	// 카메라 효과 :: 2초 후, 페이드아웃 -> 턴넘김 -> 페이드인
	CCamera::GetInstance()->WhiteScreen(1.0f);
	CCamera::GetInstance()->FadeOut(1.0f);
	CCamera::GetInstance()->Event(0.01f);
	CCamera::GetInstance()->BlackScreen(2.0f);
	CCamera::GetInstance()->FadeIn(1.0f);

	printf("CScene_Battle::TurnInit :: 전투에서 패배했습니다. 다시 돌아갑니다.\n");

}

void PlayerDefeat::Handle(CScene_Battle* _pScene)
{
	CPlayer* m_pPlayer = _pScene->GetPlayer();
	
	m_pPlayer->GetAnimator()->PlayType(L"front_damaged", true);
}

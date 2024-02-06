#include "pch.h"
#include "PlayerWin.h"

#include "CScene_Battle.h"
#include "CPlayer.h"

#include "CAnimator.h"
#include "CAnimation.h"

#include "CSound.h"
#include "GameData.h"
#include "CDataMgr.h"

void PlayerWin::Init(CScene_Battle* _pScene)
{
	CPlayer* m_pPlayer = _pScene->GetPlayer();

	// 상태 변경
	_pScene->SetBattleState(TURN_TYPE::WIN);

	// 패배 효과음 재생
	_pScene->GetSoundEffect()[(int)SOUND_TYPE::WIN]->Play(false);

	// 카메라 효과 :: 2초 후, 페이드아웃 -> 턴넘김 -> 페이드인
	CCamera::GetInstance()->WhiteScreen(0.5f);
	CCamera::GetInstance()->FadeOut(1.0f);
	CCamera::GetInstance()->Event(0.01f);
	CCamera::GetInstance()->BlackScreen(1.0f);
	CCamera::GetInstance()->FadeIn(1.0f);

	// 맵에 정보 반영
	GameData* data = (GameData*)CDataMgr::GetInstance()->FindData(0);
	data->m_vecRoute.push_back(data->m_currentGridPos);

	// 게임 데이터 저장
	data->m_PlayerInfo.fCurHP = m_pPlayer->GetHP();
	data->m_PlayerInfo.iMoney = m_pPlayer->GetMoney();

	data->SaveData();

	printf("CScene_Battle::TurnInit :: 전투에서 승리했습니다! 정보가 저장되었습니다. \n");
}

void PlayerWin::Handle(CScene_Battle* _pScene)
{
	CPlayer* m_pPlayer = _pScene->GetPlayer();

	m_pPlayer->GetAnimator()->PlayType(L"front_bow", true);
}
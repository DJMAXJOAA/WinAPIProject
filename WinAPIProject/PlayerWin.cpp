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

	// ���� ����
	_pScene->SetBattleState(TURN_TYPE::WIN);

	// �й� ȿ���� ���
	_pScene->GetSoundEffect()[(int)SOUND_TYPE::WIN]->Play(false);

	// ī�޶� ȿ�� :: 2�� ��, ���̵�ƿ� -> �ϳѱ� -> ���̵���
	CCamera::GetInstance()->WhiteScreen(0.5f);
	CCamera::GetInstance()->FadeOut(1.0f);
	CCamera::GetInstance()->Event(0.01f);
	CCamera::GetInstance()->BlackScreen(1.0f);
	CCamera::GetInstance()->FadeIn(1.0f);

	// �ʿ� ���� �ݿ�
	GameData* data = (GameData*)CDataMgr::GetInstance()->FindData(0);
	data->m_vecRoute.push_back(data->m_currentGridPos);

	// ���� ������ ����
	data->m_PlayerInfo.fCurHP = m_pPlayer->GetHP();
	data->m_PlayerInfo.iMoney = m_pPlayer->GetMoney();

	data->SaveData();

	printf("CScene_Battle::TurnInit :: �������� �¸��߽��ϴ�! ������ ����Ǿ����ϴ�. \n");
}

void PlayerWin::Handle(CScene_Battle* _pScene)
{
	CPlayer* m_pPlayer = _pScene->GetPlayer();

	m_pPlayer->GetAnimator()->PlayType(L"front_bow", true);
}
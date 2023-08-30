#include "pch.h"
#include "PlayerWin.h"

#include "CScene_Battle.h"
#include "CPlayer.h"

#include "CAnimator.h"
#include "CAnimation.h"

void PlayerWin::Handle(CScene_Battle* _pScene)
{
	CPlayer* m_pPlayer = _pScene->GetPlayer();

	m_pPlayer->GetAnimator()->PlayType(L"front_bow", true);
}
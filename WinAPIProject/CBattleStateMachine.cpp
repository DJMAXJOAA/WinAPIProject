#include "pch.h"
#include "CBattleStateMachine.h"

#include "CBattleState.h"
#include "CScene_Battle.h"

#include "EnterBattle.h"
#include "PlayerStart.h"
#include "PlayerTurn_TileSelect.h"
#include "PlayerTurn_Move.h"
#include "PlayerTurn_Attack.h"
#include "PlayerTurn_Skill.h"
#include "EnemyTurn_Move.h"
#include "PlayerWin.h"
#include "PlayerDefeat.h"

CBattleStateMachine::CBattleStateMachine(CScene_Battle* _pScene)
	: m_pScene(_pScene)
	, m_pBattleState(nullptr)
{
	m_vecStates.resize((int)TURN_TYPE::END, nullptr);

	// 씬의 State들을 추가
	m_vecStates[(int)TURN_TYPE::ENTER] = new EnterBattle;
	m_vecStates[(int)TURN_TYPE::PLAYER_START] = new PlayerStart;
	m_vecStates[(int)TURN_TYPE::PLAYER_TILESELECT] = new PlayerTurn_TileSelect;
	m_vecStates[(int)TURN_TYPE::PLAYER_MOVE] = new PlayerTurn_Move;
	m_vecStates[(int)TURN_TYPE::PLAYER_ATTACK] = new PlayerTurn_Attack;
	m_vecStates[(int)TURN_TYPE::PLAYER_SKILL] = new PlayerTurn_Skill;
	m_vecStates[(int)TURN_TYPE::ENEMY_MOVE] = new EnemyTurn;
	m_vecStates[(int)TURN_TYPE::WIN] = new PlayerWin;
	m_vecStates[(int)TURN_TYPE::DEFEAT] = new PlayerDefeat;

	ChangeState(TURN_TYPE::ENTER);
}

CBattleStateMachine::~CBattleStateMachine()
{
	SafeDeleteVec(m_vecStates);
}

void CBattleStateMachine::Handle(CScene_Battle* _pScene)
{
	m_pBattleState->Handle(_pScene);
}

void CBattleStateMachine::ChangeState(TURN_TYPE _type)
{
	if(m_pBattleState != nullptr)
		m_pBattleState->Exit(m_pScene);

	m_pBattleState = m_vecStates[(int)_type];
	m_pBattleState->Init(m_pScene);
}

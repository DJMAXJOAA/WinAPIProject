#pragma once

class CScene_Battle;

// 배틀 씬의 상태 로직을 클래스로 나눠놓음
class CBattleState
{
public:
	CBattleState() {};
	virtual ~CBattleState() {};

public:
	virtual void OnTileSelect(CScene_Battle* _pScene, CObject* _pObj) {};

public:
	virtual void Init(CScene_Battle* _pScene) {};
	virtual void Handle(CScene_Battle* _pScene) {};
	virtual void Exit(CScene_Battle* _pScene) {};

};


#pragma once

class CScene_Battle;

// ��Ʋ ���� ���� ������ Ŭ������ ��������
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


#pragma once

class IStategyTurn
{
public:
	IStategyTurn() {};
	virtual ~IStategyTurn() {};

	friend class CScene_Battle;

public:
	virtual void TurnUpdate(CScene_Battle* _scene) = 0;
};


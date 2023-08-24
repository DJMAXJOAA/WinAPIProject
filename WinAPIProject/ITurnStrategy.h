#pragma once

class CScene_Battle;

class ITurnStrategy
{
public:
	virtual ~ITurnStrategy() {};

public:
	virtual void TurnType() = 0;
};


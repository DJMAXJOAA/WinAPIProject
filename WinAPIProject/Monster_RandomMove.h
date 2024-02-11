#pragma once
#include "CMonsterStrategy.h"

#include "tile.h"

class Monster_RandomMove :
    public IMonsterStrategy
{
public:
    virtual void Handle(CScene_Battle* _pScene, CMonster* _pMon) override;

private:
    Vec2 RandomVec2(vector<vector<TileState>>& vecTiles, Vec2 _vPos);
    int RandomInteger(int _iVal, int _iRange);
};


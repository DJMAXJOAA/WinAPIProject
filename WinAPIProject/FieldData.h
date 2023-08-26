#pragma once
#include "CData.h"

class FieldData :
    public CData
{
public:
    FIELD_TYPE              m_MapType;       // 일반방, 엘리트방, 보스방
    BLOCK_TYPE              m_BlockType;     // 맵의 블럭 타입
    vector<pair<int, int>>  m_vecMonsters;   // 몬스터 생성 종류, 몇마리?
    int                     m_iDifficulty;   // 방 난이도

public:
    FieldData(int _key);
    ~FieldData();

public:
    virtual void ParseData(const json& item) override;
};


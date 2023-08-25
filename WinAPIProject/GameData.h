#pragma once
#include "CData.h"

struct PlayerInfo
{
    float	fMoney;				// 현재 돈
    POINT	ptMyLocation;		// 플레이어의 현재 스테이지 좌표 (진행도)

    float	fMaxHP;				// 최대체력
    float	fCurHP;				// 현재체력

    vector<wstring> vecInventory;	// 현재 장착중인 아이템 목록
    vector<wstring> vecSkill;		// 현재 장착중인 스킬 목록
};

// json의 직렬화, 역직렬화
void to_json(json& j, const PlayerInfo& p);
void from_json(const json& j, PlayerInfo& p);

class GameData :
    public CData
{
private:
    vector<vector<int>>     m_vecMap;       // 맵정보 2차원 배열
    PlayerInfo              m_PlayerInfo;   // 캐릭터 관련 정보
    float                   m_fPlaytime;    // 현재 플레이 타임

public:
    GameData(int _key);
    ~GameData();

public:
    vector<vector<int>> GetMap() const { return m_vecMap; }
    void SetMap(const vector<vector<int>>& _vecMap) { m_vecMap = _vecMap; }

public:
    virtual void SaveData();
    virtual void ParseData(const json& item) override;
};


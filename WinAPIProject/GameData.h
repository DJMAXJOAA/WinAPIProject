#pragma once
#include "CData.h"

struct PlayerInfo
{
    int 	iMoney;				// 현재 돈
    float	fMaxHP;				// 최대 체력
    float	fCurHP;				// 현재 체력
    float   fAtt;               // 현재 공격력

    vector<wstring> vecInventory;	// 현재 장착중인 아이템 목록
    vector<wstring> vecSkill;		// 현재 장착중인 스킬 목록
};

// json의 직렬화, 역직렬화
void to_json(json& j, const PlayerInfo& p);
void from_json(const json& j, PlayerInfo& p);

class GameData :
    public CData
{
public:
    // 세이브 데이터로 연동되는 정보들
    vector<vector<int>>     m_vecMap;       // 맵정보 2차원 배열
    vector<Vec2>            m_vecRoute;     // 현재 캐릭터가 지나간 루트 배열 -> 리스트의 마지막 위치가 현재 캐릭터 위치
    PlayerInfo              m_PlayerInfo;   // 캐릭터 관련 정보

    // 실시간 정보
    Vec2                    m_currentGridPos;   // 현재 격자 좌표에서 어디 있는지?

public:
    GameData(int _key);
    ~GameData();

public:
    void Init();        // 초기 게임 시작시 데이터 설정

public:
    vector<vector<int>> GetMap() const { return m_vecMap; }
    void SetMap(const vector<vector<int>>& _vecMap) { m_vecMap = _vecMap; }

public:
    virtual void SaveData();
    virtual void ParseData(const json& item) override;
};


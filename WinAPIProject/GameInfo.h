#pragma once
#include "CData.h"

struct Node
{
	int x;
	int y;
	int value;
	vector<Node*> children;  // 노드의 자식들 저장 (길찾기 루트)

	Node() : x(0), y(0), value(0) {};
	~Node()
	{
		for (Node* child : children)
			delete child;
	}
};
struct PlayerInfo
{
	float	fMoney;			// 현재 돈
	POINT	ptMyLocation;	// 플레이어의 현재 스테이지 좌표 (진행도)

	float	fMaxHP;			// 최대체력
	float	fCurHP;			// 현재체력

	vector<wstring>	vecInventory;	// 현재 장착중인 아이템 목록
	vector<wstring> vecSkill;		// 현재 장착중인 스킬 목록

	json to_json()
	{
		json j;
		j["Money"] = fMoney;
		j["XPos"] = ptMyLocation.x;
		j["YPos"] = ptMyLocation.y;
		j["MaxHP"] = fMaxHP;
		j["CurrentHP"] = fCurHP;
		j["Inventory"] = vecInventory;
		j["Skill"] = vecSkill;
	}
	void from_json(const json& j)
	{
		fMoney = j["Money"];
		ptMyLocation.x = j["Xpos"];
		ptMyLocation.y = j["YPos"];
		fMaxHP = j["MaxHP"];
		fCurHP = j["CurrentHP"];
		vecInventory = j["Inventory"];
		vecSkill = j["Skill"];
	}
};

class GameInfo :
	public CData
{
public:
	// 맵 불러온 후에 계산
	vector<Node*>		m_vecStartPos;		// 시작점으로부터 트리구조의 길찾기

	// json 세이브, 로드
	vector<vector<int>>	m_vecMap;			// 맵정보 2차원 배열
	PlayerInfo			m_PlayerInfo;		// 캐릭터 관련 정보
	float				m_fPlaytime;		// 현재 플레이타임

public:
	GameInfo(int _key);
	~GameInfo();

public:
	// 세이브파일 Save, Load
	void SaveData();
	virtual void LoadData() override;

public:
	// 맵 생성 관련
	void CreateRandomMap();		// 랜덤 맵 생성
	void CreateStartPos();		// 맵 생성된 정보를 토대로 길찾기 트리구조 생성

	Node* buildTree(int x, int y);

	int getValueRandomY();
	int getRandomStageSelect();
};


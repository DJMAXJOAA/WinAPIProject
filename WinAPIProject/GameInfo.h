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
struct MapInfo
{
	vector<vector<int>>	vecMap;			// 맵정보 2차원 배열
	vector<Node*>		vecStartPos;	// 시작점으로부터 트리구조의 길찾기
};
struct PlayerInfo
{
	float	fMoney;			// 현재 돈
	POINT	ptMyLocation;	// 플레이어의 현재 스테이지 좌표 (진행도)

	float	fMaxHP;			// 최대체력
	float	fCurHP;			// 현재체력

	vector<wstring>	vecInventory;	// 현재 장착중인 아이템 목록
	vector<wstring> vecSkill;		// 현재 장착중인 스킬 목록
};

class GameInfo :
	public CData
{
public:
	MapInfo		m_MapInfo;		// 맵 정보
	PlayerInfo	m_PlayerInfo;	// 캐릭터 관련 정보
	float		m_fPlaytime;	// 현재 플레이타임

public:
	GameInfo(int _key);
	~GameInfo();

public:
	// 세이브파일 Save, Load
	void SaveData();
	virtual void LoadData() override;

public:
	void SaveInfo(const wstring& _strRelativePath);
	void LoadInfo(const wstring& _strRelativePath);

public:
	// 맵 관련
	void CreateRandomMap();
	void SaveMapData();
	void LoadMapData();

	// 맵 생성 관련 (플레이어가 갈 수 있는 루트 정하기)
	Node* buildTree(const vector<vector<int>>& grid, int x, int y);

	int getValueRandomY();
	int getRandomStageSelect();
};


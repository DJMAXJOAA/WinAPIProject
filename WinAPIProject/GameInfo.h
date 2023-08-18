#pragma once
#include "CData.h"

struct Node
{
	int x;
	int y;
	int value;
	vector<Node*> children;  // ����� �ڽĵ� ���� (��ã�� ��Ʈ)

	Node() : x(0), y(0), value(0) {};
	~Node()
	{
		for (Node* child : children)
			delete child;
	}
};
struct MapInfo
{
	vector<vector<int>>	vecMap;			// ������ 2���� �迭
	vector<Node*>		vecStartPos;	// ���������κ��� Ʈ�������� ��ã��
};
struct PlayerInfo
{
	float	fMoney;			// ���� ��
	POINT	ptMyLocation;	// �÷��̾��� ���� �������� ��ǥ (���൵)

	float	fMaxHP;			// �ִ�ü��
	float	fCurHP;			// ����ü��

	vector<wstring>	vecInventory;	// ���� �������� ������ ���
	vector<wstring> vecSkill;		// ���� �������� ��ų ���
};

class GameInfo :
	public CData
{
public:
	MapInfo		m_MapInfo;		// �� ����
	PlayerInfo	m_PlayerInfo;	// ĳ���� ���� ����
	float		m_fPlaytime;	// ���� �÷���Ÿ��

public:
	GameInfo(int _key);
	~GameInfo();

public:
	// ���̺����� Save, Load
	void SaveData();
	virtual void LoadData() override;

public:
	void SaveInfo(const wstring& _strRelativePath);
	void LoadInfo(const wstring& _strRelativePath);

public:
	// �� ����
	void CreateRandomMap();
	void SaveMapData();
	void LoadMapData();

	// �� ���� ���� (�÷��̾ �� �� �ִ� ��Ʈ ���ϱ�)
	Node* buildTree(const vector<vector<int>>& grid, int x, int y);

	int getValueRandomY();
	int getRandomStageSelect();
};


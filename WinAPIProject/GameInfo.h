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
struct PlayerInfo
{
	float	fMoney;			// ���� ��
	POINT	ptMyLocation;	// �÷��̾��� ���� �������� ��ǥ (���൵)

	float	fMaxHP;			// �ִ�ü��
	float	fCurHP;			// ����ü��

	vector<wstring>	vecInventory;	// ���� �������� ������ ���
	vector<wstring> vecSkill;		// ���� �������� ��ų ���

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
	// �� �ҷ��� �Ŀ� ���
	vector<Node*>		m_vecStartPos;		// ���������κ��� Ʈ�������� ��ã��

	// json ���̺�, �ε�
	vector<vector<int>>	m_vecMap;			// ������ 2���� �迭
	PlayerInfo			m_PlayerInfo;		// ĳ���� ���� ����
	float				m_fPlaytime;		// ���� �÷���Ÿ��

public:
	GameInfo(int _key);
	~GameInfo();

public:
	// ���̺����� Save, Load
	void SaveData();
	virtual void LoadData() override;

public:
	// �� ���� ����
	void CreateRandomMap();		// ���� �� ����
	void CreateStartPos();		// �� ������ ������ ���� ��ã�� Ʈ������ ����

	Node* buildTree(int x, int y);

	int getValueRandomY();
	int getRandomStageSelect();
};


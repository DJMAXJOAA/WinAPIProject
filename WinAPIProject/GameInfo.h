#pragma once
#include "CData.h"

// Ŀ�Կ� ����
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
	float	fMoney;				// ���� ��
	POINT	ptMyLocation;		// �÷��̾��� ���� �������� ��ǥ (���൵)

	float	fMaxHP;				// �ִ�ü��
	float	fCurHP;				// ����ü��

	vector<wstring> vecInventory;	// ���� �������� ������ ���
	vector<wstring> vecSkill;		// ���� �������� ��ų ���
};

// json�� ����ȭ, ������ȭ
void to_json(json& j, const PlayerInfo& p);
void from_json(const json& j, PlayerInfo& p);

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
	virtual void SaveData();
	virtual void ParseData(const json& item) override;

public:
	// �� ���� ����
	void CreateRandomMap();		// ���� �� ����
	void CreateStartPos();		// �� ������ ������ ���� ��ã�� Ʈ������ ����

	Node* buildTree(int x, int y);

	int getValueRandomY();
	int getRandomStageSelect();
};


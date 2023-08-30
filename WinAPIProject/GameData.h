#pragma once
#include "CData.h"
#include "CMapGenerator.h"

struct PlayerInfo
{
    int 	iMoney;				// ���� ��
    float	fMaxHP;				// �ִ� ü��
    float	fCurHP;				// ���� ü��
    float   fAtt;               // ���� ���ݷ�

    vector<wstring> vecInventory;	// ���� �������� ������ ���
    vector<wstring> vecSkill;		// ���� �������� ��ų ���
};

// json�� ����ȭ, ������ȭ
void to_json(json& j, const PlayerInfo& p);
void from_json(const json& j, PlayerInfo& p);

class GameData :
    public CData
{
public:
    // ���̺� �����ͷ� �����Ǵ� ������
    vector<vector<int>>     m_vecMap;       // ������ 2���� �迭
    vector<Vec2>            m_vecRoute;     // ���� ĳ���Ͱ� ������ ��Ʈ �迭 -> ����Ʈ�� ������ ��ġ�� ���� ĳ���� ��ġ
    PlayerInfo              m_PlayerInfo;   // ĳ���� ���� ����

    // �ǽð� ����
    Vec2                    m_currentGridPos;   // ���� ���� ��ǥ���� ��� �ִ���?

public:
    GameData(int _key);
    ~GameData();

public:
    void Init();        // �ʱ� ���� ���۽� ������ ����

public:
    vector<vector<int>> GetMap() const { return m_vecMap; }
    void SetMap(const vector<vector<int>>& _vecMap) { m_vecMap = _vecMap; }

public:
    virtual void SaveData();
    virtual void ParseData(const json& item) override;
};


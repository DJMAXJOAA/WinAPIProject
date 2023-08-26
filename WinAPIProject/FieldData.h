#pragma once
#include "CData.h"

class FieldData :
    public CData
{
public:
    FIELD_TYPE              m_MapType;       // �Ϲݹ�, ����Ʈ��, ������
    BLOCK_TYPE              m_BlockType;     // ���� �� Ÿ��
    vector<pair<int, int>>  m_vecMonsters;   // ���� ���� ����, ���?
    int                     m_iDifficulty;   // �� ���̵�

public:
    FieldData(int _key);
    ~FieldData();

public:
    virtual void ParseData(const json& item) override;
};


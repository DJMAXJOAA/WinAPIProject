#pragma once
#include "CScene.h"
#include "CMapGenerator.h"

class CPanelUI_Back;
class CBtnUI_Stage;

class CScene_Robby :
    public CScene
{
private:
    CMapGenerator*                      m_MapGenerator;     // ���� ����
    vector<vector<int>>                 m_vecStage;         // �� ����
    vector<CBtnUI_Stage*>               m_vecNodes;         // ���� ����
    map<Vec2, CBtnUI_Stage*>            m_mapGridBtn;       // ���� -> ��¥ ��ǥ

    CPanelUI_Back*      m_mainUI;           // ���� �г� UI

    Vec2                m_CameraPos;        // �¿�Ű�� ī�޶� �̵�����

public:
    CScene_Robby();
    ~CScene_Robby();

public:
    virtual void Render(HDC hdc) override;
    virtual void Update() override;
    virtual void Enter() override;
    virtual void Exit() override;
};


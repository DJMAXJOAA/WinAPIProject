#pragma once
#include "CScene.h"
#include "CMapGenerator.h"

class CPanelUI_Back;
class CBtnUI_Stage;

class CScene_Robby :
    public CScene
{
private:
    vector<MapNode*>            m_vecNodes;         // ���� �迭 ����
    vector<vector<int>>         m_vecStage;         // grid ���� ����
    MapNode*                    m_pCurrentNode;     // ���� ���� ��� ��忡 �ִ���?
    list<Vec2>                  m_lstRoute;         // ���� ��� ���� ��������? (���̺��)

    map<Vec2, MapNode*>         m_mapGridNode;      // ���� -> ���ʰ� ����
    map<Vec2, CBtnUI_Stage*>    m_mapBtnUI;         // ���� ��ǥ -> ��ư�� ����

    Vec2                m_CameraPos;        // ī�޶� ��ġ ����
    CPanelUI_Back*      m_mainUI;           // ���� �г� UI

public:
    CScene_Robby();
    virtual ~CScene_Robby();

public:
    void NodeInit();
    void OnEnterStage(Vec2 _vPos);
    virtual void CameraEvent() override;

public:
    virtual void Render(HDC hdc) override;
    virtual void Update() override;
    virtual void Enter() override;
    virtual void Exit() override;
};


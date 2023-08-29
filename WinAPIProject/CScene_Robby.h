#pragma once
#include "CScene.h"
#include "CMapGenerator.h"

class CPanelUI_Back;
class CBtnUI_Stage;

class CScene_Robby :
    public CScene
{
private:
    CMapGenerator*                      m_MapGenerator;     // 노드맵 생성
    vector<vector<int>>                 m_vecStage;         // 맵 생성
    vector<CBtnUI_Stage*>               m_vecNodes;         // 노드맵 생성
    map<Vec2, CBtnUI_Stage*>            m_mapGridBtn;       // 격자 -> 진짜 좌표

    CPanelUI_Back*      m_mainUI;           // 메인 패널 UI

    Vec2                m_CameraPos;        // 좌우키로 카메라 이동가능

public:
    CScene_Robby();
    ~CScene_Robby();

public:
    virtual void Render(HDC hdc) override;
    virtual void Update() override;
    virtual void Enter() override;
    virtual void Exit() override;
};


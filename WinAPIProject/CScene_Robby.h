#pragma once
#include "CScene.h"
#include "CMapGenerator.h"

class CPanelUI_Back;
class CBtnUI_Stage;

class CScene_Robby :
    public CScene
{
private:
    vector<MapNode*>                    m_vecNodes;         // 노드맵 배열 생성

    vector<vector<int>>                 m_vecStage;         // 맵 생성 -> 데이터 매니저에서 값을 가져와서 복사생성
    map<Vec2, MapNode*>                 m_mapGridNode;      // 격자 -> 노드맵과 연결
    map<Vec2, CBtnUI_Stage*>            m_mapBtnUI;         // 격자 좌표 -> 버튼과 연결

    CPanelUI_Back*      m_mainUI;           // 메인 패널 UI

    Vec2                m_CameraPos;        // 카메라 위치 저장

    MapNode*            m_pCurrentNode;     // 현재 내가 어느 노드에 있는지?
    list<Vec2>          m_lstRoute;         // 내가 어느 길들로 갔었는지? (세이브용)

public:
    CScene_Robby();
    ~CScene_Robby();

public:
    void NodeInit();
    void EnterStage(Vec2 _vPos);
    virtual void CameraEvent() override;

public:
    virtual void Render(HDC hdc) override;
    virtual void Update() override;
    virtual void Enter() override;
    virtual void Exit() override;
};


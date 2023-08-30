#pragma once
#include "CBtnUI.h"

class CScene_Robby;
typedef void(CScene_Robby::* SCENE_BTNCLICK)(Vec2);

class CBtnUI_Stage :
    public CBtnUI
{
private:
    FIELD_TYPE              m_FieldType;    // 스테이지 타입
    bool                    m_bSelect;      // 버튼 선택 가능 여부
    bool                    m_bCleared;     // 클리어 여부

    SCENE_BTNCLICK          m_pBtnSelect;
    CScene_Robby*           m_pSceneRobby;

public:
    CBtnUI_Stage();
    CBtnUI_Stage(int _value);
    ~CBtnUI_Stage();

    CLONE(CBtnUI)

public:
    FIELD_TYPE GetFieldType() { return m_FieldType; }

    bool CanSelect() { return m_bSelect; }

    void SetSelect(bool _bTF) { m_bSelect = _bTF; }

public:
    virtual void MouseOn();
    virtual void MouseLbtnDown();
    virtual void MouseLbtnUp();
    virtual void MouseLbtnClicked();

    void SetClickedCallBack(CScene_Robby* _pScene, SCENE_BTNCLICK _pSceneFunc)
    {
        m_pSceneRobby = _pScene;
        m_pBtnSelect = _pSceneFunc;
    }

public:
    virtual void MouseOnCheck() override;

public:
    virtual void Render(HDC hdc) override;
    virtual void Update() override;
};


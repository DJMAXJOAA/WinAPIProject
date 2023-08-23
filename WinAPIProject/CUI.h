#pragma once
#include "CObject.h"

class CUI :
    public CObject
{
private:
    vector<CUI*>        m_vecChildUI;   // 자식 UI들
    CUI*                m_pParentUI;    // 부모 UI

    Vec2                m_vFinalPos;    // 최종 위치 

    bool                m_bCamAffected; // 카메라 영향을 받는 UI인지?
    bool                m_bMouseOn;     // UI위에 마우스가 있는지? (finalupdate)
    bool                m_bLbtnDown;    // UI가 왼쪽 버튼이 눌린적 있었는지?

public:
    CUI(bool _bCamAff);         // 기본 생성자 없이 무조건 유무를 받아야 한다
    CUI(const CUI& _origin);    // UI 클론시 사용할 생성자
    virtual ~CUI();

    friend class CUIMgr;
    virtual CUI* Clone() = 0;

public:
    virtual void AnimationFinishEvent() override {}

public:
    CUI* GetParent() { return m_pParentUI; }
    const vector<CUI*>& GetChildUI() { return m_vecChildUI; }
    Vec2 GetFinalPos() { return m_vFinalPos; }

    bool IsMouseOn() { return m_bMouseOn; }
    bool IsLbtnDown() { return m_bLbtnDown; }

public:
    void AddChild(CUI* _pUI) { m_vecChildUI.push_back(_pUI); _pUI->m_pParentUI = this; }

public:
    virtual void MouseOn();             // 마우스가 UI 위에 있음
    virtual void MouseLbtnDown();       // 마우스가 UI 위에서 눌림
    virtual void MouseLbtnUp();         // 마우스가 UI 위에서 떼짐 ( ex : 드래그 상황 )
    virtual void MouseLbtnClicked();    // 마우스가 UI 위에서 눌렸으면서 + 뗄 떼도 UI 위에도 있음

public:
    void MouseOnCheck();

public:
    virtual void Update();
    virtual void FinalUpdate();
    virtual void Render(HDC hdc);

    void Update_Child();
    void FinalUpdate_Child();
    void Render_Child(HDC hdc);
};


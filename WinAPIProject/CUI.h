#pragma once
#include "CObject.h"

class CUI :
    public CObject
{
private:
    vector<CUI*>        m_vecChildUI;   // �ڽ� UI��
    CUI*                m_pParentUI;    // �θ� UI

    Vec2                m_vFinalPos;    // ���� ��ġ 

    bool                m_bCamAffected; // ī�޶� ������ �޴� UI����?
    bool                m_bMouseOn;     // UI���� ���콺�� �ִ���? (finalupdate)
    bool                m_bLbtnDown;    // UI�� ���� ��ư�� ������ �־�����?

public:
    CUI(bool _bCamAff);         // �⺻ ������ ���� ������ ������ �޾ƾ� �Ѵ�
    CUI(const CUI& _origin);    // UI Ŭ�н� ����� ������
    virtual ~CUI();

    friend class CUIMgr;
    virtual CUI* Clone() = 0;

public:
    CUI* GetParent() { return m_pParentUI; }
    const vector<CUI*>& GetChildUI() { return m_vecChildUI; }
    Vec2 GetFinalPos() { return m_vFinalPos; }

    bool IsMouseOn() { return m_bMouseOn; }
    bool IsLbtnDown() { return m_bLbtnDown; }

public:
    void AddChild(CUI* _pUI) { m_vecChildUI.push_back(_pUI); _pUI->m_pParentUI = this; }

public:
    virtual void MouseOn();             // ���콺�� UI ���� ����
    virtual void MouseLbtnDown();       // ���콺�� UI ������ ����
    virtual void MouseLbtnUp();         // ���콺�� UI ������ ���� ( ex : �巡�� ��Ȳ )
    virtual void MouseLbtnClicked();    // ���콺�� UI ������ �������鼭 + �� ���� UI ������ ����

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


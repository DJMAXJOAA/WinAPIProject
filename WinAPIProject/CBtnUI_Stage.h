#pragma once
#include "CBtnUI.h"

class CScene_Robby;
class CTexture;

class CBtnUI_Stage :
    public CBtnUI
{
private:
    CTexture*               m_pCircle;      // Ŭ���� ����

    FIELD_TYPE              m_FieldType;    // �������� Ÿ��
    bool                    m_bSelect;      // ��ư ���� ���� ����
    bool                    m_bCleared;     // Ŭ���� ����

public:
    CBtnUI_Stage(int _value);
    ~CBtnUI_Stage();

    CLONE(CBtnUI)

public:
    FIELD_TYPE GetFieldType() { return m_FieldType; }

    bool CanSelect() { return m_bSelect; }

    void SetSelect(bool _bTF) { m_bSelect = _bTF; }
    void SetClear(bool _bTF) { m_bCleared = _bTF; }

public:
    virtual void MouseOn();
    virtual void MouseLbtnDown();
    virtual void MouseLbtnUp();
    virtual void MouseLbtnClicked();

    void EnterStageEvent(Vec2 _vPos);

public:
    virtual void MouseOnCheck() override;

public:
    virtual void Render(HDC hdc) override;
    virtual void Update() override;
};


#pragma once
class CCollider;

#include "CObject.h"
class CMouse :
    public CObject
{
private:
    CCollider*    m_pOtherCollider;      // ���� �ֱٿ� �浹�� ������Ʈ�� �ݶ��̴�

public:
    CMouse();
    virtual ~CMouse();

private:
    CLONE(CMouse)

public:
    virtual void OnCollisionEnter(CCollider* _pOther);
    void PlayerTileSelectTrigger(CObject* _pObj);

public:
    virtual void Render(HDC hdc);
    virtual void Update();
};


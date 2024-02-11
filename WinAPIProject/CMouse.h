#pragma once
class CCollider;

#include "CObject.h"
class CMouse :
    public CObject
{
private:
    CCollider*    m_pOtherCollider;      // 가장 최근에 충돌한 오브젝트의 콜라이더

public:
    CMouse();
    virtual ~CMouse();

private:
    CLONE(CMouse)

public:
    virtual void OnCollisionEnter(CCollider* _pOther);
    virtual void OnCollision(CCollider* _pOther);
    void SelectTile(CObject* _pObj);
    void ChangeTile(CObject* _pObj);

public:
    virtual void Render(HDC hdc);
    virtual void Update();
};


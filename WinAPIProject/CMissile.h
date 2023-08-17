#pragma once
#include "CObject.h"

class CCollider;

class CMissile :
    public CObject
{
private:
    float   m_fTheta;   // 이동 방향 (각도)
    Vec2    m_vDir; // 내가 나아갈 방향(방향성) -> 벡터 정규화를 위해서

public:
    CMissile();
    ~CMissile();

public:
    void SetDir(float _fTheta) { m_fTheta = _fTheta; }
    void SetDir(Vec2 _vDir) { m_vDir = _vDir; m_vDir.Normalize(); }

public:
    virtual void OnCollisionEnter(CCollider* _pOther);

public:
    virtual void Update();
    virtual void Render(HDC hdc);

    CLONE(CMissile);
};


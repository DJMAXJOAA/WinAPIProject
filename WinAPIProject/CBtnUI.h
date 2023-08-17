#pragma once
#include "CUI.h"

#include "CScene.h"
#include "CObject.h"

// 두개의 타입을 받을수있는 함수 포인터 미리 정의
// 원래는 공통의 클래스를 하나로 묶어서, 하나로만 사용(Engine Object...) -> 유니티의 gameObject....
typedef void(*BTN_FUNC)(DWORD_PTR, DWORD_PTR); 
typedef void(CScene::*SCENE_MEMFUNC)(void);
typedef void(CObject::* OBJECT_MEMFUNC)(void);

class CBtnUI :
    public CUI
{
private:
    BTN_FUNC        m_pFunc;
    DWORD_PTR       m_param1;
    DWORD_PTR       m_param2;

    SCENE_MEMFUNC   m_pSceneFunc;
    CScene*         m_pSceneInst;

public:
    CBtnUI();
    ~CBtnUI();

    CLONE(CBtnUI);

public:
    virtual void MouseOn();
    virtual void MouseLbtnDown();
    virtual void MouseLbtnUp();
    virtual void MouseLbtnClicked();

    void SetClickedCallBack(BTN_FUNC _pFunc, DWORD_PTR _param1, DWORD_PTR _param2)
    {
        m_pFunc = _pFunc;
        m_param1 = _param1;
        m_param2 = _param2;
    }

    void SetClickedCallBack(CScene* _pScene, SCENE_MEMFUNC _pSceneFunc)
    {
        m_pSceneInst = _pScene;
        m_pSceneFunc = _pSceneFunc;
    }
};


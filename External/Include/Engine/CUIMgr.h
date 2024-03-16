#pragma once
#include "CSingleton.h"


class CUIMgr :
    public CSingleton<CUIMgr>
{
    SINGLE(CUIMgr);

private:
    CGameObject* m_pFocusedUI;
    CGameObject* m_pPriorityUI;

private:
    CGameObject* GetPriorityUI(CGameObject* pParentUI);

    bool IsUIObject(CGameObject* _obj);

public:
    void init();
    void tick();

    CGameObject* GetFocusedUI() { return m_pFocusedUI; }

    void CloseUI(CGameObject* pParentUI) {}
    void OpenUI(CGameObject* pParentUI) {}
};


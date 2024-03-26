#pragma once
#include <Engine\CScript.h>

class ER_Cursor :
    public CScript
{
private:
    int m_CsrState;
    int m_PrevState;

    CGameObject* m_pDragItemTex;

public:
    void init();
    virtual void tick() override;

    CGameObject* GetDragItemTex() { return m_pDragItemTex; }

    void SetState(int state);

public:
    virtual void CsrOn();
    virtual void CsrTap();
    virtual void CsrRelease();
    virtual void CsrClick();
    virtual void CsrBeginOn();
    virtual void CsrAway();

public:
    ER_Cursor();
    ~ER_Cursor();

    CLONE_DISABLE(ER_Cursor);
};


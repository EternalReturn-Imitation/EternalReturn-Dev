#pragma once
#include <Engine\CScript.h>

class ER_Cursor :
    public CScript
{
private:
    int m_CsrState;
    int m_PrevState;

public:
    void init();
    virtual void tick() override;

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


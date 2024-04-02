#pragma once
#include <Engine\CScript.h>

class ER_UIScript_TimeUIBar :
    public CScript
{
private:
    CGameObject* m_pClockIcon;
    
    float* m_fGameTime;
    int m_iMinute;
    int m_iSecond;

public:
    void init();
    virtual void begin() override;
    void tick();

public:
    ER_UIScript_TimeUIBar();
    ~ER_UIScript_TimeUIBar();

    CLONE_DISABLE(ER_UIScript_TimeUIBar);

};


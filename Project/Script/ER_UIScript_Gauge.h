#pragma once
#include <Engine\CScript.h>

class ER_UIScript_Gauge :
    public CScript
{
private:

public:
    void init();
    virtual void begin() override;
    virtual void tick() override;

public:
    ER_UIScript_Gauge();
    ~ER_UIScript_Gauge();

    CLONE(ER_UIScript_Gauge);
};


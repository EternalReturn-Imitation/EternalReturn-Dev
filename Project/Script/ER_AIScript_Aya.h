#pragma once
#include "ER_AIScript.h"
class ER_AIScript_Aya :
    public ER_AIScript
{
public:
    virtual void init();
    virtual void begin() override;
    virtual void tick() override;

public:
    ER_AIScript_Aya();
    ~ER_AIScript_Aya();

    CLONE_DISABLE(ER_AIScript_Aya);
};


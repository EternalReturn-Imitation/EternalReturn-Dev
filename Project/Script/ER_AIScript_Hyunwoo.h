#pragma once
#include "ER_AIScript.h"

class ER_AIScript_Hyunwoo :
    public ER_AIScript
{
public:
    void init();
    virtual void begin() override;
    virtual void tick() override;

public:
    ER_AIScript_Hyunwoo();
    ~ER_AIScript_Hyunwoo();

    CLONE_DISABLE(ER_AIScript_Hyunwoo);
};


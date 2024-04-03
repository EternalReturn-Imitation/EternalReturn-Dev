#pragma once
#include "ER_AIScript.h"

class ER_AIScript_Yuki :
    public ER_AIScript
{
public:
    void init();
    virtual void begin() override;
    virtual void tick() override;

public:
    ER_AIScript_Yuki();
    ~ER_AIScript_Yuki();

    CLONE_DISABLE(ER_AIScript_Yuki);
};


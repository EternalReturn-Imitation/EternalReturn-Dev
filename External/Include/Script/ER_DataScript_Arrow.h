#pragma once
#include "ER_DataScript_Projectile.h"
class ER_DataScript_Arrow :
    public ER_DataScript_Projectile
{
public:
    virtual void init();
    virtual void begin() override;
    // virtual void tick() override;

public:
    ER_DataScript_Arrow();
    ~ER_DataScript_Arrow();

    CLONE_DISABLE(ER_DataScript_Arrow);
};


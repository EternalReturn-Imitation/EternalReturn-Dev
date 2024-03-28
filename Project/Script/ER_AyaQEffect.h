#pragma once
#include "ER_EffectScript.h"

class ER_AyaQEffect :
    public ER_EffectScript
{

private:
    virtual void begin() override;
    virtual void tick() override;

public:
    virtual void SaveToLevelFile(FILE* _File) override;
    virtual void LoadFromLevelFile(FILE* _FILE) override;

    CLONE(ER_AyaQEffect);

    ER_AyaQEffect();
    ~ER_AyaQEffect();
};


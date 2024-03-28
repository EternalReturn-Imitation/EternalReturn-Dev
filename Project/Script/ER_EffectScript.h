#pragma once
#include <Engine/CScript.h>

class ER_EffectScript :
    public CScript
{
private:
    float m_fTime;

private:
    virtual void begin() override;
    virtual void tick() override;

public:
    virtual void SaveToLevelFile(FILE* _File) override;
    virtual void LoadFromLevelFile(FILE* _FILE) override;

    CLONE(ER_EffectScript);

    ER_EffectScript();
    ~ER_EffectScript();
};
#pragma once
#include "ER_EffectScript.h"

class ER_RioBAEffect :
    public ER_EffectScript
{
private:
    CGameObject*    m_aEffect[2];

public:
    void EffectSpawn();

private:
    virtual void begin() override;
    virtual void tick() override;

public:
    virtual void SaveToLevelFile(FILE* _File) override;
    virtual void LoadFromLevelFile(FILE* _FILE) override;

    CLONE(ER_RioBAEffect);

    ER_RioBAEffect();
    ~ER_RioBAEffect();
};


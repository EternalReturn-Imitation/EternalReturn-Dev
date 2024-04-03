#pragma once
#include "ER_EffectScript.h"
class ER_YukiWEffect :
    public ER_EffectScript
{
private:
    CGameObject* m_pSakuraAnimation;
    int count;

private:
    virtual void begin() override;
    virtual void tick() override;

public:
    virtual void SpawnEffect(Vec3 _pos = Vec3(0.f, 0.f, 0.f), Vec3 _dir = Vec3(0.f, 0.f, 0.f), float _scale = 0.f) override;
    void SpawnSakuraEffect(CGameObject* _targetObj, float _endTime, CGameObject* _dummpyParent = nullptr);

public:
    virtual void SaveToLevelFile(FILE* _File) override;
    virtual void LoadFromLevelFile(FILE* _FILE) override;

    CLONE(ER_YukiWEffect);

    ER_YukiWEffect();
    ~ER_YukiWEffect();
};


#pragma once
#include "ER_EffectScript.h"
class ER_AyaWBulletEffect :
    public ER_EffectScript
{
private:
    float m_fTime;
    Vec3  m_vEffectDir;

private:
    virtual void begin() override;
    virtual void tick() override;

public:
    virtual void SpawnEffect(Vec3 _pos = Vec3(0.f, 0.f, 0.f), Vec3 _dir = Vec3(0.f, 0.f, 0.f), float _scale = 0.f) override;

public:
    void SetEffectDir(Vec3 _vec) { m_vEffectDir = _vec; }

public:
    virtual void SaveToLevelFile(FILE* _File) override;
    virtual void LoadFromLevelFile(FILE* _FILE) override;

    CLONE(ER_AyaWBulletEffect);

    ER_AyaWBulletEffect();
    ~ER_AyaWBulletEffect();
};


#pragma once
#include "ER_EffectScript.h"
class ER_YukiBAEffect :
    public ER_EffectScript
{
private:
    virtual void begin() override;
    virtual void tick() override;

public:
    virtual void SpawnEffect(Vec3 _pos = Vec3(0.f, 0.f, 0.f), Vec3 _dir = Vec3(0.f, 0.f, 0.f), float _scale = 0.f) override;
    void SpawnFirstEffect(Vec3 _pos = Vec3(0.f, 0.f, 0.f), Vec3 _dir = Vec3(0.f, 0.f, 0.f), float _scale = 0.f);
    void SpawnSecondEffect(Vec3 _pos = Vec3(0.f, 0.f, 0.f), Vec3 _dir = Vec3(0.f, 0.f, 0.f), float _scale = 0.f);

public:
    virtual void SaveToLevelFile(FILE* _File) override;
    virtual void LoadFromLevelFile(FILE* _FILE) override;

    CLONE(ER_YukiBAEffect);

    ER_YukiBAEffect();
    ~ER_YukiBAEffect();
};


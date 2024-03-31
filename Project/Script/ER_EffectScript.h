#pragma once
#include <Engine/CScript.h>
#include <future>

class ER_EffectScript :
    public CScript
{
private:
    float m_fTime;

public:
    virtual void SpawnEffect(Vec3 _pos = Vec3(0.f, 0.f, 0.f), Vec3 _dir = Vec3(0.f, 0.f, 0.f)) {};
    
    void EffectRotation(CGameObject* _targetObj, Vec3 _startScale, Vec3 _endScale, bool _rotDirection, float _endTime);

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
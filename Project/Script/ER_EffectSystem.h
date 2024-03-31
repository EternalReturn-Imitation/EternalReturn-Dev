#pragma once
#include <Engine\CSingleton.h>

class CGameObject;

class ER_EffectSystem :
    public CSingleton<ER_EffectSystem>
{
    SINGLE(ER_EffectSystem);

public:
    void init();

public:
    void SpawnHitEffect(CGameObject* _attacker, CGameObject* _hitter, Vec3 _pos = Vec3(0.f,0.f,0.f), Vec3 _dir = Vec3(0.f,0.f,0.f));

public:
    void SpawnRioHitEffect(Vec3 _pos, Vec3 _dir);
};


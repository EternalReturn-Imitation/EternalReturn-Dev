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
    //노말 히트 이펙트 스폰용
    void SpawnHitEffect(CGameObject* _attacker, CGameObject* _hitter, CGameObject* _projectile = nullptr, Vec3 _pos = Vec3(0.f,0.f,0.f), Vec3 _dir = Vec3(0.f,0.f,0.f));
    //스킬 히트 이펙트 스폰용
    void SpawnSkillHitEffect(CGameObject* _attacker, CGameObject* _hitter, const tSkill_Info* _skillInfo, CGameObject* _projectile = nullptr,  Vec3 _pos = Vec3(0.f, 0.f, 0.f), Vec3 _dir = Vec3(0.f, 0.f, 0.f));


//노말 히트 이펙트    
public:
    void SpawnRioHitEffect(Vec3 _pos, Vec3 _dir, Vec3 _effectMoveDir);

//스킬 히트 이펙트
public:
    void SpawnRioWHitEffect(Vec3 _pos, Vec3 _dir, Vec3 _effectMoveDir, CGameObject* _hitter);
    void SpawnRioRHitEffect(Vec3 _pos, Vec3 _dir, Vec3 _effectMoveDir);

};


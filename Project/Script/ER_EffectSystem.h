#pragma once
#include <Engine\CSingleton.h>
#include "ER_struct.h"

class CGameObject;

class ER_EffectSystem :
    public CSingleton<ER_EffectSystem>
{
    SINGLE(ER_EffectSystem);
private:
    vector<CGameObject*>    m_vDeleteParticles;
    vector<float>           m_vParticleLifeTime;

    int                     m_iYukiCount;

    mutex                   m_aFuncMutex;

public:
    void init();
    void progress();

public:
    //노말 히트 이펙트 스폰용
    void SpawnHitEffect(CGameObject* _attacker, CGameObject* _hitter, CGameObject* _projectile = nullptr, Vec3 _pos = Vec3(0.f,0.f,0.f), Vec3 _dir = Vec3(0.f,0.f,0.f));
    //스킬 히트 이펙트 스폰용
    void SpawnSkillHitEffect(CGameObject* _attacker, CGameObject* _hitter, const tSkill_Info* _skillInfo, CGameObject* _projectile = nullptr,  Vec3 _pos = Vec3(0.f, 0.f, 0.f), Vec3 _dir = Vec3(0.f, 0.f, 0.f));

//노말 히트 이펙트    
public:
    void SpawnRioHitEffect(Vec3 _pos, Vec3 _dir, Vec3 _effectMoveDir);
    void SpawnAyaHitEffect(Vec3 _pos);
    void SpawnYukiHitEffect(Vec3 _pos);
    void SpawnHyunwooHitEffect(Vec3 _pos);

//스킬 히트 이펙트
public:
    //아야 스킬 히트 이펙트
    void SpawnRioWHitEffect(Vec3 _pos, Vec3 _dir, Vec3 _effectMoveDir, CGameObject* _hitter);
    void SpawnRioRHitEffect(Vec3 _pos, Vec3 _dir, Vec3 _effectMoveDir);
    //유키 스킬 히트 이펙트
    void SpawnYukiQHitEffect(Vec3 _pos);
    void SpawnYukiR1HitEffect(Vec3 _pos, CGameObject* _hitter);
    void SpawnYukiR2HitEffect(Vec3 _pos);
    //현우 스킬 히트 이펙트
    void SpawnHyunwooQHitEffect(Vec3 _pos, CGameObject* _hitter);

public:
    void AddDeleteParticles(CGameObject* _obj, CGameObject* _parentObj = nullptr);
};


#pragma once
#include <Engine\CScript.h>
#include "ER_BattleSystem.h"

class ER_DataScript_Projectile :
    public CScript
{
protected:
    enum class eDmgType
    {
        NORMAL,
        SKILL,
        END,
    };
    Ptr<CSound>     m_pSound;

    CGameObject*    m_pShooter;
    CGameObject*    m_pTarget;

    Vec3            m_vSpawnPos;
    Vec3            m_vDir;

    eDmgType        m_DmgType;

    CScript*        m_SkillInst;
    SKILL_DMG_CALC  m_SkillFunc;
    
    float           m_fSpeed;
    float           m_fLifeTime;
    
    virtual void init();

private:
    float SetRotationToTarget(Vec3 _OwnerPos, Vec3 _TargetPos);

public:
    virtual void begin() override;
    virtual void tick() override;
    void Spawn();

    void SetForTarget(CGameObject* _Shooter, CGameObject* _Target, Vec3 _SpawnPos, float _fSpeed);
    void SetForDir(CGameObject* _Shooter, Vec3 _SpawnPos, float _fSpeed, float _fLifeTime);

    void SetSkill(CScript* _Inst, SKILL_DMG_CALC _Func)
    {
        m_DmgType = eDmgType::SKILL;
        m_SkillInst = _Inst;
        m_SkillFunc = _Func;
    };

public:
    virtual void BeginOverlap(CCollider3D* _Other);
    virtual void OnOverlap(CCollider3D* _Other);
    virtual void EndOverlap(CCollider3D* _Other);

public:
    ER_DataScript_Projectile(SCRIPT_TYPE _type);
    ~ER_DataScript_Projectile();

    CLONE_DISABLE(ER_DataScript_Projectile);
};


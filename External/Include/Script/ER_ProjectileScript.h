#pragma once
#include <Engine\CScript.h>
#include "ER_BattleSystem.h"

class ER_ProjectileScript :
    public CScript
{
public:
    enum class eDmgType
    {
        COMMON,
        SKILL,
        END,
    };

    enum eProjectileType
    {
        USELIFETIME,
        GUIDED     ,
        END,
    };


private:
    CGameObject*    m_pShooter;
    CGameObject*    m_pTarget;
    Vec3            m_vSpawnPos;
    Vec3            m_vDir;
    Vec3            m_vProjectileScale;
    eDmgType        m_DmgType;
    eProjectileType m_ProjectileType;
    CScript*        m_SkillInst;
    SKILL_DMG_CALC  m_SkiilFunc;

    float           m_fSpeed;
    float           m_fLifetTime;
    float           m_fFrontDir;

    int             m_iPoolIdx;

    CLONE_DISABLE(ER_ProjectileScript);
private:
    void Dead();
    void SetPoolIdx(int _idx) { m_iPoolIdx = _idx; }
    void SetFrontDir(float _front) { m_fFrontDir = _front; }
    
    float SetRotationToTaret(Vec3 _OwnerPos, Vec3 _TargetPos);

public:
    void SetMeshData(Ptr<CMesh> _pMesh, Ptr<CMaterial> _pMtrl);
    void Shot();
    void ShotTarget(
        CGameObject*    _Shooter
        , CGameObject* _Target
        , Vec3            _SpawnPos
        , eDmgType        _DmgType
        , float _fSpeed
    );

    void ShotDir(
        CGameObject* _Shooter
        , Vec3            _SpawnPos
        , Vec3            _Dir
        , eDmgType        _DmgType
        , float _fSpeed
        , float _fLifeTime
    );

public:
    virtual void begin() override;
    virtual void tick() override;
    
public:
    virtual void BeginOverlap(CCollider3D* _Other);
    virtual void OnOverlap(CCollider3D* _Other);
    virtual void EndOverlap(CCollider3D* _Other);

public:
    ER_ProjectileScript();
    ~ER_ProjectileScript();

    friend class ER_ProjectilePool;
};


#pragma once
#include <Engine\CSingleton.h>

class CMesh;
class CMaterial;
class ER_ProjectileScript;

class ER_ProjectilePool :
    public CSingleton<ER_ProjectilePool>
{
    SINGLE(ER_ProjectilePool);

public:
    enum class eProjType
    {
        ARROW,
        BULLET,
        END,
    };

private:
    vector<pair<bool, CGameObject*>> m_vecProjectilePool;   // 투사체 오브젝트풀

    Ptr<CMesh>      m_pProjMesh[(UINT)eProjType::END];
    Ptr<CMaterial>  m_pProjMtrl[(UINT)eProjType::END];
    Vec3            m_pProjScale[(UINT)eProjType::END];

    int             m_iLastIdx;

private:
    bool SetUse(int _PoolIdx);
    bool SetDead(int _PoolIdx);

public:
    ER_ProjectileScript* GetProjectile(eProjType _type);

public:
    void init();

    friend class ER_ProjectileScript;
};


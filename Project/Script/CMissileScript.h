#pragma once
#include <Engine\CScript.h>

class CMissileScript :
    public CScript
{
private:
    Vec3            m_vDir;     // 이동 방향
    float           m_fSpeed;   // 이동 속력

    CGameObject*    m_TargetObject;

public:
    void SetSpeed(float _Speed) { m_fSpeed = _Speed; }

public:
    virtual void begin() override;
    virtual void tick() override;

    CLONE(CMissileScript);
public:
    CMissileScript();
    ~CMissileScript();
};


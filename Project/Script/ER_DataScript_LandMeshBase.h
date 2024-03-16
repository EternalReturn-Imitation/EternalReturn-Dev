#pragma once
#include <Engine\CScript.h>

class CGameObject;

class ER_DataScript_LandMeshBase :
    public CScript
{
private:
    CGameObject* m_pRoof;

public:
    void init();
    virtual void begin() override;
    virtual void tick() override;

public:
    virtual void BeginOverlap(CCollider3D* _Other) override;
    virtual void OnOverlap(CCollider3D* _Other) override;
    virtual void EndOverlap(CCollider3D* _Other) override;

    virtual void BeginRayOverlap() override;
    virtual void OnRayOverlap() override;
    virtual void EndRayOverlap() override;

public:
    void SetRoof(CGameObject* _roof) { m_pRoof = _roof; }

public:
    ER_DataScript_LandMeshBase();
    ~ER_DataScript_LandMeshBase();

    CLONE_DISABLE(ER_DataScript_LandMeshBase);
};


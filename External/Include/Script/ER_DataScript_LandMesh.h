#pragma once
#include "E:\EldenRing-Dev\External\Include\Engine\CScript.h"
class ER_DataScript_LandMesh :
    public CScript
{
private:


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
    ER_DataScript_LandMesh();
    ~ER_DataScript_LandMesh();

    CLONE_DISABLE(ER_DataScript_LandMesh);
};


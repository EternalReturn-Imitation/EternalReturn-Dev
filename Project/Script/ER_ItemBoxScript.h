#pragma once
#include <Engine\CScript.h>

class ER_ItemBoxScript :
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
    ER_ItemBoxScript();
    ~ER_ItemBoxScript();

    CLONE_DISABLE(ER_ItemBoxScript);
};


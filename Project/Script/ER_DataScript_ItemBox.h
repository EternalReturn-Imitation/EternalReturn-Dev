#pragma once
#include <Engine\CScript.h>

class ER_DataScript_ItemBox :
    public CScript
{
private:
    CGameObject* m_aItemList[2][4];

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
    void CreateRandomItems();

public:
    ER_DataScript_ItemBox();
    ~ER_DataScript_ItemBox();

    CLONE_DISABLE(ER_DataScript_ItemBox);
};
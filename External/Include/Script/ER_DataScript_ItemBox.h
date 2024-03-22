#pragma once
#include <Engine\CScript.h>

class ER_DataScript_ItemBox :
    public CScript
{
private:
    vector<CGameObject*> m_vItemList;
    CGameObject* m_pBoxIcon;

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
    void CreateBoxIcon();

public:
    vector<CGameObject*> GetItemList() { return m_vItemList; }
    CGameObject* GetItem(int _i) { return m_vItemList[_i]; }
    
    void SetEmptyItem(int _i) { m_vItemList[_i] = nullptr; }

public:
    ER_DataScript_ItemBox();
    ~ER_DataScript_ItemBox();

    CLONE_DISABLE(ER_DataScript_ItemBox);
};
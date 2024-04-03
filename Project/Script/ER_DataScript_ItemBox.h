#pragma once
#include <Engine\CScript.h>

class ER_DataScript_ItemBox :
    public CScript
{
    enum class eItemBoxSlotIdx
    {
        SOLT_1,
        SOLT_2,
        SOLT_3,
        SOLT_4,
        SOLT_5,
        SOLT_6,
        SOLT_7,
        SOLT_8,
        END
    }typedef ITEMBOXSLOT;

private:
    // [Data]
    CGameObject* m_pItemList[(UINT)ITEMBOXSLOT::END];           // 아이템 슬롯

    UINT         m_SetItemCnt; // ItemMgr 초기 아이템 세팅확인용

    // [UI]
    CGameObject* m_UIBoxTag;

private:
    Vec3 GetUIPos();

public:
    void            init();
    virtual void    begin() override;
    virtual void    tick() override;

public:
    virtual void    BeginRayOverlap() override;
    virtual void    OnRayOverlap() override;
    virtual void    EndRayOverlap() override;

public:
    bool RegistItem(UINT _ItemID);

public:
    void CreateBoxUI();

public:
    CGameObject** GetItemList() { return m_pItemList; }
    CGameObject* GetItem(int _i) { return m_pItemList[_i]; }
    void SetEmptyItem(int _i) { m_pItemList[_i] = nullptr; }

public:
    ER_DataScript_ItemBox();
    ~ER_DataScript_ItemBox();

    CLONE(ER_DataScript_ItemBox);

    friend class ER_UIScript_ItemBox;
};
#pragma once
#include <Engine\CScript.h>

class ER_UIScript_ItemBox :
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
    CGameObject* m_pBGUI;                               // Slot Bg
    CGameObject* m_pSlotUI[(UINT)ITEMBOXSLOT::END];     // Slot UI

public:
    void init();
    virtual void begin() override;
    virtual void tick() override;

public:
    

public:
    // vector<CGameObject*> GetItemList() { return m_pItemList; }
    // CGameObject* GetItem(int _i) { return m_pItemList[_i]; }
    // 
    // void SetEmptyItem(int _i) { m_pItemList[_i] = nullptr; }

public:
    ER_UIScript_ItemBox();
    ~ER_UIScript_ItemBox();

    CLONE(ER_UIScript_ItemBox);
};


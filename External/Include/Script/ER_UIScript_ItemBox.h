#pragma once
#include <Engine\CScript.h>

class ER_DataScript_ItemBox;

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
    CGameObject*  m_pSlotUI[(UINT)ITEMBOXSLOT::END];     // Slot UI


private:
    Vec3 GetUIPos(CGameObject* _Box);

public:
    void init();
    virtual void begin() override;
    virtual void tick() override;

public:
    void RegistItemBoxData(ER_DataScript_ItemBox* _data);

public:
    ER_UIScript_ItemBox();
    ~ER_UIScript_ItemBox();

    CLONE(ER_UIScript_ItemBox);
};


#pragma once
#include <Engine\CScript.h>

// ER_DataScrip_ItemBox

class ER_UIScript_ItemSlot :
    public CScript
{
private:
    CGameObject** m_Slot;    
    // Box가 들고있는 아이템을 그대로 받아서 보여주기만.

public:
    void            init();
    virtual void    begin() override;
    virtual void    tick() override;
    
    void RegistSlotAdress(CGameObject** _ItemSlotAddress);

public:
    virtual void CsrOn();
    virtual void CsrTap();
    virtual void CsrRelease();
    virtual void CsrClick();
    virtual void CsrBeginOn();
    virtual void CsrAway();

public:
    ER_UIScript_ItemSlot();
    ~ER_UIScript_ItemSlot();

    CLONE(ER_UIScript_ItemSlot);
};


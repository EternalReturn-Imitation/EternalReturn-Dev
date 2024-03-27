#pragma once
#include <Engine\CScript.h>

class ER_UIScript_CraftSlot :
    public CScript
{
private:
    UINT  m_CurItemID;
    UINT    m_Idx;
    const vector<UINT>* m_PlayerCraftList;    // 플레이어 제작목록 벡터 포인터

public:
    void init();
    virtual void tick() override;

public:
    virtual void CsrOn();
    virtual void CsrTap();
    virtual void CsrRelease();
    virtual void CsrClick();
    virtual void CsrBeginOn();
    virtual void CsrAway();

    void RegistCraftvector(const vector<UINT>* _vecCraftItem, UINT idx);

    CLONE(ER_UIScript_CraftSlot);

public:
    ER_UIScript_CraftSlot();
    ~ER_UIScript_CraftSlot();
};


#pragma once
#include <Engine\CScript.h>
#include "ER_DataScript_Character.h"

class ER_UIScript_SkillSlot;

typedef void (ER_UIScript_SkillSlot::* SKILLUP)(void);

class ER_UIScript_SkillUpBtn :
    public CScript
{
private:
    UINT*                       m_SkillPoint;
    int                         m_CsrState;

    ER_UIScript_SkillSlot*      m_Slot;
    SKILLUP                     m_SkillUpFunc;

public:
    void SetUI(int i);

    virtual void begin() override;
    virtual void tick() override;

public:
    virtual void CsrOn();
    virtual void CsrTap();
    virtual void CsrRelease();
    virtual void CsrClick();
    virtual void CsrBeginOn();
    virtual void CsrAway();

    void SetSkill(ER_UIScript_SkillSlot* script, SKILLUP _func)
    {
        m_Slot = script;
        m_SkillUpFunc = _func;
    }
    void SetSkillPiont(UINT* _skillPoint)
    {
        m_SkillPoint = _skillPoint;
    }
    

public:
    ER_UIScript_SkillUpBtn();
    ~ER_UIScript_SkillUpBtn();

    CLONE(ER_UIScript_SkillUpBtn);
};


#pragma once
#include <Engine\CScript.h>
#include "ER_DataScript_Character.h"
#include "ER_define.h"

class ER_UIScript_SkillSlot :
    public CScript
{
private:
    CGameObject*                m_SkillUPBtn;
    CGameObject*                SkillLevelIndicator;

    tSkill_Info**               m_Skill;
    UINT*                       m_SkillPoint;
    
    UINT                        m_CurSkillLv;
    UINT                        m_MaxSkillLv; // 최대 스킬레벨

    ER_DataScript_Character*    m_Data;
    SKILLIDX                    m_SlotIdx;

public:
    void RegistPlayerCharacetr();

    virtual void begin() override;
    virtual void tick() override;

    void SetUI(int i, CGameObject* UpBtn);
    
    void SkillLevelUp();

public:
    ER_UIScript_SkillSlot();
    ~ER_UIScript_SkillSlot();

    CLONE(ER_UIScript_SkillSlot);
};


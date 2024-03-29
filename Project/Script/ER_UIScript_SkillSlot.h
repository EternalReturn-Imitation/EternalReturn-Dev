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

    tSkill_Info*                m_Equipskill;
    tSkill_Info**               m_Skill1;
    tSkill_Info**               m_Skill2;
    UINT*                       m_SkillPoint;
    
    UINT                        m_CurSkillLv;
    UINT                        m_MaxSkillLv; // 최대 스킬레벨

    UINT                        m_CurSkillIdx;        // 스킬 1, 스킬 2 변경

    bool                        m_bToggle;

    ER_DataScript_Character*    m_Data;
    SKILLIDX                    m_SlotIdx;

public:
    void RegistPlayerCharacetr();

    virtual void begin() override;
    virtual void tick() override;

    void SetToggle();

    void SetUI(int i, CGameObject* UpBtn);
    
    void SkillLevelUp();

public:
    ER_UIScript_SkillSlot();
    ~ER_UIScript_SkillSlot();

    CLONE(ER_UIScript_SkillSlot);
};


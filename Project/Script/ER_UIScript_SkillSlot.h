#pragma once
#include <Engine\CScript.h>
#include "ER_DataScript_Character.h"
#include "ER_define.h"

class ER_UIScript_SkillSlot :
    public CScript
{
private:
    ER_DataScript_Character*    m_Data;
    SKILLIDX                    m_SlotIdx;

    CGameObject*                SkillLevelIndicator;
    Ptr<CTexture>               LevelPointTex[6];

public:
    virtual void begin() override;
    virtual void tick() override;

    void SetUI(int i);
    
    // 클릭해서 사용 가능

public:
    ER_UIScript_SkillSlot();
    ~ER_UIScript_SkillSlot();

    CLONE(ER_UIScript_SkillSlot);
};


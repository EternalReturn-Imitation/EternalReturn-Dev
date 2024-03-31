#pragma once
#include <Engine\CSingleton.h>
#include "ER_struct.h"

class ER_ActionScript;

typedef int (CScript::* SKILL_DMG_CALC)();

class ER_BattleSystem :
    public CSingleton<ER_BattleSystem>
{
    SINGLE(ER_BattleSystem);
private:
    int IsCritical(tIngame_Stats* _info);
    tIngame_Stats* GetStatsInfo(CGameObject* _obj);

public:
    void init();

public:
    void Battle_Common(CGameObject* AtkObj, CGameObject* HitObj);
    void Battle_Skill(
        CGameObject* _Attacker, 
        CGameObject* _HitObj, 
        CScript* _Inst, 
        SKILL_DMG_CALC _calc, 
        const tSkill_Info* _SkillInfo,
        const int _HitStep);
    // 형식 예시 : ER_BattleSystem::GetInst()->Battle_Skill(GetOwner(), (CGameObject*)_Data.lParam, this, (SKILL_DMG_CALC)&ER_ActionScript_Yuki::SkillQ, skill);

};


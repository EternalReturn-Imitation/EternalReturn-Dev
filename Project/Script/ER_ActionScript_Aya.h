#pragma once
#include "ER_ActionScript_Character.h"

class ER_ActionScript_Aya :
    public ER_ActionScript_Character
{
private:
    float m_fSec;

protected:
    virtual FSMState* CreateWait();
    virtual FSMState* CreateMove();
    virtual FSMState* CreateFarming();
    virtual FSMState* CreateCraft();
    virtual FSMState* CreateRest();
    virtual FSMState* CreateAttack();
    virtual FSMState* CreateArrive();
    virtual FSMState* CreateDead();
    virtual FSMState* CreateSkill_Q();
    virtual FSMState* CreateSkill_W();
    virtual FSMState* CreateSkill_E();
    virtual FSMState* CreateSkill_R();

public:
    virtual void Skill_Q(tFSMData& _Data);        // Q Skill
    virtual void Skill_W(tFSMData& _Data);        // W Skill
    virtual void Skill_E(tFSMData& _Data);        // E Skill
    virtual void Skill_R(tFSMData& _Data);        // R Skill

private:
    void ArriveEnter(tFSMData& param);
    void ArriveUpdate(tFSMData& param);
    void ArriveExit(tFSMData& param);

    void AttackEnter(tFSMData& param);
    void AttackUpdate(tFSMData& param);
    void AttackExit(tFSMData& param);

    void WaitEnter(tFSMData& param);
    void WaitUpdate(tFSMData& param);
    void WaitExit(tFSMData& param);

    void MoveEnter(tFSMData& param);
    void MoveUpdate(tFSMData& param);
    void MoveExit(tFSMData& param);

    void FarmingEnter(tFSMData& param);
    void FarmingUpdate(tFSMData& param);
    void FarmingExit(tFSMData& param);

    void CraftEnter(tFSMData& param);
    void CraftUpdate(tFSMData& param);
    void CraftExit(tFSMData& param);

    void RestEnter(tFSMData& param);
    void RestUpdate(tFSMData& param);
    void RestExit(tFSMData& param);

    void Skill_QEnter(tFSMData& param);
    void Skill_QUpdate(tFSMData& param);
    void Skill_QExit(tFSMData& param);

    void Skill_WEnter(tFSMData& param);
    void Skill_WUpdate(tFSMData& param);
    void Skill_WExit(tFSMData& param);

    void Skill_EEnter(tFSMData& param);
    void Skill_EUpdate(tFSMData& param);
    void Skill_EExit(tFSMData& param);

    void Skill_REnter(tFSMData& param);
    void Skill_RUpdate(tFSMData& param);
    void Skill_RExit(tFSMData& param);

    void DeadEnter(tFSMData& param);
    void DeadUpdate(tFSMData& param);
    void DeadExit(tFSMData& param);

public:
    ER_ActionScript_Aya();
    ~ER_ActionScript_Aya();

    CLONE(ER_ActionScript_Aya);
};


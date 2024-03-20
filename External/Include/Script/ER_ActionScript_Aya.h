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
    virtual FSMState* CreateDeath();
    virtual FSMState* CreateSkill_Q();
    virtual FSMState* CreateSkill_W();
    virtual FSMState* CreateSkill_E();
    virtual FSMState* CreateSkill_R();

public:
    virtual void Attack(tFSMData& _Data);         // 기본공격
    virtual void Wait(tFSMData& _Data);           // 대기
    virtual void Move(tFSMData& _Data);           // 이동
    virtual void Farming(tFSMData& _Data);        // 파밍
    virtual void Craft(tFSMData& _Data);          // 제작
    virtual void Rest(tFSMData& _Data);           // 휴식
    virtual void Skill_Q(tFSMData& _Data);        // Q Skill
    virtual void Skill_W(tFSMData& _Data);        // W Skill
    virtual void Skill_E(tFSMData& _Data);        // E Skill
    virtual void Skill_R(tFSMData& _Data);        // R Skill

private:
    virtual void begin() override;

    void MoveEnter(tFSMData& param);
    void MoveUpdate(tFSMData& param);

    void FarmingEnter(tFSMData& param);
    void FarmingUpdate(tFSMData& param);
    void FarmingExit(tFSMData& param);

    void WaitEnter(tFSMData& param);

    void ArriveEnter(tFSMData& param);
    void ArriveUpdate(tFSMData& param);

    void CraftEnter(tFSMData& param);
    void CraftUpdate(tFSMData& param);

    void RestEnter(tFSMData& param);
    void RestUpdate(tFSMData& param);

    void Skill_QEnter(tFSMData& param);
    void Skill_QUpdate(tFSMData& param);

    void Skill_WEnter(tFSMData& param);
    void Skill_WUpdate(tFSMData& param);

    void Skill_EEnter(tFSMData& param);
    void Skill_EUpdate(tFSMData& param);

    void Skill_RUpdate(tFSMData& param);

public:
    ER_ActionScript_Aya();
    ~ER_ActionScript_Aya();

    CLONE(ER_ActionScript_Aya);
};


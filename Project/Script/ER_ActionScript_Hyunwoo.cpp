#include "pch.h"
#include "ER_ActionScript_Hyunwoo.h"

ER_ActionScript_Hyunwoo::ER_ActionScript_Hyunwoo()
    : ER_ActionScript_Character(SCRIPT_TYPE::ER_ACTIONSCRIPT_HYUNWOO)
{
}

ER_ActionScript_Hyunwoo::~ER_ActionScript_Hyunwoo()
{
}

FSMState* ER_ActionScript_Hyunwoo::CreateWait()
{
    FSMState* state = new FSMState(this);

    return state;
}

FSMState* ER_ActionScript_Hyunwoo::CreateMove()
{
    FSMState* state = new FSMState(this);

    return state;
}

FSMState* ER_ActionScript_Hyunwoo::CreateCraft()
{
    FSMState* state = new FSMState(this);

    return state;
}

FSMState* ER_ActionScript_Hyunwoo::CreateRest()
{
    FSMState* state = new FSMState(this);

    return state;
}

FSMState* ER_ActionScript_Hyunwoo::CreateAttack()
{
    FSMState* state = new FSMState(this);

    return state;
}

FSMState* ER_ActionScript_Hyunwoo::CreateArrive()
{
    FSMState* state = new FSMState(this);

    return state;
}

FSMState* ER_ActionScript_Hyunwoo::CreateDeath()
{
    FSMState* state = new FSMState(this);

    return state;
}

FSMState* ER_ActionScript_Hyunwoo::CreateSkill_Q()
{
    FSMState* state = new FSMState(this);

    return state;
}

FSMState* ER_ActionScript_Hyunwoo::CreateSkill_W()
{
    FSMState* state = new FSMState(this);

    return state;
}

FSMState* ER_ActionScript_Hyunwoo::CreateSkill_E()
{
    FSMState* state = new FSMState(this);

    return state;
}

FSMState* ER_ActionScript_Hyunwoo::CreateSkill_R()
{
    FSMState* state = new FSMState(this);

    return state;
}

void ER_ActionScript_Hyunwoo::Attack(tFSMData& _Data)
{
}

void ER_ActionScript_Hyunwoo::Wait(tFSMData& _Data)
{
}

void ER_ActionScript_Hyunwoo::Move(tFSMData& _Data)
{
}

void ER_ActionScript_Hyunwoo::Craft(tFSMData& _Data)
{
}

void ER_ActionScript_Hyunwoo::Rest(tFSMData& _Data)
{
}

void ER_ActionScript_Hyunwoo::Skill_Q(tFSMData& _Data)
{
}

void ER_ActionScript_Hyunwoo::Skill_W(tFSMData& _Data)
{
}

void ER_ActionScript_Hyunwoo::Skill_E(tFSMData& _Data)
{
}

void ER_ActionScript_Hyunwoo::Skill_R(tFSMData& _Data)
{
}

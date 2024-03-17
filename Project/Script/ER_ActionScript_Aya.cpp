#include "pch.h"
#include "ER_ActionScript_Aya.h"

ER_ActionScript_Aya::ER_ActionScript_Aya()
    : ER_ActionScript_Character(SCRIPT_TYPE::ER_ACTIONSCRIPT_AYA)
{
}

ER_ActionScript_Aya::~ER_ActionScript_Aya()
{
}

FSMState* ER_ActionScript_Aya::CreateWait()
{
    FSMState* state = new FSMState(this);

    return state;
}

FSMState* ER_ActionScript_Aya::CreateMove()
{
    FSMState* state = new FSMState(this);

    return state;
}

FSMState* ER_ActionScript_Aya::CreateCraft()
{
    FSMState* state = new FSMState(this);

    return state;
}

FSMState* ER_ActionScript_Aya::CreateRest()
{
    FSMState* state = new FSMState(this);

    return state;
}

FSMState* ER_ActionScript_Aya::CreateAttack()
{
    FSMState* state = new FSMState(this);

    return state;
}

FSMState* ER_ActionScript_Aya::CreateArrive()
{
    FSMState* state = new FSMState(this);

    return state;
}

FSMState* ER_ActionScript_Aya::CreateDeath()
{
    FSMState* state = new FSMState(this);

    return state;
}

FSMState* ER_ActionScript_Aya::CreateSkill_Q()
{
    FSMState* state = new FSMState(this);

    return state;
}

FSMState* ER_ActionScript_Aya::CreateSkill_W()
{
    FSMState* state = new FSMState(this);

    return state;
}

FSMState* ER_ActionScript_Aya::CreateSkill_E()
{
    FSMState* state = new FSMState(this);

    return state;
}

FSMState* ER_ActionScript_Aya::CreateSkill_R()
{
    FSMState* state = new FSMState(this);

    return state;
}

void ER_ActionScript_Aya::Attack(tFSMData& _Data)
{
}

void ER_ActionScript_Aya::Wait(tFSMData& _Data)
{
}

void ER_ActionScript_Aya::Move(tFSMData& _Data)
{
}

void ER_ActionScript_Aya::Craft(tFSMData& _Data)
{
}

void ER_ActionScript_Aya::Rest(tFSMData& _Data)
{
}

void ER_ActionScript_Aya::Skill_Q(tFSMData& _Data)
{
}

void ER_ActionScript_Aya::Skill_W(tFSMData& _Data)
{
}

void ER_ActionScript_Aya::Skill_E(tFSMData& _Data)
{
}

void ER_ActionScript_Aya::Skill_R(tFSMData& _Data)
{
}

#include "pch.h"
#include "ER_ActionScript_Jackie.h"

ER_ActionScript_Jackie::ER_ActionScript_Jackie()
    : ER_ActionScript_Character(SCRIPT_TYPE::ER_ACTIONSCRIPT_JACKIE)
{
}

ER_ActionScript_Jackie::~ER_ActionScript_Jackie()
{
}

FSMState* ER_ActionScript_Jackie::CreateWait()
{
    FSMState* state = new FSMState(this);

    return state;
}

FSMState* ER_ActionScript_Jackie::CreateMove()
{
    FSMState* state = new FSMState(this);

    return state;
}

FSMState* ER_ActionScript_Jackie::CreateCraft()
{
    FSMState* state = new FSMState(this);

    return state;
}

FSMState* ER_ActionScript_Jackie::CreateRest()
{
    FSMState* state = new FSMState(this);

    return state;
}

FSMState* ER_ActionScript_Jackie::CreateAttack()
{
    FSMState* state = new FSMState(this);

    return state;
}

FSMState* ER_ActionScript_Jackie::CreateArrive()
{
    FSMState* state = new FSMState(this);

    return state;
}

FSMState* ER_ActionScript_Jackie::CreateDeath()
{
    FSMState* state = new FSMState(this);

    return state;
}

FSMState* ER_ActionScript_Jackie::CreateSkill_Q()
{
    FSMState* state = new FSMState(this);

    return state;
}

FSMState* ER_ActionScript_Jackie::CreateSkill_W()
{
    FSMState* state = new FSMState(this);

    return state;
}

FSMState* ER_ActionScript_Jackie::CreateSkill_E()
{
    FSMState* state = new FSMState(this);

    return state;
}

FSMState* ER_ActionScript_Jackie::CreateSkill_R()
{
    FSMState* state = new FSMState(this);

    return state;
}

void ER_ActionScript_Jackie::Attack(tFSMData& _Data)
{
}

void ER_ActionScript_Jackie::Wait(tFSMData& _Data)
{
}

void ER_ActionScript_Jackie::Move(tFSMData& _Data)
{
}

void ER_ActionScript_Jackie::Craft(tFSMData& _Data)
{
}

void ER_ActionScript_Jackie::Rest(tFSMData& _Data)
{
}

void ER_ActionScript_Jackie::Skill_Q(tFSMData& _Data)
{
}

void ER_ActionScript_Jackie::Skill_W(tFSMData& _Data)
{
}

void ER_ActionScript_Jackie::Skill_E(tFSMData& _Data)
{
}

void ER_ActionScript_Jackie::Skill_R(tFSMData& _Data)
{
}

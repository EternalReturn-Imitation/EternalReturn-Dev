#include "pch.h"
#include "ER_ActionScript_Yuki.h"

ER_ActionScript_Yuki::ER_ActionScript_Yuki()
    : ER_ActionScript_Character(SCRIPT_TYPE::ER_ACTIONSCRIPT_YUKI)
{
}

ER_ActionScript_Yuki::~ER_ActionScript_Yuki()
{
}

FSMState* ER_ActionScript_Yuki::CreateWait()
{
    FSMState* state = new FSMState(this);

    return state;
}

FSMState* ER_ActionScript_Yuki::CreateMove()
{
    FSMState* state = new FSMState(this);

    return state;
}

FSMState* ER_ActionScript_Yuki::CreateCraft()
{
    FSMState* state = new FSMState(this);

    return state;
}

FSMState* ER_ActionScript_Yuki::CreateRest()
{
    FSMState* state = new FSMState(this);

    return state;
}

FSMState* ER_ActionScript_Yuki::CreateAttack()
{
    FSMState* state = new FSMState(this);

    return state;
}

FSMState* ER_ActionScript_Yuki::CreateArrive()
{
    FSMState* state = new FSMState(this);

    return state;
}

FSMState* ER_ActionScript_Yuki::CreateDeath()
{
    FSMState* state = new FSMState(this);

    return state;
}

FSMState* ER_ActionScript_Yuki::CreateSkill_Q()
{
    FSMState* state = new FSMState(this);

    return state;
}

FSMState* ER_ActionScript_Yuki::CreateSkill_W()
{
    FSMState* state = new FSMState(this);

    return state;
}

FSMState* ER_ActionScript_Yuki::CreateSkill_E()
{
    FSMState* state = new FSMState(this);

    return state;
}

FSMState* ER_ActionScript_Yuki::CreateSkill_R()
{
    FSMState* state = new FSMState(this);

    return state;
}

void ER_ActionScript_Yuki::Attack(tFSMData& _Data)
{
}

void ER_ActionScript_Yuki::Wait(tFSMData& _Data)
{
}

void ER_ActionScript_Yuki::Move(tFSMData& _Data)
{
}

void ER_ActionScript_Yuki::Craft(tFSMData& _Data)
{
}

void ER_ActionScript_Yuki::Rest(tFSMData& _Data)
{
}

void ER_ActionScript_Yuki::Skill_Q(tFSMData& _Data)
{
}

void ER_ActionScript_Yuki::Skill_W(tFSMData& _Data)
{
}

void ER_ActionScript_Yuki::Skill_E(tFSMData& _Data)
{
}

void ER_ActionScript_Yuki::Skill_R(tFSMData& _Data)
{
}

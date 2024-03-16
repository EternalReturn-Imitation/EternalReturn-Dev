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

void ER_ActionScript_Yuki::Attack(CGameObject* _EnemyObj)
{
}

void ER_ActionScript_Yuki::Wait()
{
}

void ER_ActionScript_Yuki::Move(CGameObject* _Target, Vec3 _DestPos)
{
}

void ER_ActionScript_Yuki::Skill_Q(CGameObject* _Target, Vec3 _DestPos)
{
}

void ER_ActionScript_Yuki::Skill_W(CGameObject* _Target, Vec3 _DestPos)
{
}

void ER_ActionScript_Yuki::Skill_E(CGameObject* _Target, Vec3 _DestPos)
{
}

void ER_ActionScript_Yuki::Skill_R(CGameObject* _Target, Vec3 _DestPos)
{
}

void ER_ActionScript_Yuki::Craft()
{
}

void ER_ActionScript_Yuki::Rest()
{
}
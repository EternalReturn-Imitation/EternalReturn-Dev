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

void ER_ActionScript_Jackie::Attack(CGameObject* _EnemyObj)
{
}

void ER_ActionScript_Jackie::Wait()
{
}

void ER_ActionScript_Jackie::Move(CGameObject* _Target, Vec3 _DestPos)
{
}

void ER_ActionScript_Jackie::Skill_Q(CGameObject* _Target, Vec3 _DestPos)
{
}

void ER_ActionScript_Jackie::Skill_W(CGameObject* _Target, Vec3 _DestPos)
{
}

void ER_ActionScript_Jackie::Skill_E(CGameObject* _Target, Vec3 _DestPos)
{
}

void ER_ActionScript_Jackie::Skill_R(CGameObject* _Target, Vec3 _DestPos)
{
}

void ER_ActionScript_Jackie::Craft()
{
}

void ER_ActionScript_Jackie::Rest()
{
}
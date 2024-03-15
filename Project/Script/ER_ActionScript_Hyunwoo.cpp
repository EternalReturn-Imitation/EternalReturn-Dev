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

void ER_ActionScript_Hyunwoo::Attack(CGameObject* _EnemyObj)
{
}

void ER_ActionScript_Hyunwoo::Wait()
{
}

void ER_ActionScript_Hyunwoo::Move(CGameObject* _Target, Vec3 _DestPos)
{
}

void ER_ActionScript_Hyunwoo::Skill_Q(CGameObject* _Target, Vec3 _DestPos)
{
}

void ER_ActionScript_Hyunwoo::Skill_W(CGameObject* _Target, Vec3 _DestPos)
{
}

void ER_ActionScript_Hyunwoo::Skill_E(CGameObject* _Target, Vec3 _DestPos)
{
}

void ER_ActionScript_Hyunwoo::Skill_R(CGameObject* _Target, Vec3 _DestPos)
{
}

void ER_ActionScript_Hyunwoo::Craft()
{
}

void ER_ActionScript_Hyunwoo::Rest()
{
}
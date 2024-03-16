#pragma once
#include "ER_ActionScript_Character.h"

class ER_ActionScript_Aya :
    public ER_ActionScript_Character
{
protected:
    virtual FSMState* CreateWait();
    virtual FSMState* CreateMove();
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
    virtual void Attack(CGameObject* _EnemyObj);                // 기본공격
    virtual void Wait();                                        // 대기
    virtual void Move(CGameObject* _Target, Vec3 _DestPos);     // 이동
    virtual void Craft();                                       // 제작
    virtual void Rest();                                        // 휴식
    virtual void Skill_Q(CGameObject* _Target, Vec3 _DestPos);  // Q Skill
    virtual void Skill_W(CGameObject* _Target, Vec3 _DestPos);  // W Skill
    virtual void Skill_E(CGameObject* _Target, Vec3 _DestPos);  // E Skill
    virtual void Skill_R(CGameObject* _Target, Vec3 _DestPos);  // R Skill

public:
    ER_ActionScript_Aya();
    ~ER_ActionScript_Aya();

    CLONE(ER_ActionScript_Aya);
};


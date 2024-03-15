#include "pch.h"
#include "ER_ActionScript_Rio.h"
#include "ER_DataScript_Character.h"

ER_ActionScript_Rio::ER_ActionScript_Rio()
    :ER_ActionScript_Character(SCRIPT_TYPE::ER_ACTIONSCRIPT_RIO)
{
}

ER_ActionScript_Rio::~ER_ActionScript_Rio()
{
}

FSMState* ER_ActionScript_Rio::CreateWait()
{
    FSMState* state = new FSMState(this);

    state->SetStateEnter((SCRIPT_DELEGATE)&ER_ActionScript_Rio::WaitEnter);

    return state;
}

FSMState* ER_ActionScript_Rio::CreateMove()
{
    FSMState* state = new FSMState(this);
    
    state->SetStateEnter((SCRIPT_DELEGATE)&ER_ActionScript_Rio::MoveEnter);
    state->SetStateUpdate((SCRIPT_DELEGATE)&ER_ActionScript_Rio::MoveUpdate);

    return state;
}

FSMState* ER_ActionScript_Rio::CreateCraft()
{
    FSMState* state = new FSMState(this);

    return state;
}

FSMState* ER_ActionScript_Rio::CreateRest()
{
    FSMState* state = new FSMState(this);

    return state;
}

FSMState* ER_ActionScript_Rio::CreateAttack()
{
    FSMState* state = new FSMState(this);

    return state;
}

FSMState* ER_ActionScript_Rio::CreateArrive()
{
    FSMState* state = new FSMState(this);

    return state;
}

FSMState* ER_ActionScript_Rio::CreateDeath()
{
    FSMState* state = new FSMState(this);

    return state;
}

FSMState* ER_ActionScript_Rio::CreateSkill_Q()
{
    FSMState* state = new FSMState(this);

    return state;
}

FSMState* ER_ActionScript_Rio::CreateSkill_W()
{
    FSMState* state = new FSMState(this);

    return state;
}

FSMState* ER_ActionScript_Rio::CreateSkill_E()
{
    FSMState* state = new FSMState(this);

    return state;
}

FSMState* ER_ActionScript_Rio::CreateSkill_R()
{
    FSMState* state = new FSMState(this);

    return state;
}

void ER_ActionScript_Rio::Attack(CGameObject* _EnemyObj)
{
}

void ER_ActionScript_Rio::Wait()
{
}

void ER_ActionScript_Rio::Move(CGameObject* _Target, Vec3 _DestPos)
{
    ER_ActionScript_Character::Move(nullptr, _DestPos);
}

void ER_ActionScript_Rio::Skill_Q(CGameObject* _Target, Vec3 _DestPos)
{
}

void ER_ActionScript_Rio::Skill_W(CGameObject* _Target, Vec3 _DestPos)
{
}

void ER_ActionScript_Rio::Skill_E(CGameObject* _Target, Vec3 _DestPos)
{
}

void ER_ActionScript_Rio::Skill_R(CGameObject* _Target, Vec3 _DestPos)
{
}

void ER_ActionScript_Rio::Craft()
{
}

void ER_ActionScript_Rio::Rest()
{
}

void ER_ActionScript_Rio::MoveEnter()
{
    GetOwner()->Animator3D()->SelectAnimation(L"Rio_Run");
    SetAbleToCancle(bAbleChange::COMMON);
}

void ER_ActionScript_Rio::MoveUpdate()
{
    ER_DataScript_Character* CharacterData = GetOwner()->GetScript<ER_DataScript_Character>();

    // 캐릭터 속도 얻어와서 넣어주기
    // float speed = GetSpeed;

    // 다음 이동지점이 없다면 대기상태로 전환
    if (!GetOwner()->FindPath()->PathMove(5.f))
        ChangeState(ER_CHAR_ACT::WAIT);
}

void ER_ActionScript_Rio::WaitEnter()
{
    GetOwner()->Animator3D()->SelectAnimation(L"Rio_Wait");
    SetAbleToCancle(bAbleChange::COMMON);
}
#include "pch.h"
#include "ER_ActionScript_Aya.h"
#include "ER_DataScript_Character.h"

#include <Engine/CAnim3D.h>

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

    STATEDELEGATE_ENTER(state, ER_ActionScript_Aya, Wait);

    return state;
}

FSMState* ER_ActionScript_Aya::CreateMove()
{
    FSMState* state = new FSMState(this);

    STATEDELEGATE_ENTER(state, ER_ActionScript_Aya, Move);
    STATEDELEGATE_UPDATE(state, ER_ActionScript_Aya, Move);

    return state;
}

FSMState* ER_ActionScript_Aya::CreateCraft()
{
    FSMState* state = new FSMState(this);

    STATEDELEGATE_ENTER(state, ER_ActionScript_Aya, Craft);
    STATEDELEGATE_UPDATE(state, ER_ActionScript_Aya, Craft);

    return state;
}

FSMState* ER_ActionScript_Aya::CreateRest()
{
    FSMState* state = new FSMState(this);

    STATEDELEGATE_ENTER(state, ER_ActionScript_Aya, Rest);
    STATEDELEGATE_UPDATE(state, ER_ActionScript_Aya, Rest);

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

    STATEDELEGATE_ENTER(state, ER_ActionScript_Aya, Arrive);
    STATEDELEGATE_UPDATE(state, ER_ActionScript_Aya, Arrive);

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

    STATEDELEGATE_ENTER(state, ER_ActionScript_Aya, Skill_Q);
    STATEDELEGATE_UPDATE(state, ER_ActionScript_Aya, Skill_Q);

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

    STATEDELEGATE_UPDATE(state, ER_ActionScript_Aya, Skill_R);

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
    STATEDATA_SET(MOVE, _Data);

    ER_ActionScript_Character::Move(_Data);
}

void ER_ActionScript_Aya::Craft(tFSMData& _Data)
{
    ChangeState(ER_CHAR_ACT::CRAFT);
}

void ER_ActionScript_Aya::Rest(tFSMData& _Data)
{
    ChangeState(ER_CHAR_ACT::REST);
}

void ER_ActionScript_Aya::Skill_Q(tFSMData& _Data)
{
    ChangeState(ER_CHAR_ACT::SKILL_Q);
}

void ER_ActionScript_Aya::Skill_W(tFSMData& _Data)
{
}

void ER_ActionScript_Aya::Skill_E(tFSMData& _Data)
{
}

void ER_ActionScript_Aya::Skill_R(tFSMData& _Data)
{
    ChangeState(ER_CHAR_ACT::SKILL_R);
}

void ER_ActionScript_Aya::begin()
{
    ER_ActionScript_Character::begin();
    ChangeState(ER_CHAR_ACT::ARRIVE);
}

void ER_ActionScript_Aya::MoveEnter(tFSMData& param)
{
    GetOwner()->Animator3D()->SelectAnimation(L"Aya_Run");
    SetAbleToCancle(bAbleChange::COMMON);

    Vec3 DestPos = param.v4Data;

    CFindPath* findpathcomp = GetOwner()->FindPath();
    bool bMove = findpathcomp->FindPath(DestPos);

    if (!bMove)
        ChangeState(ER_CHAR_ACT::WAIT);
}

void ER_ActionScript_Aya::MoveUpdate(tFSMData& param)
{// 캐릭터 속도 얻어와서 넣어주기
    float speed = m_Data->GetStatus().fMovementSpeed;

    // 다음 이동지점이 없다면 대기상태로 전환
    if (!GetOwner()->FindPath()->PathMove(speed))
        ChangeState(ER_CHAR_ACT::WAIT);
}

void ER_ActionScript_Aya::WaitEnter(tFSMData& param)
{
    GetOwner()->Animator3D()->SelectAnimation(L"Aya_Idle");
    SetAbleToCancle(bAbleChange::COMMON);
}

void ER_ActionScript_Aya::ArriveEnter(tFSMData& param)
{
    GetOwner()->Animator3D()->SelectAnimation(L"Aya_Arrive", false);
}

void ER_ActionScript_Aya::ArriveUpdate(tFSMData& param)
{
    if (GetOwner()->Animator3D()->IsFinish())
        ChangeState(ER_CHAR_ACT::WAIT);
}

void ER_ActionScript_Aya::CraftEnter(tFSMData& param)
{
    GetOwner()->Animator3D()->SelectAnimation(L"Aya_Craft", false);
}

void ER_ActionScript_Aya::CraftUpdate(tFSMData& param)
{
}

void ER_ActionScript_Aya::RestEnter(tFSMData& param)
{
    GetOwner()->Animator3D()->SelectAnimation(L"Aya_Rest_Start", false);

    param.iData = 0;
}

void ER_ActionScript_Aya::RestUpdate(tFSMData& param)
{
    CAnimator3D* animator = GetOwner()->Animator3D();

    switch (param.iData)
    {
    case 0: // 시작 동작
    {
        // 시전 캔슬가능
        // 애니메이션 길이만큼 시전게이지 UI 출력

        if (animator->IsFinish())
        {
            animator->SelectAnimation(L"Aya_Rest_Loop");

            // 상태변경불가
            SetAbleToCancle(bAbleChange::DISABLE);
            param.iData++;
        }
        break;
    }
    case 1: // 시전 중
    {
        // 캔슬 불가
        if (KEY_TAP(KEY::RBTN) || KEY_TAP(KEY::X))
        {
            animator->SelectAnimation(L"Aya_Rest_End", false);
            param.iData++;
        }
        break;
    }
    case 2: // 종료 동작
    {
        // 캔슬 가능
        // 애니메이션 길이만큼 시전게이지 UI 출력
        if (animator->IsFinish())
        {
            SetAbleToCancle(bAbleChange::COMMON);
            ChangeState(ER_CHAR_ACT::WAIT);
            param.iData = 0;
        }
        break;
    }
    }
}

void ER_ActionScript_Aya::Skill_QEnter(tFSMData& param)
{
    GetOwner()->Animator3D()->SelectAnimation(L"Aya_SkillQ", false);
    SetAbleToCancle(bAbleChange::DISABLE);
}

void ER_ActionScript_Aya::Skill_QUpdate(tFSMData& param)
{
    CAnimator3D* animator = GetOwner()->Animator3D();

    int curFrame = animator->GetCurFrame();

    if (animator->IsFinish())
    {
        SetAbleToCancle(bAbleChange::COMMON);
        ChangeState(ER_CHAR_ACT::WAIT);
        param.iData = 0;
    }

    if (curFrame > 16) {
        SetAbleToCancle(bAbleChange::COMMON);
        param.iData = 0;
    }
}

void ER_ActionScript_Aya::Skill_RUpdate(tFSMData& param)
{
    CAnimator3D* animator = GetOwner()->Animator3D();
    switch (param.iData)
    {
    //시전 중 캐슬불가
    case 0:
    {
        animator->SelectAnimation(L"Aya_SkillR_Start", false);
        SetAbleToCancle(bAbleChange::DISABLE);
        ++param.iData;
    }
        break;
    //후딜 캐슬 가능
    case 1:
    {
        if (animator->IsFinish())
        {
            animator->SelectAnimation(L"Aya_SkillR_End", false);
            SetAbleToCancle(bAbleChange::COMMON);
            ++param.iData;
        }
    }
        break;
    case 2:
    {
        if (animator->IsFinish())
        {
            SetAbleToCancle(bAbleChange::COMMON);
            ChangeState(ER_CHAR_ACT::WAIT);
            param.iData=0;
        }
    }
        break;
    default:
        break;
    }
}
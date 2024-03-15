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

    // state->SetStateEnter((SCRIPT_DELEGATE)&ER_ActionScript_Rio::WaitEnter);
    STATEDELEGATE_ENTER(state, ER_ActionScript_Rio, Wait);

    return state;
}

FSMState* ER_ActionScript_Rio::CreateMove()
{
    FSMState* state = new FSMState(this);
    
    // state->SetStateEnter((SCRIPT_DELEGATE)&ER_ActionScript_Rio::MoveEnter);
    // state->SetStateUpdate((SCRIPT_DELEGATE)&ER_ActionScript_Rio::MoveUpdate);

    Vector3 vector;
    vector.x = 1.0f;
    vector.y = 2.0f;
    vector.z = 3.0f;

    Vector3* pVector = &vector;
    DWORD_PTR address = reinterpret_cast<DWORD_PTR>(pVector);

    STATEDELEGATE_ENTER(state, ER_ActionScript_Rio, Move);
    STATEDELEGATE_UPDATE(state, ER_ActionScript_Rio, Move);

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

    STATEDELEGATE_ENTER(state, ER_ActionScript_Rio, Rest);
    STATEDELEGATE_UPDATE(state, ER_ActionScript_Rio, Rest);

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
    
    STATEDELEGATE_ENTER(state, ER_ActionScript_Rio, Arrive);
    STATEDELEGATE_UPDATE(state, ER_ActionScript_Rio, Arrive);

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

void ER_ActionScript_Rio::begin()
{
    ER_ActionScript_Character::begin();
    ChangeState(ER_CHAR_ACT::ARRIVE);
}

void ER_ActionScript_Rio::Attack(CGameObject* _EnemyObj)
{
}

void ER_ActionScript_Rio::Wait()
{
}

void ER_ActionScript_Rio::Move(CGameObject* _Target, Vec3 _DestPos)
{
    tFSMData tmp;
    tmp.v4Data = Vec4(_DestPos.x, _DestPos.y, _DestPos.z, 0.f);
    StateList[ER_CHAR_ACT::MOVE]->SetData(tmp);

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
    ChangeState(ER_CHAR_ACT::REST);
}

void ER_ActionScript_Rio::MoveEnter(tFSMData param)
{
    GetOwner()->Animator3D()->SelectAnimation(L"Rio_Run");
    SetAbleToCancle(bAbleChange::COMMON);

    Vec3 DestPos = param.v4Data;

    CFindPath* findpathcomp = GetOwner()->FindPath();
    bool bMove = findpathcomp->FindPath(DestPos);

    if (!bMove)
        ChangeState(ER_CHAR_ACT::WAIT);
}

void ER_ActionScript_Rio::MoveUpdate(tFSMData param)
{
    // 캐릭터 속도 얻어와서 넣어주기
    float speed = m_Data->GetStatus().fMovementSpeed;

    // 다음 이동지점이 없다면 대기상태로 전환
    if (!GetOwner()->FindPath()->PathMove(speed))
        ChangeState(ER_CHAR_ACT::WAIT);
}

void ER_ActionScript_Rio::WaitEnter(tFSMData param)
{
    GetOwner()->Animator3D()->SelectAnimation(L"Rio_Wait");
    SetAbleToCancle(bAbleChange::COMMON);
}

void ER_ActionScript_Rio::ArriveEnter(tFSMData param)
{
    GetOwner()->Animator3D()->SelectAnimation(L"Rio_Arrive", false);
}

void ER_ActionScript_Rio::ArriveUpdate(tFSMData param)
{
    if (GetOwner()->Animator3D()->IsFinish())
        ChangeState(ER_CHAR_ACT::WAIT);
}

void ER_ActionScript_Rio::RestEnter(tFSMData param)
{
    GetOwner()->Animator3D()->SelectAnimation(L"Rio_Rest_Start", false);
    
    // 시전게이지 표기
}

void ER_ActionScript_Rio::RestUpdate(tFSMData param)
{
    static int RestAnimStep = 0;
    
    CAnimator3D* animator = GetOwner()->Animator3D();
    
    switch (RestAnimStep)
    {
    case 0: // 시작 동작
    {
        // 시전 캔슬가능
        // 애니메이션 길이만큼 시전게이지 UI 출력

        if (animator->IsFinish())
        {
            animator->SelectAnimation(L"Rio_Rest_Loop");
            
            // 상태변경불가
            SetAbleToCancle(bAbleChange::DISABLE);
            RestAnimStep++;
        }
        break;
    }
    case 1: // 시전 중
    {
        // 캔슬 불가
        if (KEY_TAP(KEY::RBTN) || KEY_TAP(KEY::X))
        {
            animator->SelectAnimation(L"Rio_Rest_End", false);
            RestAnimStep++;
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
            RestAnimStep = 0;
        }
        break;
    }
    }
}

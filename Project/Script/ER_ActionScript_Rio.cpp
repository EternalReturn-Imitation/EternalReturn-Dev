#include "pch.h"
#include "ER_ActionScript_Rio.h"
#include "ER_DataScript_Character.h"

#include <Engine\CAnim3D.h>

ER_ActionScript_Rio::ER_ActionScript_Rio()
    : ER_ActionScript_Character(SCRIPT_TYPE::ER_ACTIONSCRIPT_RIO)
    , m_BowType(false)
{
}

ER_ActionScript_Rio::~ER_ActionScript_Rio()
{
}

FSMState* ER_ActionScript_Rio::CreateWait()
{
    FSMState* state = new FSMState(this);

    STATEDELEGATE_ENTER(state, Rio, Wait);

    return state;
}

FSMState* ER_ActionScript_Rio::CreateMove()
{
    FSMState* state = new FSMState(this);

    STATEDELEGATE_ENTER(state, Rio, Move);
    STATEDELEGATE_UPDATE(state, Rio, Move);

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

    STATEDELEGATE_ENTER(state, Rio, Rest);
    STATEDELEGATE_UPDATE(state, Rio, Rest);

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
    
    STATEDELEGATE_ENTER(state, Rio, Arrive);
    STATEDELEGATE_UPDATE(state, Rio, Arrive);

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

    STATEDELEGATE_ENTER(state, Rio, Skill_Q);
    STATEDELEGATE_UPDATE(state, Rio, Skill_Q);

    return state;
}

FSMState* ER_ActionScript_Rio::CreateSkill_W()
{
    FSMState* state = new FSMState(this);

    STATEDELEGATE_ENTER(state, Rio, Skill_W);
    STATEDELEGATE_UPDATE(state, Rio, Skill_W);

    return state;
}

FSMState* ER_ActionScript_Rio::CreateSkill_E()
{
    FSMState* state = new FSMState(this);

    STATEDELEGATE_ENTER(state, Rio, Skill_E);
    STATEDELEGATE_UPDATE(state, Rio, Skill_E);

    return state;
}

FSMState* ER_ActionScript_Rio::CreateSkill_R()
{
    FSMState* state = new FSMState(this);

    STATEDELEGATE_ENTER(state, Rio, Skill_R);
    STATEDELEGATE_UPDATE(state, Rio, Skill_R);

    return state;
}

void ER_ActionScript_Rio::Attack(tFSMData& _Data)
{
}

void ER_ActionScript_Rio::Wait(tFSMData& _Data)
{
}

void ER_ActionScript_Rio::Move(tFSMData& _Data)
{
    STATEDATA_SET(MOVE, _Data);

    ER_ActionScript_Character::Move(_Data);
}

void ER_ActionScript_Rio::Craft(tFSMData& _Data)
{
}

void ER_ActionScript_Rio::Rest(tFSMData& _Data)
{
    ChangeState(ER_CHAR_ACT::REST);
}

void ER_ActionScript_Rio::Skill_Q(tFSMData& _Data)
{
    ChangeState(ER_CHAR_ACT::SKILL_Q);
}

void ER_ActionScript_Rio::Skill_W(tFSMData& _Data)
{
    ChangeState(ER_CHAR_ACT::SKILL_W);
}

void ER_ActionScript_Rio::Skill_E(tFSMData& _Data)
{
    STATEDATA_SET(SKILL_E, _Data);

    ChangeState(ER_CHAR_ACT::SKILL_E);
}

void ER_ActionScript_Rio::Skill_R(tFSMData& _Data)
{
    ChangeState(ER_CHAR_ACT::SKILL_R);
}

void ER_ActionScript_Rio::MoveEnter(tFSMData& param)
{
    if (m_BowType)
        GetOwner()->Animator3D()->SelectAnimation(L"Rio_Long_Run", true);
    else
        GetOwner()->Animator3D()->SelectAnimation(L"Rio_Short_Run", true);

    SetAbleToCancle(bAbleChange::COMMON);

    Vec3 DestPos = param.v4Data;

    CFindPath* findpathcomp = GetOwner()->FindPath();
    findpathcomp->FindPath(DestPos);
}

void ER_ActionScript_Rio::MoveUpdate(tFSMData& param)
{
    // 캐릭터 속도 얻어와서 넣어주기
    float speed = m_Data->GetStatus()->fMovementSpeed;

    // 다음 이동지점이 없다면 대기상태로 전환
    if (!GetOwner()->FindPath()->PathMove(speed))
        ChangeState(ER_CHAR_ACT::WAIT);
} 

void ER_ActionScript_Rio::WaitEnter(tFSMData& param)
{
    if (m_BowType)
        GetOwner()->Animator3D()->SelectAnimation(L"Rio_Long_Wait", true);
    else
        GetOwner()->Animator3D()->SelectAnimation(L"Rio_Short_Wait", true);

    SetAbleToCancle(bAbleChange::COMMON);
}

void ER_ActionScript_Rio::ArriveEnter(tFSMData& param)
{
    GetOwner()->Animator3D()->SelectAnimation(L"Rio_Arrive", false);
}

void ER_ActionScript_Rio::ArriveUpdate(tFSMData& param)
{
    if (GetOwner()->Animator3D()->IsFinish())
        ChangeState(ER_CHAR_ACT::WAIT);
}

void ER_ActionScript_Rio::RestEnter(tFSMData& param)
{
    GetOwner()->Animator3D()->SelectAnimation(L"Rio_Rest_Start", false);
    
    param.iData = 0;
}

void ER_ActionScript_Rio::RestUpdate(tFSMData& param)
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
            animator->SelectAnimation(L"Rio_Rest_Loop");
            
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
            animator->SelectAnimation(L"Rio_Rest_End", false);
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

void ER_ActionScript_Rio::Skill_QEnter(tFSMData& param)
{
    // Bow 타입 전환
    m_BowType = !m_BowType;

    if(m_BowType)
        GetOwner()->Animator3D()->SelectAnimation(L"Rio_Long_Skill_Q", false);
    else
        GetOwner()->Animator3D()->SelectAnimation(L"Rio_Short_Skill_Q", false);

    SetAbleToCancle(bAbleChange::ABSOUTE);
}

void ER_ActionScript_Rio::Skill_QUpdate(tFSMData& param)
{
    if (ER_CHAR_ACT::MOVE == m_iPrevState)
    {
        if (7 <= GetOwner()->Animator3D()->GetCurFrame())
        {
            SetAbleToCancle(bAbleChange::COMMON);
            ChangeState(ER_CHAR_ACT::MOVE);
        }
    }
    else if (GetOwner()->Animator3D()->IsFinish())
    {
        SetAbleToCancle(bAbleChange::COMMON);

        ChangeState(ER_CHAR_ACT::WAIT);
    }
}

void ER_ActionScript_Rio::Skill_WEnter(tFSMData& param)
{
    CAnimator3D* Animator = GetOwner()->Animator3D();
    if (m_BowType)
        Animator->SelectAnimation(L"Rio_Long_Skill_W", false);
    else
        Animator->SelectAnimation(L"Rio_Short_Skill_W", false);

    SetAbleToCancle(bAbleChange::ABSOUTE);
}

void ER_ActionScript_Rio::Skill_WUpdate(tFSMData& param)
{
    // 11프레임에 캔슬가능으로 변경 (후딜레이삭제)
    if (11 == GetOwner()->Animator3D()->GetCurFrame())
        SetAbleToCancle(bAbleChange::COMMON);

    // 스킬 애니메이션이 끝나면 대기상태로 자동 전환.
    if (GetOwner()->Animator3D()->IsFinish())
    {
        SetAbleToCancle(bAbleChange::COMMON);
        ChangeState(ER_CHAR_ACT::WAIT);
    }
}

void ER_ActionScript_Rio::Skill_EEnter(tFSMData& param)
{
    // 단궁폼과 장궁폼 같은모션
    CAnimator3D* Animator = GetOwner()->Animator3D();
    Animator->SelectAnimation(L"Rio_Short_Skill_E", false);
    
    // 방향 전환
    SetRotationToTarget(param.v4Data);

    // 방향 저장
    Vec3 vPos = GetOwner()->Transform()->GetRelativePos();
    Vec3 vDir = (param.v4Data - vPos).Normalize();
    
    // param.v4Data 에 받아온 방향정보를 v2Data로 이동
    param.v2Data.x = vDir.x;
    param.v2Data.y = vDir.z;
    
    param.v4Data[0] = 10.f;                                              // 스킬 거리
    float ClearDist = GetClearDistance(vDir, param.v4Data[0]);
    param.v4Data[1] = ClearDist;                                        // 이동 가능 거리
    param.v4Data[2] = (float)Animator->GetCurAnim()->GetAnimClip().dEndTime;   // 전체 애니메이션 재생 시간
    param.v4Data[3] = 0.f;                                              // 이동한 거리 초기화.

    SetAbleToCancle(bAbleChange::ABSOUTE);
}

void ER_ActionScript_Rio::Skill_EUpdate(tFSMData& param)
{
    CTransform* transform = GetOwner()->Transform();
    CAnimator3D* animator = GetOwner()->Animator3D();

    // param.v4Data[0] : 스킬 거리(속도)
    // param.v4Data[1] : 이동 가능 거리
    // param.v4Data[2] : 전체 애니메이션 재생 시간
    // param.v4Data[3] : 이동한 거리

    if (animator->GetCurFrame() < 3)
    {
        if (param.v4Data[3] < param.v4Data[1] * 0.1f)
        {
            Vec3 vPos = transform->GetRelativePos();
            Vec3 vDir(param.v2Data.x, 0.f, param.v2Data.y);

            float speed = param.v4Data[0] * 0.5f;

            float CurFrameMoveDist = speed * param.v4Data[2] * DT;

            param.v4Data[3] += CurFrameMoveDist;

            vPos += vDir * CurFrameMoveDist;

            // 캐릭터 이동
            transform->SetRelativePos(vPos);
        }
        return;
    }
    // 3 점프
    else if (animator->GetCurFrame() < 16)
    {
        // 이동한거리가 이동가능거리를 넘지 않았는지 판단.
        if (param.v4Data[3] < param.v4Data[1])
        {
            Vec3 vPos = transform->GetRelativePos();
            Vec3 vDir(param.v2Data.x, 0.f, param.v2Data.y);

            float speed = param.v4Data[0] * 1.5f;

            float CurFrameMoveDist = speed * param.v4Data[2] * DT;

            param.v4Data[3] += CurFrameMoveDist;

            vPos += vDir * CurFrameMoveDist;

            // 캐릭터 이동
            transform->SetRelativePos(vPos);
        }
        return;
    }
    // 17 착지

    if (GetOwner()->Animator3D()->IsFinish())
    {
        SetAbleToCancle(bAbleChange::COMMON);
        // clear
        param.v4Data = Vec4();
        param.v4Data = Vec2();
     
        ChangeState(ER_CHAR_ACT::WAIT);
    }
}

void ER_ActionScript_Rio::Skill_REnter(tFSMData& param)
{
    if (m_BowType)
    {
        GetOwner()->Animator3D()->SelectAnimation(L"Rio_Long_Skill_R", false);
        param.iData = 0;
    }
    else
    {
        // 첫 시전
        if(0 == param.iData)
            GetOwner()->Animator3D()->SelectAnimation(L"Rio_Short_Skill_R_Start", false);

        // 두번째 시전
        if(1 == param.iData)
            GetOwner()->Animator3D()->SelectAnimation(L"Rio_Short_Skill_R_End", false);
    }

    SetAbleToCancle(bAbleChange::DISABLE);
}

void ER_ActionScript_Rio::Skill_RUpdate(tFSMData& param)
{
    // 장궁 폼
    if (m_BowType)
    {
        if (GetOwner()->Animator3D()->IsFinish())
        {
            SetAbleToCancle(bAbleChange::COMMON);
            ChangeState(ER_CHAR_ACT::WAIT);
        }
    }
    // 단궁 폼
    else
    {
        CAnimator3D* animator = GetOwner()->Animator3D();

        switch (param.iData)
        {
        case 0: // 시작 동작
        {
            // 애니메이션 길이만큼 시전게이지 UI 출력

            // 3연발

            if (animator->IsFinish())
            {
                SetAbleToCancle(bAbleChange::COMMON);
                ChangeState(ER_CHAR_ACT::WAIT);
                param.iData++;
            }
            break;
        }
        case 1: // 종료동작
        {
            // 애니메이션 길이만큼 시전게이지 UI 출력

            // 1발

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
}

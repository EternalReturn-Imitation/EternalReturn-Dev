#include "pch.h"
#include "ER_ActionScript_Jackie.h"
#include "ER_DataScript_Character.h"

#include "ER_DataScript_ItemBox.h"
#include "ER_DataScript_Item.h"

#include <Engine\CAnim3D.h>

ER_ActionScript_Jackie::ER_ActionScript_Jackie()
    : ER_ActionScript_Character(SCRIPT_TYPE::ER_ACTIONSCRIPT_JACKIE)
{
}

ER_ActionScript_Jackie::~ER_ActionScript_Jackie()
{
}

void ER_ActionScript_Jackie::WaitEnter(tFSMData& param)
{
    /*
    bData[0] = 궁극기 동작중 판단
    */
    
    // 궁극기 작동 여부
    param.bData[0] = IsSkillOn(SKILLIDX::R_1);

    if (param.bData[0])
        Animator3D()->SelectAnimation(L"Jackie_R_Wait", true);
    else
        Animator3D()->SelectAnimation(L"Jackie_Wait", true);

    SetStateGrade(eAccessGrade::BASIC);
    param.fData = 0.f;
}
void ER_ActionScript_Jackie::WaitUpdate(tFSMData& param)
{
    /*
    bData[0]    = 궁극기 동작중이었는지 판단
    */
    
    // 0.5초마다 체력회복
    param.fData += DT;

    if (0.5f <= param.fData)
    {
        // HP/SP 자연 회복
        m_Data->HPRegen();
        m_Data->SPRegen();

        // 체력재생 카운트 초기화
        param.fData -= 0.5f;
    }

    // 스킬 지속시간이 종료된경우
    bool IsAction = IsSkillOn(SKILLIDX::R_1);
    if (param.bData[0] && !IsAction)
        Animator3D()->SelectAnimation(L"Jackie_Wait", true);
}
void ER_ActionScript_Jackie::WaitExit(tFSMData& param)
{
}


void ER_ActionScript_Jackie::MoveEnter(tFSMData& param)
{
    /*
    [MOVE]
    bData[0]	: 타겟 추적 여부
    bData[1]    : 이동속도 버프스킬 작용 여부
    fData		: 공격 가능 거리 / 파밍 가능 거리
    iData[0]	: 타겟 타입 : 1 - 공격대상 / 2 - 아이템박스
    v4Data		: 목표 이동 좌표
    */
    
    param.bData[1] = IsSkillOn(SKILLIDX::W_1);

    if(param.bData[1])
        Animator3D()->SelectAnimation(L"Jackie_SkillW", true);
    else
        Animator3D()->SelectAnimation(L"Jackie_Run", true);

    FindPath()->FindPath(param.v4Data);
}
void ER_ActionScript_Jackie::MoveUpdate(tFSMData& param)
{
    /*
    [MOVE]
    bData[0]	: 타겟 추적 여부
    bData[1]    : 이동속도 버프스킬 작용 여부
    fData		: 공격 가능 거리 / 파밍 가능 거리
    iData[0]	: 타겟 타입 : 1 - 공격대상 / 2 - 아이템박스
    v4Data		: 목표 이동 좌표
    */

    // 타겟 추적중
    if (param.bData[0])
    {
        if (IsInRange((CGameObject*)param.lParam, param.fData))
        {
            param.bData[0] = false;     // 추적 종료    
            FindPath()->ClearPath();    // 이동 경로 초기화
            switch (param.iData[0])
            {
            case 1: // 공격 대상
                ChangeState(ER_CHAR_ACT::ATTACK);
                break;
            case 2: // 아이템 박스
                ChangeState(ER_CHAR_ACT::FARMING);
                break;
            }
            return; // 상태전환 후 작업 완료
        }
    }

    // 애니메이션 변경 판단
    bool IsAction = IsSkillOn(SKILLIDX::W_1);
    if (param.bData[1] && !IsAction)
        Animator3D()->SelectAnimation(L"Jackie_Run", true);

    // 버프/디버프 효과 반영
    tStatus_Effect* SpeedEfc = m_Data->GetStatusEffect();

    // 이동속도 설정
    float fMoveSpeed = GetStatus()->fMovementSpeed;
    fMoveSpeed += (fMoveSpeed * SpeedEfc->GetIncSPD()) - (fMoveSpeed * SpeedEfc->GetDecSPD());

    // 다음 이동지점이 없다면 대기상태로 전환
    if (!FindPath()->PathMove(fMoveSpeed))
        ChangeState(ER_CHAR_ACT::WAIT);
}
void ER_ActionScript_Jackie::MoveExit(tFSMData& param)
{
}

void ER_ActionScript_Jackie::FarmingEnter(tFSMData& param)
{
    param.bData[0] = IsSkillOn(SKILLIDX::R_1);

    if (param.bData[0])
        Animator3D()->SelectAnimation(L"Jackie_R_Wait", true);
    else
        Animator3D()->SelectAnimation(L"Jackie_Wait", true);

    SetStateGrade(eAccessGrade::BASIC);

    CGameObject* ItemObj = ((CGameObject*)param.lParam);

    ER_DataScript_ItemBox* ItemBox = ItemObj->GetScript<ER_DataScript_ItemBox>();
    ER_UIMgr::GetInst()->OpenItemBoxUI(ItemBox);

}

void ER_ActionScript_Jackie::FarmingExit(tFSMData& param)
{
    ER_UIMgr::GetInst()->CloseItemBoxUI();
}

void ER_ActionScript_Jackie::RestEnter(tFSMData& param)
{
    /*
    iData[0] = 휴식 애니메이션 재생판단
    fData    = 체력재생시간 카운트
    */
    param.iData[0] = 0;
    param.fData = 0.f;
    Animator3D()->SelectAnimation(L"Jackie_Rest_Start", false);
}
void ER_ActionScript_Jackie::RestUpdate(tFSMData& param)
{
    switch (param.iData[0])
    {
    case 0: // 시작 동작
    {
        // 애니메이션 길이만큼 시전게이지 UI 출력

        if (Animator3D()->IsFinish())
        {
            Animator3D()->SelectAnimation(L"Jackie_Rest_Loop");

            // 상태변경불가
            SetStateGrade(eAccessGrade::UTMOST);
            param.iData[0]++;
        }
        break;
    }
    case 1: // 시전 중
    {
        // 캔슬 불가
        // 0.5초마다 회복
        param.fData += DT;

        if (0.5f <= param.fData)
        {
            // HP/SP 자연 회복 5배 빠르게 회복
            m_Data->HPRegen(5.f);
            m_Data->SPRegen(5.f);

            // 자원재생 카운트 초기화
            param.fData -= 0.5f;
        }

        if (KEY_TAP(KEY::RBTN) || KEY_TAP(KEY::X))
        {
            Animator3D()->SelectAnimation(L"Jackie_Rest_End", false);
            param.iData[0]++;
        }
        break;
    }
    case 2: // 종료 동작
    {
        // 애니메이션 길이만큼 시전게이지 UI 출력
        if (Animator3D()->IsFinish())
        {
            SetStateGrade(eAccessGrade::BASIC);
            ChangeState(ER_CHAR_ACT::WAIT);
        }
        break;
    }
    }
}
void ER_ActionScript_Jackie::RestExit(tFSMData& param)
{
}

void ER_ActionScript_Jackie::ArriveEnter(tFSMData& param)
{
    Animator3D()->SelectAnimation(L"Jackie_Arrive", false);
}
void ER_ActionScript_Jackie::ArriveUpdate(tFSMData& param)
{
    if (Animator3D()->IsFinish())
        ChangeState(ER_CHAR_ACT::WAIT);
}
void ER_ActionScript_Jackie::ArriveExit(tFSMData& param)
{
}

void ER_ActionScript_Jackie::DeadEnter(tFSMData& param)
{
    Animator3D()->SelectAnimation(L"Jackie_Death", false);
}
void ER_ActionScript_Jackie::DeadUpdate(tFSMData& param)
{
}
void ER_ActionScript_Jackie::DeadExit(tFSMData& param)
{
}


void ER_ActionScript_Jackie::AttackEnter(tFSMData& param)
{
    /*
    [ATTACK]
    bData[0]	    : 공격동작 진행중인지 여부
    bData[1]	    : Battle Event 실행 여부
    bData[2]	    : 다음 타겟 지정 여부
    bData[3]        : 공격모션 변경
    iData[0]	    : 타격지점 애니메이션 프레임 = Hit Frame
    lParam		    : 타겟 오브젝트
    RParam		    : 타겟 예정 오브젝트
    */

    // 공격 시작단계 초기화
    param.bData[0] = true;
    param.bData[1] = false;
    param.bData[3] = !param.bData[3];

    if (IsSkillOn(SKILLIDX::R_1))
    {
        if (param.bData[3])
        {
            GetOwner()->Animator3D()->SelectAnimation(L"Jackie_R_Attack0", false);
            param.iData[0] = 6;
        }
        else
        {
            GetOwner()->Animator3D()->SelectAnimation(L"Jackie_R_Attack1", false);
            param.iData[0] = 6;
        }
    }
    else
    {
        if (param.bData[3])
        {
            GetOwner()->Animator3D()->SelectAnimation(L"Jackie_Attack0", false);
            param.iData[0] = 6;
        }
        else
        {
            GetOwner()->Animator3D()->SelectAnimation(L"Jackie_Attack1", false);
            param.iData[0] = 6;
        }
    }

    // 타겟방향으로 회전
    SetRotationToTarget(((CGameObject*)param.lParam)->Transform()->GetRelativePos());
    SetStateGrade(eAccessGrade::UTMOST);
}
void ER_ActionScript_Jackie::AttackUpdate(tFSMData& param)
{
    /*
     [ATTACK]
     bData[0]	: 공격동작 진행중인지 여부
     bData[1]	: Battle Event 실행 여부
     bData[2]	: 다음 타겟 지정 여부
     bData[3]    : 공격모션 변경
     iData[0]	: 타격지점 애니메이션 프레임 = Hit Frame
     lParam		: 타겟 오브젝트
     RParam		: 타겟 예정 오브젝트
     */

    float Atkspd = GetStatus()->fAttackSpeed;

    // 버프/디버프 확인
    tStatus_Effect* statusefc = GetStatusEffect();
    Atkspd += (Atkspd * statusefc->GetIncAPD()) - (Atkspd * statusefc->GetDecAPD());

    // 애니메이션 속도 증가
    Animator3D()->PlaySpeedValue(Atkspd);

    if (!param.bData[1] && param.iData[0] < Animator3D()->GetCurFrame())
    {
        // 사운드 재생

        // 캐릭터 고유 공격 알고리즘

        if (IsSkillOn(SKILLIDX::R_1))
        {
            // 강화평타로 동시적용
            // 스킬 데미지 
            // 평타 데미지
        }
        else
            BATTLE_COMMON(GetOwner(), param.lParam);

        param.bData[1] = true;
        SetStateGrade(eAccessGrade::BASIC);
    }


    if (Animator3D()->IsFinish())
    {
        param.bData[0] = false;         // 공격 동작 완료

        // 공격중 타겟이 변경되었다
        if (param.bData[2])
        {
            param.lParam = param.RParam;
            param.bData[2] = false;
            param.RParam = 0;
        }

        CGameObject* Target = (CGameObject*)param.lParam;

        // 사망 판단
        bool IsDead = Target->GetScript<ER_DataScript_Character>()->IsDeadState();

        if (IsDead)
            ChangeState(ER_CHAR_ACT::WAIT);
        else
        {
            // 거리 판단
            float AtkRange = GetStatus()->fAtkRange;

            if (IsInRange(Target, AtkRange))
                AttackEnter(param);
            else
                Attack(param);
        }
    }
}
void ER_ActionScript_Jackie::AttackExit(tFSMData& param)
{
    param.bData[0] = false;
    SetStateGrade(eAccessGrade::BASIC);
}

void ER_ActionScript_Jackie::CraftEnter(tFSMData& param)
{
    Animator3D()->SelectAnimation(L"Jackie_Craft", false);
    ERCHARSOUND(CRAFT_SOUND);
}

void ER_ActionScript_Jackie::CraftUpdate(tFSMData& param)
{
    if (Animator3D()->IsFinish())
    {
        STOPSOUND(CRAFT_SOUND);
        // 아이탬 생성함수
        GetOwner()->GetScript<ER_DataScript_Character>()->CraftItem(param.iData[0]);

        param.bData[0] = false;
        ChangeState(ER_CHAR_ACT::WAIT);
    }
}

void ER_ActionScript_Jackie::CraftExit(tFSMData& param)
{
}

void ER_ActionScript_Jackie::Skill_Q(tFSMData& _Data)
{
    STATEDATA_SET(SKILL_Q, _Data);

    ChangeState(ER_CHAR_ACT::SKILL_Q);
}

void ER_ActionScript_Jackie::Skill_W(tFSMData& _Data)
{
    STATEDATA_SET(SKILL_W, _Data);

    ChangeState(ER_CHAR_ACT::SKILL_W);
}

void ER_ActionScript_Jackie::Skill_E(tFSMData& _Data)
{
    // 시전중이 아니라면
    if (!STATEDATA_GET(SKILL_E).bData[0])
        STATEDATA_SET(SKILL_E, _Data);

    ChangeState(ER_CHAR_ACT::SKILL_E);
}

void ER_ActionScript_Jackie::Skill_R(tFSMData& _Data)
{
    ChangeState(ER_CHAR_ACT::SKILL_R);
}

void ER_ActionScript_Jackie::Skill_QEnter(tFSMData& param)
{
    tSkill_Info* Skill = m_Data->GetSkill((UINT)SKILLIDX::Q_1);

    GetOwner()->Animator3D()->SelectAnimation(L"Jackie_Wait", true);
    param.iData[0] = 1; // 0. 기본, 1. 스킬 조준, 2. 스킬 공격
}

void ER_ActionScript_Jackie::Skill_QUpdate(tFSMData& param)
{
    switch (param.iData[0])
    {
    case 1:
    {
        if (KEY_TAP(KEY::LBTN) || KEY_TAP(KEY::Q))
        {
            // 스킬 조준
            Vec3 vTargetPoint = GetFocusPoint();
            SetRotationToTarget(vTargetPoint);

            // 스킬 발동
            GetOwner()->Animator3D()->SelectAnimation(L"Jackie_SkillQ", false);
            SetStateGrade(eAccessGrade::UTMOST);
            param.iData[0]++;
        }
        break;
    }
    case 2:
    {

        // 특정 프레임에 공격 모션과 함께 전투 매니저 호출

        if (GetOwner()->Animator3D()->IsFinish())
            ChangeState(ER_CHAR_ACT::WAIT, eAccessGrade::UTMOST);

        break;
    }
    default:
        break;
    }
}

void ER_ActionScript_Jackie::Skill_QExit(tFSMData& param)
{
    param.iData[0] = 0;
}

void ER_ActionScript_Jackie::Skill_WEnter(tFSMData& param)
{
    // 스킬사용
    tSkill_Info* Skill = m_Data->GetSkill((UINT)SKILLIDX::W_1);
    if (Skill->Use(&GetStatus()->iSP, true))
    {
        float SpdValue = Skill->Float1();
        float Time = Skill->fActionTime;
        m_Data->GetStatusEffect()->ActiveEffect((UINT)eStatus_Effect::INCREASE_SPD, Time, SpdValue);
    }
}

void ER_ActionScript_Jackie::Skill_WUpdate(tFSMData& param)
{
    SetStateGrade(eAccessGrade::BASIC);

    if ((UINT)ER_CHAR_ACT::MOVE == m_iPrevState)
        ChangeState(ER_CHAR_ACT::MOVE);
    else
        ChangeState(ER_CHAR_ACT::WAIT);
}

void ER_ActionScript_Jackie::Skill_WExit(tFSMData& param)
{
}

void ER_ActionScript_Jackie::Skill_EEnter(tFSMData& param)
{
    tSkill_Info* Skill = m_Data->GetSkill((UINT)SKILLIDX::E_1);
    
    GetOwner()->Animator3D()->SelectAnimation(L"Jackie_Wait", true);
    param.iData[0] = 1; // 0. 기본, 1. 스킬 조준, 2. 스킬 공격
}

void ER_ActionScript_Jackie::Skill_EUpdate(tFSMData& param)
{
    CTransform* transform = GetOwner()->Transform();
    CAnimator3D* animator = GetOwner()->Animator3D();

    switch (param.iData[0])
    {
    case 1:
    {
        //조준
        Vec3 vTargetPoint = GetFocusPoint();

        // 최대거리 이내로 스킬포인트를 찍은 지점으로 이동.
        if (KEY_TAP(KEY::LBTN) || KEY_TAP(KEY::E))
        {
            animator->SelectAnimation(L"Jackie_SkillE_Start", false);

            // 스킬 조준
            SetRotationToTarget(vTargetPoint);

            Vec3 vPos = transform->GetRelativePos();
            Vec3 vDir = (vTargetPoint - vPos).Normalize();
            float fDist = Vec3::Distance(vPos, vTargetPoint);

            // 방향정보를 v2Data로 이동
            param.v2Data.x = vDir.x;
            param.v2Data.y = vDir.z;

            float fMaxRange = 15.f; // 최대 스킬 거리

            param.v4Data[0] = fMaxRange < fDist ? fMaxRange : fDist;                                                      // 스킬 거리
            float ClearDist = GetClearDistance(vDir, param.v4Data[0]);
            param.v4Data[1] = ClearDist;                                                // 이동 가능 거리
            param.v4Data[2] = (float)animator->GetCurAnim()->GetAnimClip().dEndTime;    // 전체 애니메이션 재생 시간
            param.v4Data[3] = 0.f;

            // 스킬 발동
            param.bData[0] = true;
            param.iData[0]++;
            SetStateGrade(eAccessGrade::UTMOST);
        }
        break;
    }
    case 2:
    {
        // 3frm 이후부터 이동
        if (GetOwner()->Animator3D()->GetCurFrame() < 3)
            return;

        //점프
        if (param.v4Data[3] < param.v4Data[1])
        {

            // param.v4Data[0] : 스킬 거리(속도)
            // param.v4Data[1] : 이동 가능 거리
            // param.v4Data[2] : 전체 애니메이션 재생 시간
            // param.v4Data[3] : 이동한 거리

            Vec3 vPos = transform->GetRelativePos();
            Vec3 vDir(param.v2Data.x, 0.f, param.v2Data.y);

            float speed = param.v4Data[0] * 5.f;

            float CurFrameMoveDist = speed * param.v4Data[2] * DT;

            param.v4Data[3] += CurFrameMoveDist;

            vPos += vDir * CurFrameMoveDist;

            // 캐릭터 이동
            transform->SetRelativePos(vPos);
        }

        if (GetOwner()->Animator3D()->IsFinish())
        {
            animator->SelectAnimation(L"Jackie_SkillE_End", false);
            param.iData[0]++;
        }

        break;
    }
    case 3:
    {
        // 찍기
        // 

        if (GetOwner()->Animator3D()->IsFinish())
            ChangeState(ER_CHAR_ACT::WAIT, eAccessGrade::UTMOST);

        break;
    }

    default:
        break;
    }
}

void ER_ActionScript_Jackie::Skill_EExit(tFSMData& param)
{
    param.iData[0] = 0;
}

void ER_ActionScript_Jackie::Skill_REnter(tFSMData& param)
{
    tSkill_Info* Skill = m_Data->GetSkill((UINT)SKILLIDX::R_1);
    if (Skill->Use(&GetStatus()->iSP))
    {
        float SpdValue = (float)Skill->Int1();
        float Time = Skill->Float1();
    }

    param.bData[0] = 1;    // 스킬 ON
}

void ER_ActionScript_Jackie::Skill_RUpdate(tFSMData& param)
{
}

void ER_ActionScript_Jackie::Skill_RExit(tFSMData& param)
{
}



FSMState* ER_ActionScript_Jackie::CreateWait()
{
    FSMState* state = new FSMState(this);

    STATEDELEGATE_ENTER(state, Jackie, Wait);
    STATEDELEGATE_UPDATE(state, Jackie, Wait);
    STATEDELEGATE_EXIT(state, Jackie, Wait);

    return state;
}

FSMState* ER_ActionScript_Jackie::CreateMove()
{
    FSMState* state = new FSMState(this);

    STATEDELEGATE_ENTER(state, Jackie, Move);
    STATEDELEGATE_UPDATE(state, Jackie, Move);
    STATEDELEGATE_EXIT(state, Jackie, Move);

    return state;
}

FSMState* ER_ActionScript_Jackie::CreateFarming()
{
    FSMState* state = new FSMState(this);

    STATEDELEGATE_ENTER(state, Jackie, Farming);
    STATEDELEGATE_UPDATE(state, Jackie, Farming);
    STATEDELEGATE_EXIT(state, Jackie, Farming);

    return state;
}

FSMState* ER_ActionScript_Jackie::CreateCraft()
{
    FSMState* state = new FSMState(this);

    STATEDELEGATE_ENTER(state, Jackie, Craft);
    STATEDELEGATE_UPDATE(state, Jackie, Craft);
    STATEDELEGATE_EXIT(state, Jackie, Craft);

    return state;
}

FSMState* ER_ActionScript_Jackie::CreateRest()
{
    FSMState* state = new FSMState(this);

    STATEDELEGATE_ENTER(state, Jackie, Rest);
    STATEDELEGATE_UPDATE(state, Jackie, Rest);
    STATEDELEGATE_EXIT(state, Jackie, Rest);

    return state;
}

FSMState* ER_ActionScript_Jackie::CreateAttack()
{
    FSMState* state = new FSMState(this);

    STATEDELEGATE_ENTER(state, Jackie, Attack);
    STATEDELEGATE_UPDATE(state, Jackie, Attack);
    STATEDELEGATE_EXIT(state, Jackie, Attack);

    return state;
}

FSMState* ER_ActionScript_Jackie::CreateArrive()
{
    FSMState* state = new FSMState(this);

    STATEDELEGATE_ENTER(state, Jackie, Arrive);
    STATEDELEGATE_UPDATE(state, Jackie, Arrive);
    STATEDELEGATE_EXIT(state, Jackie, Attack);

    return state;
}

FSMState* ER_ActionScript_Jackie::CreateDead()
{
    FSMState* state = new FSMState(this);

    STATEDELEGATE_ENTER(state, Jackie, Dead);
    STATEDELEGATE_UPDATE(state, Jackie, Dead);
    STATEDELEGATE_EXIT(state, Jackie, Dead);

    return state;
}

FSMState* ER_ActionScript_Jackie::CreateSkill_Q()
{
    FSMState* state = new FSMState(this);

    STATEDELEGATE_ENTER(state, Jackie, Skill_Q);
    STATEDELEGATE_UPDATE(state, Jackie, Skill_Q);
    STATEDELEGATE_EXIT(state, Jackie, Skill_Q);

    return state;
}

FSMState* ER_ActionScript_Jackie::CreateSkill_W()
{
    FSMState* state = new FSMState(this);

    STATEDELEGATE_ENTER(state, Jackie, Skill_W);
    STATEDELEGATE_UPDATE(state, Jackie, Skill_W);
    STATEDELEGATE_EXIT(state, Jackie, Skill_W);

    return state;
}

FSMState* ER_ActionScript_Jackie::CreateSkill_E()
{
    FSMState* state = new FSMState(this);

    STATEDELEGATE_ENTER(state, Jackie, Skill_E);
    STATEDELEGATE_UPDATE(state, Jackie, Skill_E);
    STATEDELEGATE_EXIT(state, Jackie, Skill_E);

    return state;
}

FSMState* ER_ActionScript_Jackie::CreateSkill_R()
{
    FSMState* state = new FSMState(this);

    STATEDELEGATE_ENTER(state, Jackie, Skill_R);
    STATEDELEGATE_UPDATE(state, Jackie, Skill_R);
    STATEDELEGATE_EXIT(state, Jackie, Skill_R);

    return state;
}

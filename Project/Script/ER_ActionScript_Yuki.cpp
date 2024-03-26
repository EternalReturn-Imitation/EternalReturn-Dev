#include "pch.h"
#include "ER_ActionScript_Yuki.h"
#include "ER_DataScript_Character.h"

#include "ER_DataScript_ItemBox.h"
#include "ER_DataScript_Item.h"

#include <Engine\CAnim3D.h>

ER_ActionScript_Yuki::ER_ActionScript_Yuki()
    : ER_ActionScript_Character(SCRIPT_TYPE::ER_ACTIONSCRIPT_YUKI)
{
}

ER_ActionScript_Yuki::~ER_ActionScript_Yuki()
{
}


void ER_ActionScript_Yuki::WaitEnter(tFSMData& param)
{
    Animator3D()->SelectAnimation(L"Yuki_Wait", true);
    SetStateGrade(eAccessGrade::BASIC);
    param.fData = 0.f;
}
void ER_ActionScript_Yuki::WaitUpdate(tFSMData& param)
{
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
}
void ER_ActionScript_Yuki::WaitExit(tFSMData& param)
{
    // 기능 없음
}

void ER_ActionScript_Yuki::MoveEnter(tFSMData& param)
{
    /*
    [MOVE]
    bData[0]	: 타겟 추적 여부
    bData[1]    : 이동속도 버프스킬 작용 여부
    fData		: 공격 가능 거리 / 파밍 가능 거리
    iData[0]	: 타겟 타입 : 1 - 공격대상 / 2 - 아이템박스
    v4Data		: 목표 이동 좌표
    */

    Animator3D()->SelectAnimation(L"Yuki_Run", true);
    FindPath()->FindPath(param.v4Data);
}
void ER_ActionScript_Yuki::MoveUpdate(tFSMData& param)
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

    // 버프/디버프 효과 반영
    tStatus_Effect* SpeedEfc = m_Data->GetStatusEffect();

    // 이동속도 설정
    float fMoveSpeed = GetStatus()->fMovementSpeed;
    fMoveSpeed += (fMoveSpeed * SpeedEfc->GetIncSPD()) - (fMoveSpeed * SpeedEfc->GetDecSPD());

    // 다음 이동지점이 없다면 대기상태로 전환
    if (!FindPath()->PathMove(fMoveSpeed))
        ChangeState(ER_CHAR_ACT::WAIT);
}
void ER_ActionScript_Yuki::MoveExit(tFSMData& param)
{
}

void ER_ActionScript_Yuki::FarmingEnter(tFSMData& param)
{
    GetOwner()->Animator3D()->SelectAnimation(L"Yuki_Wait", true);

    SetStateGrade(eAccessGrade::BASIC);

    CGameObject* ItemObj = ((CGameObject*)param.lParam);

    ER_DataScript_ItemBox* ItemBox = ItemObj->GetScript<ER_DataScript_ItemBox>();
    ER_UIMgr::GetInst()->OpenItemBoxUI(ItemBox);
}

void ER_ActionScript_Yuki::FarmingExit(tFSMData& param)
{
    ER_UIMgr::GetInst()->CloseItemBoxUI();
}

void ER_ActionScript_Yuki::RestEnter(tFSMData& param)
{
    /*
    iData[0] = 휴식 애니메이션 재생판단
    fData    = 체력재생시간 카운트
    */
    param.iData[0] = 0;
    param.fData = 0.f;
    Animator3D()->SelectAnimation(L"Yuki_Rest_Start", false);
}
void ER_ActionScript_Yuki::RestUpdate(tFSMData& param)
{
    switch (param.iData[0])
    {
    case 0: // 시작 동작
    {
        // 애니메이션 길이만큼 시전게이지 UI 출력

        if (Animator3D()->IsFinish())
        {
            Animator3D()->SelectAnimation(L"Yuki_Rest_Loop");

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
            Animator3D()->SelectAnimation(L"Yuki_Rest_End", false);
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
void ER_ActionScript_Yuki::RestExit(tFSMData& param)
{
}

void ER_ActionScript_Yuki::ArriveEnter(tFSMData& param)
{
    Animator3D()->SelectAnimation(L"Yuki_Arrive", false);
}
void ER_ActionScript_Yuki::ArriveUpdate(tFSMData& param)
{
    if (Animator3D()->IsFinish())
        ChangeState(ER_CHAR_ACT::WAIT);
}
void ER_ActionScript_Yuki::ArriveExit(tFSMData& param)
{
}

void ER_ActionScript_Yuki::DeadEnter(tFSMData& param)
{
    Animator3D()->SelectAnimation(L"Yuki_Death", false);
}
void ER_ActionScript_Yuki::DeadUpdate(tFSMData& param)
{
}
void ER_ActionScript_Yuki::DeadExit(tFSMData& param)
{
}



void ER_ActionScript_Yuki::AttackEnter(tFSMData& param)
{
    /*
   [ATTACK]
   bData[0]	    : 공격동작 진행중인지 여부
   bData[1]	    : Battle Event 실행 여부
   bData[2]	    : 다음 타겟 지정 여부
   bData[3]     : 공격모션 변경
   iData[0]	    : 타격지점 애니메이션 프레임 = Hit Frame
   lParam		: 타겟 오브젝트
   RParam		: 타겟 예정 오브젝트
   */

   // 공격 시작단계 초기화
    param.bData[0] = true;
    param.bData[1] = false;
    param.bData[3] = !param.bData[3];

    // Q스킬 적용 여부
    if (IsSkillOn(SKILLIDX::Q_1))
    {
        Animator3D()->SelectAnimation(L"Yuki_SkillQ", false);
        param.iData[0] = 6;
    }
    else if (param.bData[3])
    {
        Animator3D()->SelectAnimation(L"Yuki_Attack0", false);
        param.iData[0] = 6;
    }
    else
    {
        Animator3D()->SelectAnimation(L"Yuki_Attack1", false);
        param.iData[0] = 6;
    }
    
    // 타겟방향으로 회전
    SetRotationToTarget(((CGameObject*)param.lParam)->Transform()->GetRelativePos());
    SetStateGrade(eAccessGrade::UTMOST);
}
void ER_ActionScript_Yuki::AttackUpdate(tFSMData& param)
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
        
        if (IsSkillOn(SKILLIDX::Q_1))
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
void ER_ActionScript_Yuki::AttackExit(tFSMData& param)
{
    param.bData[0] = false;
    SetStateGrade(eAccessGrade::BASIC);
}

void ER_ActionScript_Yuki::CraftEnter(tFSMData& param)
{
}

void ER_ActionScript_Yuki::CraftUpdate(tFSMData& param)
{
}

void ER_ActionScript_Yuki::CraftExit(tFSMData& param)
{
}
void ER_ActionScript_Yuki::Skill_Q(tFSMData& _Data)
{
    ChangeState(ER_CHAR_ACT::SKILL_Q);
}

void ER_ActionScript_Yuki::Skill_W(tFSMData& _Data)
{
    ChangeState(ER_CHAR_ACT::SKILL_W);
}

void ER_ActionScript_Yuki::Skill_E(tFSMData& _Data)
{
    // 시전중이 아니라면
    if (!STATEDATA_GET(SKILL_E).iData)
        STATEDATA_SET(SKILL_E, _Data);

    ChangeState(ER_CHAR_ACT::SKILL_E);
}

void ER_ActionScript_Yuki::Skill_R(tFSMData& _Data)
{
    ChangeState(ER_CHAR_ACT::SKILL_R);
}


void ER_ActionScript_Yuki::Skill_QEnter(tFSMData& param)
{
    // 강화 평타 전달
    tSkill_Info* Skill = m_Data->GetSkill((UINT)SKILLIDX::Q_1);
    Skill->Use(&GetStatus()->iSP, true);
}

void ER_ActionScript_Yuki::Skill_QUpdate(tFSMData& param)
{
    // 실제 동작은 없는스킬.

    SetStateGrade(eAccessGrade::BASIC);

    if ((UINT)ER_CHAR_ACT::MOVE == m_iPrevState)
        ChangeState(ER_CHAR_ACT::MOVE);
    else
        ChangeState(ER_CHAR_ACT::WAIT);
}

void ER_ActionScript_Yuki::Skill_QExit(tFSMData& param)
{
}

void ER_ActionScript_Yuki::Skill_WEnter(tFSMData& param)
{
    tSkill_Info* Skill = m_Data->GetSkill((UINT)SKILLIDX::W_1);
    Skill->Use(&GetStatus()->iSP, true);

    GetOwner()->Animator3D()->SelectAnimation(L"Yuki_SkillW_Upper_Wait", false);
    SetStateGrade(eAccessGrade::ADVANCED);
}

void ER_ActionScript_Yuki::Skill_WUpdate(tFSMData& param)
{
    if (GetOwner()->Animator3D()->IsFinish())
    {
        SetStateGrade(eAccessGrade::BASIC);
        ChangeState(ER_CHAR_ACT::WAIT);
    }
}
void ER_ActionScript_Yuki::Skill_WExit(tFSMData& param)
{
}
;

void ER_ActionScript_Yuki::Skill_EEnter(tFSMData& param)
{
    tSkill_Info* Skill = m_Data->GetSkill((UINT)SKILLIDX::E_1);
    Skill->Use(&GetStatus()->iSP);

    CAnimator3D* Animator = GetOwner()->Animator3D();
    Animator->SelectAnimation(L"Yuki_SkillE_Move", false);

    // 방향 전환
    SetRotationToTarget(param.v4Data);

    // 방향 저장
    Vec3 vPos = GetOwner()->Transform()->GetRelativePos();
    Vec3 vDir = (param.v4Data - vPos).Normalize();

    // param.v4Data 에 받아온 방향정보를 v2Data로 이동
    param.v2Data.x = vDir.x;
    param.v2Data.y = vDir.z;

    param.v4Data[0] = 5.f;                                                      // 스킬 거리
    float ClearDist = GetClearDistance(vDir, param.v4Data[0]);
    param.v4Data[1] = ClearDist;                                                // 이동 가능 거리
    param.v4Data[2] = (float)Animator->GetCurAnim()->GetAnimClip().dEndTime;    // 전체 애니메이션 재생 시간
    param.v4Data[3] = 0.f;                                                      // 이동한 거리 초기화.

    param.iData[0] = 1;                                                            // 적 개체 충돌여부

    SetStateGrade(eAccessGrade::UTMOST);
}

void ER_ActionScript_Yuki::Skill_EUpdate(tFSMData& param)
{
    CTransform* transform = GetOwner()->Transform();

    switch (param.iData[0])
    {
    case 1: // 이동
    {
        // 4frm 이후부터 이동
        if (GetOwner()->Animator3D()->GetCurFrame() < 4)
            return;
        
        
        // 적개체 충돌판단
        {
            // 임의로 이동거리의 80% 지점에서 충돌했다고 가정 : TEST 코드
            if (param.v4Data[1] * 0.8 < param.v4Data[3])
            {
                GetOwner()->Animator3D()->SelectAnimation(L"Yuki_SkillE_Attack", false);
                param.iData[0]++;
                return;
            }
        }

        // 이동한거리가 이동가능거리를 넘지 않았는지 판단.
        if (param.v4Data[3] < param.v4Data[1])
        {

            // param.v4Data[0] : 스킬 거리(속도)
            // param.v4Data[1] : 이동 가능 거리
            // param.v4Data[2] : 전체 애니메이션 재생 시간
            // param.v4Data[3] : 이동한 거리

            Vec3 vPos = transform->GetRelativePos();
            Vec3 vDir(param.v2Data.x, 0.f, param.v2Data.y);

            float speed = param.v4Data[0] * 4;

            float CurFrameMoveDist = speed * param.v4Data[2] * DT;

            param.v4Data[3] += CurFrameMoveDist;

            vPos += vDir * CurFrameMoveDist;

            // 캐릭터 이동
            transform->SetRelativePos(vPos);
        }

        // 이동범위 내에 충돌이 발생하지 않고 애니메이션 종료.
        if (GetOwner()->Animator3D()->IsFinish())
            ChangeState(ER_CHAR_ACT::WAIT,eAccessGrade::UTMOST);

        break;
    }
    case 2: // 공격 애니메이션
    {
        Vec3 vPos = transform->GetRelativePos();

        // 4m 추가이동으로 갱신
        Vec3 vDir(param.v2Data.x, 0.f, param.v2Data.y);

        param.v4Data[0] = 0.5f;                                              // 스킬 거리
        float ClearDist = GetClearDistance(vDir, param.v4Data[0]);

        vPos += vDir * ClearDist;
        transform->SetRelativePos(vPos);

        param.iData[0]++;
        break;
    }
    case 3: // 애니메이션 종료
    {
        if (GetOwner()->Animator3D()->IsFinish())
            ChangeState(ER_CHAR_ACT::WAIT, eAccessGrade::UTMOST);
        
        break;
    }
    }
}

void ER_ActionScript_Yuki::Skill_EExit(tFSMData& param)
{
    param.iData[0] = 0;
}

void ER_ActionScript_Yuki::Skill_REnter(tFSMData& param)
{
    GetOwner()->Animator3D()->SelectAnimation(L"Yuki_Wait", true);
    param.iData[0] = 1; // 0. 기본, 1. 스킬 조준, 2. 스킬 차징 공격, 3. 스킬 표식 공격
}

void ER_ActionScript_Yuki::Skill_RUpdate(tFSMData& param)
{
    switch (param.iData[0])
    {
    case 1:
    {
        if (KEY_TAP(KEY::LBTN) || KEY_TAP(KEY::R))
        {
            // 스킬 조준
            Vec3 vTargetPoint = GetFocusPoint();
            SetRotationToTarget(vTargetPoint);

            // 스킬 발동
            GetOwner()->Animator3D()->SelectAnimation(L"Yuki_SkillR_Loop", false);
            SetStateGrade(eAccessGrade::UTMOST);
            param.iData[0]++;
        }
        break;
    }
    case 2:
    {
        // 스킬 타격범위 애니메이션 출력

        // 특정 프레임에 공격 모션과 함께 전투매니저호출

        if (GetOwner()->Animator3D()->IsFinish())
        {
            GetOwner()->Animator3D()->SelectAnimation(L"Yuki_SkillR_End", false);
            param.iData[0]++;
        }
        break;
    }
    case 3:
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

void ER_ActionScript_Yuki::Skill_RExit(tFSMData& param)
{
    // 스킬조준 대기상태 해제
    param.iData[0] = 0;
}


int ER_ActionScript_Yuki::SkillQ(const tSkill_Info* skilldata)
{
    return skilldata->iValue1[0] + (int)skilldata->fValue1[0];
}


FSMState* ER_ActionScript_Yuki::CreateWait()
{
    FSMState* state = new FSMState(this);

    STATEDELEGATE_ENTER(state, Yuki, Wait);
    STATEDELEGATE_UPDATE(state, Yuki, Wait);
    STATEDELEGATE_EXIT(state, Yuki, Wait);

    return state;
}
FSMState* ER_ActionScript_Yuki::CreateMove()
{
    FSMState* state = new FSMState(this);

    STATEDELEGATE_ENTER(state, Yuki, Move);
    STATEDELEGATE_UPDATE(state, Yuki, Move);
    STATEDELEGATE_EXIT(state, Yuki, Move);

    return state;
}

FSMState* ER_ActionScript_Yuki::CreateFarming()
{
    FSMState* state = new FSMState(this);

    STATEDELEGATE_ENTER(state, Yuki, Farming);
    STATEDELEGATE_UPDATE(state, Yuki, Farming);
    STATEDELEGATE_EXIT(state, Yuki, Farming);

    return state;
}

FSMState* ER_ActionScript_Yuki::CreateCraft()
{
    FSMState* state = new FSMState(this);

    STATEDELEGATE_ENTER(state, Yuki, Craft);
    STATEDELEGATE_UPDATE(state, Yuki, Craft);
    STATEDELEGATE_EXIT(state, Yuki, Craft);

    return state;
}
FSMState* ER_ActionScript_Yuki::CreateRest()
{
    FSMState* state = new FSMState(this);

    STATEDELEGATE_ENTER(state, Yuki, Rest);
    STATEDELEGATE_UPDATE(state, Yuki, Rest);
    STATEDELEGATE_EXIT(state, Yuki, Rest);

    return state;
}
FSMState* ER_ActionScript_Yuki::CreateAttack()
{
    FSMState* state = new FSMState(this);

    STATEDELEGATE_ENTER(state, Yuki, Attack);
    STATEDELEGATE_UPDATE(state, Yuki, Attack);
    STATEDELEGATE_EXIT(state, Yuki, Attack);

    return state;
}
FSMState* ER_ActionScript_Yuki::CreateArrive()
{
    FSMState* state = new FSMState(this);

    STATEDELEGATE_ENTER(state, Yuki, Arrive);
    STATEDELEGATE_UPDATE(state, Yuki, Arrive);
    STATEDELEGATE_EXIT(state, Yuki, Arrive);

    return state;
}
FSMState* ER_ActionScript_Yuki::CreateDead()
{
    FSMState* state = new FSMState(this);

    STATEDELEGATE_ENTER(state, Yuki, Dead);
    STATEDELEGATE_UPDATE(state, Yuki, Dead);
    STATEDELEGATE_EXIT(state, Yuki, Dead);

    return state;
}
FSMState* ER_ActionScript_Yuki::CreateSkill_Q()
{
    FSMState* state = new FSMState(this);

    STATEDELEGATE_ENTER(state, Yuki, Skill_Q);
    STATEDELEGATE_UPDATE(state, Yuki, Skill_Q);
    STATEDELEGATE_EXIT(state, Yuki, Skill_Q);

    return state;
}
FSMState* ER_ActionScript_Yuki::CreateSkill_W()
{
    FSMState* state = new FSMState(this);

    STATEDELEGATE_ENTER(state, Yuki, Skill_W);
    STATEDELEGATE_UPDATE(state, Yuki, Skill_W);
    STATEDELEGATE_EXIT(state, Yuki, Skill_W);

    return state;
}
FSMState* ER_ActionScript_Yuki::CreateSkill_E()
{
    FSMState* state = new FSMState(this);

    STATEDELEGATE_ENTER(state, Yuki, Skill_E);
    STATEDELEGATE_UPDATE(state, Yuki, Skill_E);
    STATEDELEGATE_EXIT(state, Yuki, Skill_E);

    return state;
}
FSMState* ER_ActionScript_Yuki::CreateSkill_R()
{
    FSMState* state = new FSMState(this);

    STATEDELEGATE_ENTER(state, Yuki, Skill_R);
    STATEDELEGATE_UPDATE(state, Yuki, Skill_R);
    STATEDELEGATE_EXIT(state, Yuki, Skill_R);

    return state;
}

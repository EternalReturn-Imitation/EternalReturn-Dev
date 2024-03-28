#include "pch.h"
#include "ER_ActionScript_Aya.h"
#include "ER_DataScript_Character.h"

#include "ER_DataScript_ItemBox.h"
#include "ER_DataScript_Item.h"
#include "ER_ProjectilePool.h"
#include "ER_ProjectileScript.h"

#include "ER_ItemMgr.h"

#include <Engine/CAnim3D.h>



ER_ActionScript_Aya::ER_ActionScript_Aya()
    : ER_ActionScript_Character(SCRIPT_TYPE::ER_ACTIONSCRIPT_AYA)
    , m_fSec(0.f)
{
}

ER_ActionScript_Aya::~ER_ActionScript_Aya()
{
}


void ER_ActionScript_Aya::WaitEnter(tFSMData& param)
{
    Animator3D()->SelectAnimation(L"Aya_Idle", true);
    SetStateGrade(eAccessGrade::BASIC);
    param.fData = 0.f;
}
void ER_ActionScript_Aya::WaitUpdate(tFSMData& param)
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
void ER_ActionScript_Aya::WaitExit(tFSMData& param)
{
    // 기능 없음
}

void ER_ActionScript_Aya::MoveEnter(tFSMData& param)
{
    /*
    [MOVE]
    bData[0]	: 타겟 추적 여부
    bData[1]    : 이동속도 버프스킬 작용 여부
    fData		: 공격 가능 거리 / 파밍 가능 거리
    iData[0]	: 타겟 타입 : 1 - 공격대상 / 2 - 아이템박스
    v4Data		: 목표 이동 좌표
    */

    // 고정사격 스킬 사용중
    if (IsSkillOn(SKILLIDX::W_1))
    {
        // Move로 들어온 이동위치만 갱신해준다.
        FindPath()->FindPath(param.v4Data);
        return;
    }

    Animator3D()->SelectAnimation(L"Aya_Run", true);
    FindPath()->FindPath(param.v4Data);
}
void ER_ActionScript_Aya::MoveUpdate(tFSMData& param)
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
void ER_ActionScript_Aya::MoveExit(tFSMData& param)
{

}

void ER_ActionScript_Aya::FarmingEnter(tFSMData& param)
{
    Animator3D()->SelectAnimation(L"Aya_Idle", true);
    
    SetStateGrade(eAccessGrade::BASIC);

    CGameObject* ItemObj = ((CGameObject*)param.lParam);

    ER_DataScript_ItemBox* ItemBox = ItemObj->GetScript<ER_DataScript_ItemBox>();
    ER_UIMgr::GetInst()->OpenItemBoxUI(ItemBox);
}
void ER_ActionScript_Aya::FarmingExit(tFSMData& param)
{
    ER_UIMgr::GetInst()->CloseItemBoxUI();
}

void ER_ActionScript_Aya::RestEnter(tFSMData& param)
{
    /*
   iData[0] = 휴식 애니메이션 재생판단
   fData    = 체력재생시간 카운트
   */
    param.iData[0] = 0;
    param.fData = 0.f;
    Animator3D()->SelectAnimation(L"Aya_Rest_Start", false);
}
void ER_ActionScript_Aya::RestUpdate(tFSMData& param)
{
    switch (param.iData[0])
    {
    case 0: // 시작 동작
    {
        // 애니메이션 길이만큼 시전게이지 UI 출력

        if (Animator3D()->IsFinish())
        {
            Animator3D()->SelectAnimation(L"Aya_Rest_Loop");

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
            Animator3D()->SelectAnimation(L"Aya_Rest_End", false);
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
void ER_ActionScript_Aya::RestExit(tFSMData& param)
{
}

void ER_ActionScript_Aya::ArriveEnter(tFSMData& param)
{
    Animator3D()->SelectAnimation(L"Aya_Arrive", false);
}
void ER_ActionScript_Aya::ArriveUpdate(tFSMData& param)
{
    if (Animator3D()->IsFinish())
        ChangeState(ER_CHAR_ACT::WAIT);
}
void ER_ActionScript_Aya::ArriveExit(tFSMData& param)
{
}

void ER_ActionScript_Aya::DeadEnter(tFSMData& param)
{
    Animator3D()->SelectAnimation(L"Aya_Death", false);
}
void ER_ActionScript_Aya::DeadUpdate(tFSMData& param)
{
}
void ER_ActionScript_Aya::DeadExit(tFSMData& param)
{
}

void ER_ActionScript_Aya::CraftEnter(tFSMData& param)
{
    GetOwner()->Animator3D()->SelectAnimation(L"Aya_Craft", false);
    
    int ItemGrade = ER_ItemMgr::GetInst()->GetItemObj(param.iData[0])->GetScript<ER_DataScript_Item>()->GetGrade();
    int CraftTime = 2 + (2 * ItemGrade);
    param.bData[0] = true;
    param.iData[1] = (int)CraftTime;
    param.fData = 0.f;

    ERCHARSOUND(CRAFT_SOUND);
}
void ER_ActionScript_Aya::CraftUpdate(tFSMData& param)
{
    param.fData += DT;

    if (param.iData[1] <= param.fData || Animator3D()->IsFinish())
    {
        // 아이탬 생성함수
        GetOwner()->GetScript<ER_DataScript_Character>()->CraftItem(param.iData[0]);

        
        ChangeState(ER_CHAR_ACT::WAIT);
    }
}
void ER_ActionScript_Aya::CraftExit(tFSMData& param)
{
    param.bData[0] = false;
    param.iData[1] = 0;
    param.fData = 0.f;
    STOPSOUND(CRAFT_SOUND);
}

void ER_ActionScript_Aya::AttackEnter(tFSMData& param)
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

    // 공격 시작단계 초기화
    param.bData[0] = true;
    param.bData[1] = false;

    Animator3D()->SelectAnimation(L"Aya_Attack", false);
    param.iData[0] = 9;

    // 타겟방향으로 회전
    SetRotationToTarget(((CGameObject*)param.lParam)->Transform()->GetRelativePos());
    SetStateGrade(eAccessGrade::UTMOST);
}

void ER_ActionScript_Aya::AttackUpdate(tFSMData& param)
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

        // 총알양 반영할지

        // 투사체 생성
        ER_ProjectileScript* Bullet = GETPROJECTILE(BULLET);

        Bullet->ShotTarget(GetOwner(),                  // 공격 오브젝트
            (CGameObject*)param.lParam,                 // 타겟 오브젝트
            GetProjSpawnPos(param.lParam),              // 투사체 생성 위치
            ER_ProjectileScript::eDmgType::NORMAL,      // 데미지 타입
            20.f);                                      // 투사체 속도

        Bullet->Shot();                                 // 투사체 발사

        param.bData[1] = true;                          // Battle Event 완료
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

void ER_ActionScript_Aya::AttackExit(tFSMData& param)
{
    param.bData[0] = false;
    SetStateGrade(eAccessGrade::BASIC);
}

void ER_ActionScript_Aya::Skill_Q(tFSMData& _Data)
{
    STATEDATA_SET(SKILL_Q, _Data);

    ChangeState(ER_CHAR_ACT::SKILL_Q);
}

void ER_ActionScript_Aya::Skill_W(tFSMData& _Data)
{
    STATEDATA_SET(SKILL_W, _Data);

    ChangeState(ER_CHAR_ACT::SKILL_W);
}

void ER_ActionScript_Aya::Skill_E(tFSMData& _Data)
{
    STATEDATA_SET(SKILL_E, _Data);

    ChangeState(ER_CHAR_ACT::SKILL_E, eAccessGrade::ADVANCED);
}

void ER_ActionScript_Aya::Skill_R(tFSMData& _Data)
{
    ChangeState(ER_CHAR_ACT::SKILL_R);
}

void ER_ActionScript_Aya::Skill_QEnter(tFSMData& param)
{
    GetOwner()->Animator3D()->SelectAnimation(L"Aya_SkillQ", false);
    SetRotationToTarget(param.v4Data);
    SetStateGrade(eAccessGrade::UTMOST);
}

void ER_ActionScript_Aya::Skill_QUpdate(tFSMData& param)
{
    CAnimator3D* animator = GetOwner()->Animator3D();

    int curFrame = animator->GetCurFrame();

    if (animator->IsFinish())
    {
        SetStateGrade(eAccessGrade::BASIC);
        ChangeState(ER_CHAR_ACT::WAIT);
        param.iData[0] = 0;
    }

    if (curFrame > 16) {
        SetStateGrade(eAccessGrade::BASIC);
        param.iData[0] = 0;
    }
}

void ER_ActionScript_Aya::Skill_QExit(tFSMData& param)
{
}

void ER_ActionScript_Aya::Skill_WEnter(tFSMData& param)
{
    CAnimator3D* Animator = GetOwner()->Animator3D();
    Animator->SelectAnimation(L"Aya_SkillW_Shot", false);

    SetRotationToTarget(param.v4Data);

    // 방향 저장
    Vec3 vPos = GetOwner()->Transform()->GetRelativePos();
    Vec3 vDir = (param.v4Data - vPos).Normalize();

    // param.v4Data 에 받아온 방향정보를 v2Data로 이동
    param.v2Data.x = vDir.x;
    param.v2Data.y = vDir.z;
    param.v2Data.Normalize();

    param.v4Data.x = vPos.x;
    param.v4Data.y = vPos.y;
    param.v4Data.z = vPos.z;

    param.iData[0] = 0;
    param.fData = 0.f;

    param.bData[0] = false;

    SetStateGrade(eAccessGrade::ADVANCED);
}

void ER_ActionScript_Aya::Skill_WUpdate(tFSMData& param)
{
    m_fSec += DT;

    CAnimator3D* Animator = GetOwner()->Animator3D();

    // 방향 저장
    Vec3 vPos = GetOwner()->Transform()->GetRelativePos();
    Vec2 vDir;//이동시 이동하는 곳의 방향.
    Vec3 vDestPoint; //이동시 목적지의 위치
    if (KEY_TAP(KEY::RBTN)) {
        vDestPoint = GetFocusPoint();
        Vec3 tempDir = vDestPoint - vPos;
        vDir.x = tempDir.x;
        vDir.y = tempDir.z;
        vDir.Normalize();

        float angleRad = atan2(vDir.y, vDir.x) - atan2(param.v2Data.y, param.v2Data.x);
        // 라디안을 도로 변환
        float angleDeg = (float)(angleRad * (180.0 / XM_PI));

        // 각도를 -180 ~ 180 범위로 조정
        if (angleDeg > 180.0) {
            angleDeg -= 360.0;
        }
        else if (angleDeg < -180.0) {
            angleDeg += 360.0;
        }

        //param.v4Data에는 이제 이동하는 목적지가 들어감.
        param.v4Data.x = vDestPoint.x;
        param.v4Data.y = vDestPoint.y;
        param.v4Data.z = vDestPoint.z;

        //angleDeg에는 방향이 들어감.
        param.fData = angleDeg;

        param.bData[0] = true;

        GetOwner()->FindPath()->FindPath(param.v4Data);
    }

    //애니메이션이 끝날때마다 이동 애니메이션 갱신
    if ((abs(vPos.x- param.v4Data.x)>0.1f || abs(vPos.z - param.v4Data.z)>0.1f) && param.bData[0]) {
        // 방향 결정
        if (param.fData > -45 && param.fData <= 45) {
            if (param.iData[0] != 1) {
                Animator->SelectAnimation(L"Aya_SkillW_Forward", true);
                param.iData[0] = 1;
            }
        }
        else if (param.fData > 45 && param.fData <= 135) {
            if (param.iData[0] != 2) {
                Animator->SelectAnimation(L"Aya_SkillW_Left", true);
                param.iData[0] = 2;
            }
        }
        else if (param.fData > -135 && param.fData <= -45) {
            if (param.iData[0] != 3) {
                Animator->SelectAnimation(L"Aya_SkillW_Right", true);
                param.iData[0] = 3;
            }
        }
        else {
            if (param.iData[0] != 4) {
                Animator->SelectAnimation(L"Aya_SkillW_Back", true);
                param.iData[0] = 4;
            }
        }
        param.bData[0] = false;
    }
    //같으면 그냥 쏨.
    else if((abs(vPos.x - param.v4Data.x) < 0.1f && abs(vPos.z - param.v4Data.z) < 0.1f)){
        Animator->SelectAnimation(L"Aya_SkillW_Shot", false);
        param.iData[0] = 0;
    }

    //목적지 위치랑 현재 위치랑 같지 않을 때, 계속 이동함.
    if (vPos != param.v4Data) {
        float speed = m_Data->GetStatus()->fMovementSpeed;
        GetOwner()->FindPath()->PathMove(speed, false);
    }

    if (m_fSec > 1.5f) {
        SetStateGrade(eAccessGrade::BASIC);
        ChangeState(ER_CHAR_ACT::WAIT);
        param.iData[0] = 0;
        param.fData = 0.f;
        param.v2Data = Vec2();
        param.v4Data = Vec4();
        m_fSec = 0.f;
    }
}

void ER_ActionScript_Aya::Skill_WExit(tFSMData& param)
{
}

void ER_ActionScript_Aya::Skill_EEnter(tFSMData& param)
{
    CAnimator3D* Animator = GetOwner()->Animator3D();
    Animator->SelectAnimation(L"Aya_SkillE", false);

    SetRotationToTarget(param.v4Data);

    // 방향 저장
    Vec3 vPos = GetOwner()->Transform()->GetRelativePos();
    Vec3 vDir = (param.v4Data - vPos).Normalize();

    // param.v4Data 에 받아온 방향정보를 v2Data로 이동
    param.v2Data.x = vDir.x;
    param.v2Data.y = vDir.z;

    param.v4Data[0] = 8.f;                                              // 스킬 거리
    float ClearDist = GetClearDistance(vDir, param.v4Data[0]);
    param.v4Data[1] = ClearDist;                                        // 이동 가능 거리
    param.v4Data[2] = (float)Animator->GetCurAnim()->GetAnimClip().dEndTime;   // 전체 애니메이션 재생 시간
    param.v4Data[3] = 0.f;                                              // 이동한 거리 초기화.

    SetStateGrade(eAccessGrade::ADVANCED);
}

void ER_ActionScript_Aya::Skill_EUpdate(tFSMData& param)
{
    CTransform* transform = GetOwner()->Transform();
    int curFrame = GetOwner()->Animator3D()->GetCurFrame();
    //끝나는 프레임은 31
    int EndFrame = GetOwner()->Animator3D()->GetCurAnim()->GetAnimClip().iEndFrame;

    // 이동한거리가 이동가능거리를 넘지 않았는지 판단.
    if (param.v4Data[3] < param.v4Data[1] * 0.8)
    {
        // param.v4Data[0] : 스킬 거리(속도)
        // param.v4Data[1] : 이동 가능 거리
        // param.v4Data[2] : 전체 애니메이션 재생 시간
        // param.v4Data[3] : 이동한 거리

        Vec3 vPos = transform->GetRelativePos();
        Vec3 vDir(param.v2Data.x, 0.f, param.v2Data.y);

        float speed = param.v4Data[0];

        if (param.v4Data[3] > (param.v4Data[1] / 5.f))
            speed = param.v4Data[0] * 1.5f;

        float CurFrameMoveDist = speed * param.v4Data[2] * DT;

        param.v4Data[3] += CurFrameMoveDist;

        vPos += vDir * CurFrameMoveDist;

        // 캐릭터 이동
        transform->SetRelativePos(vPos);
    }
    else {
        //끝까지 애니메이션 출력안해도 목표위치에 도달하면 캔슬 가능
        SetStateGrade(eAccessGrade::BASIC);
        param.v4Data = Vec4();
        param.v4Data = Vec2();
        param.iData[0] = 0;
    }

    if (GetOwner()->Animator3D()->IsFinish())
    {
        SetStateGrade(eAccessGrade::BASIC);
        // clear
        param.v4Data = Vec4();
        param.v4Data = Vec2();

        ChangeState(ER_CHAR_ACT::WAIT);
    }
}

void ER_ActionScript_Aya::Skill_EExit(tFSMData& param)
{
}

void ER_ActionScript_Aya::Skill_REnter(tFSMData& param)
{
}

void ER_ActionScript_Aya::Skill_RUpdate(tFSMData& param)
{
    CAnimator3D* animator = GetOwner()->Animator3D();
    switch (param.iData[0])
    {
    //시전 중 캐슬불가
    case 0:
    {
        animator->SelectAnimation(L"Aya_SkillR_Start", false);
        SetStateGrade(eAccessGrade::UTMOST);
        ++param.iData[0];
    }
        break;
    //후딜 캐슬 가능
    case 1:
    {
        if (animator->IsFinish())
        {
            animator->SelectAnimation(L"Aya_SkillR_End", false);
            SetStateGrade(eAccessGrade::BASIC);
            ++param.iData[0];
            param.iData[0] = 0;
        }
    }
        break;
    case 2:
    {
        if (animator->IsFinish())
        {
            SetStateGrade(eAccessGrade::BASIC);
            ChangeState(ER_CHAR_ACT::WAIT);
            param.iData[0] =0;
        }
    }
        break;
    default:
        break;
    }
}

void ER_ActionScript_Aya::Skill_RExit(tFSMData& param)
{
}


FSMState* ER_ActionScript_Aya::CreateWait()
{
    FSMState* state = new FSMState(this);

    STATEDELEGATE_ENTER(state, Aya, Wait);
    STATEDELEGATE_UPDATE(state, Aya, Wait);
    STATEDELEGATE_EXIT(state, Aya, Wait);

    return state;
}

FSMState* ER_ActionScript_Aya::CreateMove()
{
    FSMState* state = new FSMState(this);

    STATEDELEGATE_ENTER(state, Aya, Move);
    STATEDELEGATE_UPDATE(state, Aya, Move);
    STATEDELEGATE_EXIT(state, Aya, Move);

    return state;
}

FSMState* ER_ActionScript_Aya::CreateFarming()
{
    FSMState* state = new FSMState(this);

    STATEDELEGATE_ENTER(state, Aya, Farming);
    STATEDELEGATE_UPDATE(state, Aya, Farming);
    STATEDELEGATE_EXIT(state, Aya, Farming);

    return state;
}

FSMState* ER_ActionScript_Aya::CreateCraft()
{
    FSMState* state = new FSMState(this);

    STATEDELEGATE_ENTER(state, Aya, Craft);
    STATEDELEGATE_UPDATE(state, Aya, Craft);
    STATEDELEGATE_EXIT(state, Aya, Craft);

    return state;
}

FSMState* ER_ActionScript_Aya::CreateRest()
{
    FSMState* state = new FSMState(this);

    STATEDELEGATE_ENTER(state, Aya, Rest);
    STATEDELEGATE_UPDATE(state, Aya, Rest);
    STATEDELEGATE_EXIT(state, Aya, Rest);

    return state;
}

FSMState* ER_ActionScript_Aya::CreateAttack()
{
    FSMState* state = new FSMState(this);

    STATEDELEGATE_ENTER(state, Aya, Attack);
    STATEDELEGATE_UPDATE(state, Aya, Attack);
    STATEDELEGATE_EXIT(state, Aya, Attack);

    return state;
}

FSMState* ER_ActionScript_Aya::CreateArrive()
{
    FSMState* state = new FSMState(this);

    STATEDELEGATE_ENTER(state, Aya, Arrive);
    STATEDELEGATE_UPDATE(state, Aya, Arrive);
    STATEDELEGATE_EXIT(state, Aya, Arrive);

    return state;
}

FSMState* ER_ActionScript_Aya::CreateDead()
{
    FSMState* state = new FSMState(this);

    STATEDELEGATE_ENTER(state, Aya, Dead);
    STATEDELEGATE_UPDATE(state, Aya, Dead);
    STATEDELEGATE_EXIT(state, Aya, Dead);

    return state;
}

FSMState* ER_ActionScript_Aya::CreateSkill_Q()
{
    FSMState* state = new FSMState(this);

    STATEDELEGATE_ENTER(state, Aya, Skill_Q);
    STATEDELEGATE_UPDATE(state, Aya, Skill_Q);
    STATEDELEGATE_EXIT(state, Aya, Skill_Q);

    return state;
}

FSMState* ER_ActionScript_Aya::CreateSkill_W()
{
    FSMState* state = new FSMState(this);

    STATEDELEGATE_ENTER(state, Aya, Skill_W);
    STATEDELEGATE_UPDATE(state, Aya, Skill_W);
    STATEDELEGATE_EXIT(state, Aya, Skill_W);

    return state;
}

FSMState* ER_ActionScript_Aya::CreateSkill_E()
{
    FSMState* state = new FSMState(this);

    STATEDELEGATE_ENTER(state, Aya, Skill_E);
    STATEDELEGATE_UPDATE(state, Aya, Skill_E);
    STATEDELEGATE_EXIT(state, Aya, Skill_E);

    return state;
}

FSMState* ER_ActionScript_Aya::CreateSkill_R()
{
    FSMState* state = new FSMState(this);

    STATEDELEGATE_ENTER(state, Aya, Skill_R);
    STATEDELEGATE_UPDATE(state, Aya, Skill_R);
    STATEDELEGATE_EXIT(state, Aya, Skill_R);

    return state;
}

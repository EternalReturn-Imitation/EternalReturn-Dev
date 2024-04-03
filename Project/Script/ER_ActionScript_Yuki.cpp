#include "pch.h"
#include "ER_ActionScript_Yuki.h"
#include "ER_DataScript_Character.h"

#include "ER_DataScript_ItemBox.h"
#include "ER_DataScript_Item.h"

#include "ER_ItemMgr.h"
#include "ER_GameSystem.h"

#include <Engine\CAnim3D.h>

//이펙트 관련
#include "ER_YukiBAEffect.h"
#include "ER_YukiQEffect.h"
#include "ER_YukiWEffect.h"
#include "ER_YukiEEffect.h"
#include "ER_YukiREffect.h"

ER_ActionScript_Yuki::ER_ActionScript_Yuki()
    : ER_ActionScript_Character(SCRIPT_TYPE::ER_ACTIONSCRIPT_YUKI)
{
}

ER_ActionScript_Yuki::~ER_ActionScript_Yuki()
{
}

void ER_ActionScript_Yuki::begin()
{
    ER_ActionScript_Character::begin();
    ER_YukiBAEffect* BAEffect = onew(ER_YukiBAEffect);
    ER_YukiQEffect* QEffect = onew(ER_YukiQEffect);
    ER_YukiWEffect* WEffect = onew(ER_YukiWEffect);
    ER_YukiEEffect* EEffect = onew(ER_YukiEEffect);
    ER_YukiREffect* REffect = onew(ER_YukiREffect);

    GetOwner()->AddComponent(BAEffect);
    GetOwner()->AddComponent(QEffect);
    GetOwner()->AddComponent(WEffect);
    GetOwner()->AddComponent(EEffect);
    GetOwner()->AddComponent(REffect);
}

void ER_ActionScript_Yuki::WaitEnter(tFSMData& param)
{
    Animator3D()->SelectAnimation(L"Yuki_Wait", true);
    SetStateGrade(eAccessGrade::BASIC);
    param.fData[0] = 0.f;
}
void ER_ActionScript_Yuki::WaitUpdate(tFSMData& param)
{
    // 0.5초마다 체력회복
    param.fData[0] += DT;

    if (0.5f <= param.fData[0])
    {
        // HP/SP 자연 회복
        m_Data->HPRegen();
        m_Data->SPRegen();

        // 체력재생 카운트 초기화
        param.fData[0] -= 0.5f;
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
        if (IsInRange((CGameObject*)param.lParam, param.fData[0]))
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

    // 버프/디버프 효과 반영
    tStatus_Effect* SpeedEfc = m_Data->GetStatusEffect();

    // 애니메이션 반영
    float SpdEfcAnim = ((SpeedEfc->GetIncSPD()) * 10.f) + ((SpeedEfc->GetDecSPD()) * -10.f);
    Animator3D()->PlaySpeedValue(SpdEfcAnim);

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
    if(IsPlayer())
        ER_UIMgr::GetInst()->OpenItemBoxUI(ItemBox);
}
void ER_ActionScript_Yuki::FarmingExit(tFSMData& param)
{
    if(IsPlayer())
        ER_UIMgr::GetInst()->CloseItemBoxUI();
}

void ER_ActionScript_Yuki::RestEnter(tFSMData& param)
{
    /*
    iData[0] = 휴식 애니메이션 재생판단
    fData    = 체력재생시간 카운트
    */
    param.iData[0] = 0;
    param.fData[0] = 0.f;
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
        param.fData[0] += DT;

        if (0.5f <= param.fData[0])
        {
            // HP/SP 자연 회복 5배 빠르게 회복
            m_Data->HPRegen(5.f);
            m_Data->SPRegen(5.f);

            // 자원재생 카운트 초기화
            param.fData[0] -= 0.5f;
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

void ER_ActionScript_Yuki::CraftEnter(tFSMData& param)
{
    Animator3D()->SelectAnimation(L"Yuki_Craft", false);
    
    int ItemGrade = ER_ItemMgr::GetInst()->GetItemObj(param.iData[0])->GetScript<ER_DataScript_Item>()->GetGrade();
    int CraftTime = 2 + (2 * ItemGrade);
    param.bData[0] = true;
    param.iData[1] = (int)CraftTime;
    param.fData[0] = 0.f;

    ERCHARSOUND(CRAFT_SOUND);
}
void ER_ActionScript_Yuki::CraftUpdate(tFSMData& param)
{
    param.fData[0] += DT;

    if (param.iData[1] <= param.fData[0] || Animator3D()->IsFinish())
    {
        // 아이탬 생성함수
        GetOwner()->GetScript<ER_DataScript_Character>()->CraftItem(param.iData[0]);

        
        ChangeState(ER_CHAR_ACT::WAIT);
    }
}
void ER_ActionScript_Yuki::CraftExit(tFSMData& param)
{
    param.bData[0] = false;
    param.iData[1] = 0;
    param.fData[0] = 0.f;
    STOPSOUND(CRAFT_SOUND);
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
   iData[1]     : 공격모션 구분
   lParam		: 타겟 오브젝트
   RParam		: 타겟 예정 오브젝트
   */

   // 공격 시작단계 초기화
    param.bData[0] = true;
    param.bData[1] = false;
    param.bData[3] = !param.bData[3];

    // Q스킬 적용 여부 : 강화평타
    if (IsSkillOn(SKILLIDX::Q_1))
    {
        Animator3D()->SelectAnimation(L"Yuki_SkillQ", false);
        m_Data->GetSkill((UINT)SKILLIDX::Q_1)->ActionOver();
        param.iData[0] = 7;
        param.iData[1] = 2;
        ERCHARSOUND(SKILLQ_ATTACK);
    }
    else if (!param.bData[3])
    {
        Animator3D()->SelectAnimation(L"Yuki_Attack0", false);
        param.iData[0] = 8;
        param.iData[1] = 0;
        ERCHARSOUND(ATTACK_NORMAL1);
    }
    else
    {
        Animator3D()->SelectAnimation(L"Yuki_Attack1", false);
        param.iData[0] = 8;
        param.iData[1] = 1;
        ERCHARSOUND(ATTACK_NORMAL2);
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
     bData[3]   : 공격모션 변경
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
        switch (param.iData[1])
        {
        case 0:
        {

        }
        case 1:
        {
            // 기본 공격
            BATTLE_COMMON(GetOwner(), param.lParam);
            ERCHARSOUND(HITSOUND);
            break;
        }
        case 2:
        {
            // 강화평타
            tSkill_Info* skill = m_Data->GetSkill((UINT)SKILLIDX::Q_1);
            BATTLE_SKILL(GetOwner(), (CGameObject*)param.lParam, ER_ActionScript_Yuki, SkillQ, skill, 0);
            ERCHARSOUND(SKILLQ_HIT1);
            break;
        }
        }

        Vec3 _dir = GetProjSpawnPos(param.lParam) - GetOwner()->Transform()->GetRelativePos();
        _dir.y = 0.f;
        _dir.Normalize();
        Vec3 resultPos = GetProjSpawnPos(param.lParam) - _dir * 0.4f;
        resultPos.y += 0.2f;
        if (param.iData[1] == 0) {
            GetOwner()->GetScript<ER_YukiBAEffect>()->SpawnSecondEffect(resultPos, GetOwner()->Transform()->GetRelativeRot());
        }
        else if (param.iData[1] == 1) {            
            GetOwner()->GetScript<ER_YukiBAEffect>()->SpawnFirstEffect(resultPos, GetOwner()->Transform()->GetRelativeRot());
        }
        else if (param.iData[1] == 2) {
            GetOwner()->GetScript<ER_YukiQEffect>()->SpawnAttackEffect(resultPos, GetOwner()->Transform()->GetRelativeRot());
        }

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

void ER_ActionScript_Yuki::Skill_Q(tFSMData& _Data)
{
    STATEDATA_SET(SKILL_Q, _Data);
    ChangeState(ER_CHAR_ACT::SKILL_Q);
}

void ER_ActionScript_Yuki::Skill_W(tFSMData& _Data)
{
    tFSMData Prevdata = STATEDATA_GET(SKILL_W);
    if (Prevdata.bData[0])
        return;

    STATEDATA_SET(SKILL_W, _Data);
    ChangeState(ER_CHAR_ACT::SKILL_W);
}

void ER_ActionScript_Yuki::Skill_E(tFSMData& _Data)
{
    tFSMData Prevdata = STATEDATA_GET(SKILL_E);
    if (Prevdata.bData[0])
        return;

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
    if (Skill->Use(&GetStatus()->iSP, true))
    {
        // 이펙트 재생
        ERCHARSOUND(SKILLQ_MOTION);

        Vec3 resultPos = Transform()->GetRelativePos();
        GetOwner()->GetScript<ER_YukiQEffect>()->SpawnEffect(resultPos, Transform()->GetRelativeRot());
    }
    else
        ChangeState(ER_CHAR_ACT::WAIT);
}

void ER_ActionScript_Yuki::Skill_QUpdate(tFSMData& param)
{
    // 실제 동작은 없는스킬.

    SetStateGrade(eAccessGrade::BASIC);

    if ((UINT)ER_CHAR_ACT::MOVE == m_iPrevState)
        ChangeState(ER_CHAR_ACT::MOVE);
    else if ((UINT)ER_CHAR_ACT::ATTACK == m_iPrevState)
        ChangeState(ER_CHAR_ACT::ATTACK);
    else
        ChangeState(ER_CHAR_ACT::WAIT);
}

void ER_ActionScript_Yuki::Skill_QExit(tFSMData& param)
{
}

void ER_ActionScript_Yuki::Skill_WEnter(tFSMData& param)
{
    tSkill_Info* Skill = m_Data->GetSkill((UINT)SKILLIDX::W_1);
    
    if (Skill->Use(&GetStatus()->iSP, true))
    {
        param.bData[0] = true;

        // 빗겨치기 쿨다운 3초 감소 int2
        tSkill_Info* SkillE = m_Data->GetSkill((UINT)SKILLIDX::E_1);
        if (!SkillE->IsUsable)
        {
            float QCoolRatio = (float)Skill->Int2();
            SkillE->fCoolDown = SkillE->fCoolDown - QCoolRatio <= 0.5f ? 0.5f : SkillE->fCoolDown - QCoolRatio;
        }

        // [ 버프 ]
        // 공격력 증가 int1
        // 방어력 30% 증가 float1,
        int AtkValue = Skill->Int1();
        int DefValue = (int)((GetStatus()->iDefense * Skill->Float1()));
        float ActionTime = Skill->ActionTime();

        m_Data->GetStatusEffect()->ActiveEffect((UINT)eStatus_Effect::INCREASE_ATK, ActionTime, (float)AtkValue);
        m_Data->GetStatusEffect()->ActiveEffect((UINT)eStatus_Effect::INCREASE_DEF, ActionTime, (float)DefValue);

        ERCHARSOUND(SKILLW_MOTION);

        GetOwner()->GetScript<ER_YukiWEffect>()->SpawnEffect(Transform()->GetRelativePos(), Transform()->GetRelativeRot());

        Animator3D()->SelectAnimation(L"Yuki_SkillW_Upper_Wait", false);
        SetStateGrade(eAccessGrade::UTMOST);
    }
    else
        ChangeState(ER_CHAR_ACT::WAIT);

}

void ER_ActionScript_Yuki::Skill_WUpdate(tFSMData& param)
{
    if (GetOwner()->Animator3D()->IsFinish())
    {
        param.bData[0] = false;
        SetStateGrade(eAccessGrade::BASIC);
        ChangeState(ER_CHAR_ACT::WAIT);
    }
}
void ER_ActionScript_Yuki::Skill_WExit(tFSMData& param)
{
    param.bData[0] = false;
}

void ER_ActionScript_Yuki::Skill_EEnter(tFSMData& param)
{
    tSkill_Info* Skill = m_Data->GetSkill((UINT)SKILLIDX::E_1);
    
    if (Skill->Use(&GetStatus()->iSP))
    {
        Animator3D()->SelectAnimation(L"Yuki_SkillE_Move", false);
        SetRotationToTarget(param.v4Data);

        // 스킬 동작 여부
        param.bData[0] = true;

        // 공격 판정
        param.bData[1] = false;

        // 방향 저장
        Vec3 vPos = GetOwner()->Transform()->GetRelativePos();
        Vec3 vDir = (param.v4Data - vPos).Normalize();

        // 충돌 동작 구분
        param.iData[0] = 0;

        // param.v4Data 에 받아온 방향정보를 v2Data로 이동
        param.v2Data.x = vDir.x;
        param.v2Data.y = vDir.z;

        param.fData[0] = 6.f;                                                          // 스킬 거리
        float ClearDist = GetClearDistance(vDir, param.fData[0]);
        param.fData[1] = ClearDist;                                                    // 이동 가능 거리
        param.fData[2] = (float)Animator3D()->GetCurAnim()->GetAnimClip().dEndTime;    // 전체 애니메이션 재생 시간
        param.fData[3] = 0.f;                                                          // 이동한 거리 초기화.

        SetStateGrade(eAccessGrade::UTMOST);

        ERCHARSOUND(SKILLE_MOTION);
    }
    else
        ChangeState(ER_CHAR_ACT::WAIT);
}

void ER_ActionScript_Yuki::Skill_EUpdate(tFSMData& param)
{
    // Animator3D()->PlaySpeedValue(1.5f);

    switch (param.iData[0])
    {
    case 0:
    {
        // 4frm 이후부터 이동
        if (4 < Animator3D()->GetCurFrame())
        {
            param.iData[0] = 1;
            return;
        }

        break;
    }
    case 1:
    {
        // 이동한거리가 이동가능거리를 넘지 않았는지 판단.
        if (param.fData[3] < param.fData[1] - 1.f)
        {
            // param.fData[0] : 스킬 거리(속도)
            // param.fData[1] : 이동 가능 거리
            // param.fData[2] : 전체 애니메이션 재생 시간
            // param.fData[3] : 이동한 거리

            Vec3 vPos = Transform()->GetRelativePos();
            Vec3 vDir(param.v2Data.x, 0.f, param.v2Data.y);

            float speed = param.fData[0];

            if (param.fData[3] > (param.fData[1] / 5.f))
                speed = param.fData[0] * 1.2f;

            float CurFrameMoveDist = speed * param.fData[0] * DT;

            param.fData[3] += CurFrameMoveDist;

            vPos += vDir * CurFrameMoveDist;

            // 캐릭터 이동
            Transform()->SetRelativePos(vPos);
        }

        if (Animator3D()->IsFinish())
        {
            SetStateGrade(eAccessGrade::BASIC);
            ChangeState(ER_CHAR_ACT::WAIT);
        }
        break;
    }
    case 2:
    {
        if (!param.bData[1])
        {
            // 스킬 타격
            STOPSOUND(SKILLE_MOTION);
            ERCHARSOUND(SKILLE_ATTACK);
            ERCHARSOUND(SKILLE_HIT1);

            tSkill_Info* skill = m_Data->GetSkill((UINT)SKILLIDX::E_1);
            BATTLE_SKILL(GetOwner(), (CGameObject*)param.lParam, ER_ActionScript_Yuki, SkillE, skill, 0);

            param.bData[1] = true;

            GetOwner()->GetScript<ER_YukiEEffect>()->SpawnEffect(Transform()->GetRelativePos(), Transform()->GetRelativeRot());
        }

        if (Animator3D()->IsFinish())
        {
            SetStateGrade(eAccessGrade::BASIC);
            ChangeState(ER_CHAR_ACT::WAIT);
        }
        break;
    }
    }
}

void ER_ActionScript_Yuki::Skill_EExit(tFSMData& param)
{
    // 스킬 동작 여부
    param.bData[0] = false;
    param.bData[1] = false;
    // 충돌 동작 구분
    param.iData[0] = 0;
    param.v2Data = Vec2();

    param.fData[0] = 0.f;
    param.fData[1] = 0.f;
    param.fData[2] = 0.f;
    param.fData[3] = 0.f;
}

void ER_ActionScript_Yuki::Skill_REnter(tFSMData& param)
{
    tSkill_Info* Skill = m_Data->GetSkill((UINT)SKILLIDX::R_1);

    // 스킬을 사용할 수 있는지 여부 판단
    if (Skill->IsUsable && Skill->UseSP() <= GetStatus()->iSP)
    {
        GetOwner()->Animator3D()->SelectAnimation(L"Yuki_Wait", true);

        param.bData[0] = false;
        param.bData[1] = false;
        param.iData[0] = 0;     // 0. 기본, 1. 스킬 조준, 2. 스킬 차징 공격, 3. 스킬 표식 공격
        param.iData[1] = 31;    // End Anim Hit Frame
    }
    else if((UINT)ER_CHAR_ACT::MOVE == m_iPrevState)
        ChangeState(ER_CHAR_ACT::MOVE);
    else if ((UINT)ER_CHAR_ACT::ATTACK == m_iPrevState)
        ChangeState(ER_CHAR_ACT::ATTACK);
    else
        ChangeState(ER_CHAR_ACT::WAIT);
}

void ER_ActionScript_Yuki::Skill_RUpdate(tFSMData& param)
{
    switch (param.iData[0])
    {
    case 0:
    {
        // 스킬 조준
        Vec3 vTargetPoint = GetFocusPoint();
        SetRotationToTarget(vTargetPoint);

        if (KEY_TAP(KEY::LBTN) || KEY_TAP(KEY::R))
        {
            tSkill_Info* Skill = m_Data->GetSkill((UINT)SKILLIDX::R_1);

            if (Skill->Use(&GetStatus()->iSP))
            {
                param.bData[0] = true;
                param.bData[1] = false;
                param.iData[0] = 1; // 0. 기본, 1. 스킬 조준, 2. 스킬 차징 공격, 3. 스킬 표식 공격
                param.iData[1] = 31;    // End Anim Hit Frame

                GetOwner()->GetScript<ER_YukiREffect>()->AreaSpawn(Transform()->GetRelativePos(), Transform()->GetRelativeRot());

                // 스킬 발동
                GetOwner()->Animator3D()->SelectAnimation(L"Yuki_SkillR_Loop", false);
                SetStateGrade(eAccessGrade::UTMOST);
                ERCHARSOUND(SKILLR_ACTIVE);
            }
            else
            {
                ChangeState(ER_CHAR_ACT::WAIT);
            }
        }
        else if (KEY_TAP(KEY::ESC))
        {
            ChangeState(ER_CHAR_ACT::WAIT);
        }
        break;
    }
    case 1:
    {
        // 스킬 타격범위 애니메이션 출력
        // 특정 프레임에 공격 모션과 함께 전투매니저호출


        if (GetOwner()->Animator3D()->IsFinish())
        {
            ERCHARSOUND(SKILLR_ATTACK);
            // 데미지 처리
            
            bool IsHit = false;

            vector<CGameObject*> vecChar = ER_GameSystem::GetInst()->GetCharacters();
            tSkill_Info* skill = m_Data->GetSkill((UINT)SKILLIDX::R_1);

            for (auto Target : vecChar)
            {
                // 본인 검사
                if (Target == GetOwner())
                    continue;

                // 반원형태 스킬범위 판단
                if (!IsInRangeWithAngle(GetOwner(), Target, 5.f, 90.f))
                    continue;

                float DebufTime = 1.4f;
                float SpeedValue = 0.9f;

                // float 1 공퍼
                // float 2 최대뎀퍼퍼

                // 이동속도 90% 디버프
                Target->GetScript<ER_DataScript_Character>()->GetStatusEffect()->
                    ActiveEffect((UINT)eStatus_Effect::DECREASE_SPD, DebufTime, SpeedValue);

                // 데미지 판정
                IsHit = true;
                BATTLE_SKILL(GetOwner(), Target, ER_ActionScript_Yuki, SkillR1, skill, 0);
            }

            if(IsHit)
                ERCHARSOUND(SKILLR_HIT);

            GetOwner()->Animator3D()->SelectAnimation(L"Yuki_SkillR_End", false);
            param.iData[0] = 2;
            
        }
        break;
    }
    case 2:
    {
        if (!param.bData[1] && param.iData[1] < Animator3D()->GetCurFrame())
        {
            bool IsHit = false;

            param.bData[1] = true;

            vector<CGameObject*> vecChar = ER_GameSystem::GetInst()->GetCharacters();
            tSkill_Info* skill = m_Data->GetSkill((UINT)SKILLIDX::R_1);

            for (auto Target : vecChar)
            {
                // 본인 검사
                if (Target == GetOwner())
                    continue;

                // 반원형태 스킬범위 판단
                if (!IsInRangeWithAngle(GetOwner(), Target, 5.f, 90.f))
                    continue;

                // 스킬데미지에 타겟의 최대체력을 얻기위해 lParam 값으로 전달
                param.lParam = (DWORD_PTR)Target;

                // 데미지 판정
                IsHit = true;
                BATTLE_SKILL(GetOwner(), Target, ER_ActionScript_Yuki, SkillR2, skill, 1);
            }

            if (IsHit)
            {
                ERCHARSOUND(SKILLR_END);
                ERCHARSOUND(SKILLR_HIT);
            }
        }

        if (GetOwner()->Animator3D()->IsFinish())
        {
            SetStateGrade(eAccessGrade::BASIC);
            ChangeState(ER_CHAR_ACT::WAIT);
        }

        break;
    }
    }
}

void ER_ActionScript_Yuki::Skill_RExit(tFSMData& param)
{
    // 스킬조준 대기상태 해제
    param.iData[0] = 0;
    param.bData[0] = false;
    param.bData[1] = false;
}


int ER_ActionScript_Yuki::SkillQ()
{
    tSkill_Info* skill = m_Data->GetSkill((UINT)SKILLIDX::Q_1);
    int Dmg = (int)(skill->Int1() + GetStatus()->iAttackPower * 1.f);

    return Dmg;
}

int ER_ActionScript_Yuki::SkillE()
{
    tSkill_Info* skill = m_Data->GetSkill((UINT)SKILLIDX::E_1);
    int Dmg = (int)(skill->Int1() + GetStatus()->iAttackPower * 0.65f);

    return Dmg;
}

int ER_ActionScript_Yuki::SkillR1()
{
    // float 1 공퍼
    // float 2 최대뎀퍼퍼


    tSkill_Info* skill = m_Data->GetSkill((UINT)SKILLIDX::R_1);
    int Dmg = (int)(skill->Int1() + GetStatus()->iAttackPower * skill->Float1());

    return Dmg;
}

int ER_ActionScript_Yuki::SkillR2()
{
    tFSMData StateData = STATEDATA_GET(SKILL_R);
    tSkill_Info* skill = m_Data->GetSkill((UINT)SKILLIDX::R_1);
    int TargetMaxHP = ((CGameObject*)StateData.lParam)->GetScript<ER_DataScript_Character>()->GetStatus()->iMaxHP;
    
    // 최대 HP 비례데미지
    int Dmg = (int)(TargetMaxHP * skill->Float2());

    return Dmg;
}

void ER_ActionScript_Yuki::BeginOverlap(CCollider3D* _Other)
{
    // 충돌체크 true, 안전거리 끝까지이동,
    // Move Attack 재생
    tFSMData SkillE = STATEDATA_GET(SKILL_E);
    CGameObject* Target = _Other->GetOwner();

    // SkillE 시전중이고 이동상태일때
    if (SkillE.bData[0]  && 1 == SkillE.iData[0])
    {
        if (!IsCharacter(Target) || IsDead(Target))
            return;

        // param.fData[0] : 스킬 거리(속도)
        // param.fData[1] : 이동 가능 거리
        // param.fData[2] : 전체 애니메이션 재생 시간
        // param.fData[3] : 이동한 거리

        // 이때 첫 충돌한 오브젝트를 타겟으로 지정
        SkillE.lParam = (DWORD_PTR)Target;

        GetOwner()->Animator3D()->SelectAnimation(L"Yuki_SkillE_Attack", false);
        SkillE.iData[0] = 2;
        
        // 1.f 추가 순간 이동, 최대이동가능거리 제한
        Vec3 vPos = Target->Transform()->GetRelativePos();
        Vec3 vDir(SkillE.v2Data.x, 0.f, SkillE.v2Data.y);

        // 안전이동거리 최대이동거리넘지않도록 제한
        float ClearDist = SkillE.fData[3] + 1.f <= SkillE.fData[1] ? 1.f : SkillE.fData[1] - SkillE.fData[3];

        vPos += vDir * ClearDist;

        Transform()->SetRelativePos(vPos);

        STATEDATA_SET(SKILL_E, SkillE);
        return;
    }

}

void ER_ActionScript_Yuki::OnOverlap(CCollider3D* _Other)
{
}

void ER_ActionScript_Yuki::EndOverlap(CCollider3D* _Other)
{
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

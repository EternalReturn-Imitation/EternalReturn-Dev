#include "pch.h"
#include "ER_ActionScript_Hyunwoo.h"
#include "ER_DataScript_Character.h"

#include "ER_GameSystem.h"
#include "ER_ItemMgr.h"

#include "ER_DataScript_ItemBox.h"
#include "ER_DataScript_Item.h"
#include "ER_PlayerScript.h"

#include <Engine/CAnim3D.h>

//이펙트 관련
#include "ER_HyunwooBAEffect.h"
#include "ER_HyunwooQEffect.h"
#include "ER_HyunwooWEffect.h"

ER_ActionScript_Hyunwoo::ER_ActionScript_Hyunwoo()
    : ER_ActionScript_Character(SCRIPT_TYPE::ER_ACTIONSCRIPT_HYUNWOO)
    , m_pSounds{}
{
}

ER_ActionScript_Hyunwoo::~ER_ActionScript_Hyunwoo()
{
}

void ER_ActionScript_Hyunwoo::begin()
{
    ER_ActionScript_Character::begin();

    ER_HyunwooBAEffect* BAEffect = onew(ER_HyunwooBAEffect);
    ER_HyunwooQEffect* QEffect = onew(ER_HyunwooQEffect);
    ER_HyunwooWEffect* WEffect = onew(ER_HyunwooWEffect);
    //ER_HyunwooEEffect* EEffect = onew(ER_HyunwooEEffect);

    GetOwner()->AddComponent(BAEffect);
    GetOwner()->AddComponent(QEffect);
    GetOwner()->AddComponent(WEffect);
    //GetOwner()->AddComponent(EEffect);
}

void ER_ActionScript_Hyunwoo::WaitEnter(tFSMData& param)
{
    Animator3D()->SelectAnimation(L"Hyunwoo_Wait");
    SetStateGrade(eAccessGrade::BASIC);
    param.fData[0] = 0.f;
}
void ER_ActionScript_Hyunwoo::WaitUpdate(tFSMData& param)
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
void ER_ActionScript_Hyunwoo::WaitExit(tFSMData& param)
{
    // 기능 없음
}

void ER_ActionScript_Hyunwoo::MoveEnter(tFSMData& param)
{
    /*
    [MOVE]
    bData[0]	: 타겟 추적 여부
    bData[1]    : 이동속도 버프스킬 작용 여부
    fData		: 공격 가능 거리 / 파밍 가능 거리
    iData[0]	: 타겟 타입 : 1 - 공격대상 / 2 - 아이템박스
    v4Data		: 목표 이동 좌표
    */

    Animator3D()->SelectAnimation(L"Hyunwoo_Run", true);
    FindPath()->FindPath(param.v4Data);
}
void ER_ActionScript_Hyunwoo::MoveUpdate(tFSMData& param)
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

    // 애니메이션 변경 판단

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
void ER_ActionScript_Hyunwoo::MoveExit(tFSMData& param)
{
    // 기능 없음
}

void ER_ActionScript_Hyunwoo::FarmingEnter(tFSMData& param)
{
    Animator3D()->SelectAnimation(L"Hyunwoo_Wait");

    SetStateGrade(eAccessGrade::BASIC);

    CGameObject* ItemObj = ((CGameObject*)param.lParam);

    ER_DataScript_ItemBox* ItemBox = ItemObj->GetScript<ER_DataScript_ItemBox>();
    if (IsPlayer())
        ER_UIMgr::GetInst()->OpenItemBoxUI(ItemBox);
}
void ER_ActionScript_Hyunwoo::FarmingExit(tFSMData& param)
{
    if (IsPlayer())
        ER_UIMgr::GetInst()->CloseItemBoxUI();
}

void ER_ActionScript_Hyunwoo::RestEnter(tFSMData& param)
{
    /*
   iData[0] = 휴식 애니메이션 재생판단
   fData    = 체력재생시간 카운트
   */
    param.iData[0] = 0;
    param.fData[0] = 0.f;
    Animator3D()->SelectAnimation(L"Hyunwoo_Rest_Start", false);
}
void ER_ActionScript_Hyunwoo::RestUpdate(tFSMData& param)
{
    switch (param.iData[0])
    {
    case 0: // 시작 동작
    {
        // 애니메이션 길이만큼 시전게이지 UI 출력

        if (Animator3D()->IsFinish())
        {
            Animator3D()->SelectAnimation(L"Hyunwoo_Rest_Loop");

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
            Animator3D()->SelectAnimation(L"Hyunwoo_Rest_End", false);
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
void ER_ActionScript_Hyunwoo::RestExit(tFSMData& param)
{
}

void ER_ActionScript_Hyunwoo::ArriveEnter(tFSMData& param)
{
    Animator3D()->SelectAnimation(L"Hyunwoo_Arrive", false);
}
void ER_ActionScript_Hyunwoo::ArriveUpdate(tFSMData& param)
{
    if (Animator3D()->IsFinish())
        ChangeState(ER_CHAR_ACT::WAIT);
}
void ER_ActionScript_Hyunwoo::ArriveExit(tFSMData& param)
{
}

void ER_ActionScript_Hyunwoo::DeadEnter(tFSMData& param)
{
    Animator3D()->SelectAnimation(L"Hyunwoo_Death", false);
}
void ER_ActionScript_Hyunwoo::DeadUpdate(tFSMData& param)
{
}
void ER_ActionScript_Hyunwoo::DeadExit(tFSMData& param)
{
}

void ER_ActionScript_Hyunwoo::CraftEnter(tFSMData& param)
{
    Animator3D()->SelectAnimation(L"Hyunwoo_Craft", false);
    int ItemGrade = ER_ItemMgr::GetInst()->GetItemObj(param.iData[0])->GetScript<ER_DataScript_Item>()->GetGrade();
    int CraftTime = 2 + (2 * ItemGrade);
    param.bData[0] = true;
    param.iData[1] = (int)CraftTime;
    param.fData[0] = 0.f;

    ERCHARSOUND(CRAFT_SOUND);
}
void ER_ActionScript_Hyunwoo::CraftUpdate(tFSMData& param)
{
    param.fData[0] += DT;

    if (param.iData[1] <= param.fData[0] || Animator3D()->IsFinish())
    {
        // 아이탬 생성함수
        GetOwner()->GetScript<ER_DataScript_Character>()->CraftItem(param.iData[0]);

        
        ChangeState(ER_CHAR_ACT::WAIT);
    }
}
void ER_ActionScript_Hyunwoo::CraftExit(tFSMData& param)
{
    param.bData[0] = false;
    param.iData[1] = 0;
    param.fData[0] = 0.f;
    STOPSOUND(CRAFT_SOUND);
}

void ER_ActionScript_Hyunwoo::AttackEnter(tFSMData& param)
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
    param.bData[3] = !param.bData[3];

    if (param.bData[3])
    {
        Animator3D()->SelectAnimation(L"Hyunwoo_Attack0", false);
        ERCHARSOUND(ATTACK_NORMAL1);
        param.iData[0] = 6;
        param.iData[1] = 0;
    }
    else
    {
        Animator3D()->SelectAnimation(L"Hyunwoo_Attack1", false);
        ERCHARSOUND(ATTACK_NORMAL2);
        param.iData[0] = 6;
        param.iData[1] = 1;
    }

    // 타겟방향으로 회전
    SetRotationToTarget(((CGameObject*)param.lParam)->Transform()->GetRelativePos());
    SetStateGrade(eAccessGrade::UTMOST);
}
void ER_ActionScript_Hyunwoo::AttackUpdate(tFSMData& param)
{
    /*
     [ATTACK]
     bData[0]	: 공격동작 진행중인지 여부
     bData[1]	: Battle Event 실행 여부
     bData[2]	: 다음 타겟 지정 여부
     bData[3]   : 공격모션 변경
     iData[0]	: 타격지점 애니메이션 프레임 = Hit Frame
     iData[1]   : 0: 1타, 1: 2타
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
        ERCHARSOUND(HITSOUND);
        // 캐릭터 고유 공격 알고리즘
        BATTLE_COMMON(GetOwner(), param.lParam);
        param.bData[1] = true;

        SetStateGrade(eAccessGrade::BASIC);

        switch (param.iData[1])
        {
        case 0:
            GetOwner()->GetScript<ER_HyunwooBAEffect>()->SpawnFirstEffect(Transform()->GetRelativePos(), Transform()->GetRelativeRot());
            break;
        case 1:
            GetOwner()->GetScript<ER_HyunwooBAEffect>()->SpawnSecondEffect(Transform()->GetRelativePos(), Transform()->GetRelativeRot());
            break;
        default:
            break;
        }
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
void ER_ActionScript_Hyunwoo::AttackExit(tFSMData& param)
{
    param.bData[0] = false;
    SetStateGrade(eAccessGrade::BASIC);
}

void ER_ActionScript_Hyunwoo::Skill_Q(tFSMData& _Data)
{
    tFSMData Prevdata = STATEDATA_GET(SKILL_Q);
    if (Prevdata.bData[0])
        return;
    
    STATEDATA_SET(SKILL_Q, _Data);
    ChangeState(ER_CHAR_ACT::SKILL_Q);
}

void ER_ActionScript_Hyunwoo::Skill_W(tFSMData& _Data)
{
    tFSMData Prevdata = STATEDATA_GET(SKILL_W);
    if (Prevdata.bData[0])
        return;

    ChangeState(ER_CHAR_ACT::SKILL_W);
}

void ER_ActionScript_Hyunwoo::Skill_E(tFSMData& _Data)
{
    tFSMData Prevdata = STATEDATA_GET(SKILL_E);
    if (Prevdata.bData[0])
        return;

    STATEDATA_SET(SKILL_E, _Data);
    ChangeState(ER_CHAR_ACT::SKILL_E);
}

void ER_ActionScript_Hyunwoo::Skill_R(tFSMData& _Data)
{
    tFSMData Prevdata = STATEDATA_GET(SKILL_R);
    if (Prevdata.bData[0])
        return;

    STATEDATA_SET(SKILL_R, _Data);
    ChangeState(ER_CHAR_ACT::SKILL_R);
}

void ER_ActionScript_Hyunwoo::Skill_QEnter(tFSMData& param)
{
    tSkill_Info* Skill = m_Data->GetSkill((UINT)SKILLIDX::Q_1);
    
    if (Skill->Use(&GetStatus()->iSP, true))
    {
        float SpdValue = Skill->Float1();
        float Time = Skill->fActionTime;

        // 자체 이속증가 버프
        m_Data->GetStatusEffect()->ActiveEffect((UINT)eStatus_Effect::INCREASE_SPD, Time, SpdValue);
        
        param.bData[0] = true;
        param.bData[1] = false;
        param.iData[0] = 10;

        ERCHARSOUND(SKILLQ_MOTION);

        Animator3D()->SelectAnimation(L"Hyunwoo_SkillQ", false);
        SetRotationToTarget(param.v4Data);
        param.v4Data = GetFocusDir();

        SetStateGrade(eAccessGrade::UTMOST);
    }
    else
        ChangeState(ER_CHAR_ACT::WAIT);
}

void ER_ActionScript_Hyunwoo::Skill_QUpdate(tFSMData& param)
{
    if (!param.bData[1] && param.iData[0] < Animator3D()->GetCurFrame())
    {
        GetOwner()->GetScript<ER_HyunwooQEffect>()->SpawnEffect(Transform()->GetRelativePos(), Transform()->GetRelativeRot());

        ERCHARSOUND(SKILLQ_HIT);
        param.bData[1] = true;

        vector<CGameObject*> vecChar = ER_GameSystem::GetInst()->GetCharacters();
        Vec3 vPos = Transform()->GetRelativePos();
        Vec3 SkillPos = vPos + (param.v4Data * 1.f);

        for (auto Target : vecChar)
        {
            if (Target == GetOwner())
                continue;

            Vec3 TargetPos = Target->Transform()->GetRelativePos();
            float dist = Vec3::Distance(SkillPos, TargetPos);

            float DebufTime = 2.f;
            float SpeedValue = 0.4f;

            if (2.f < dist)
                continue;

            // 이속 감소 디버프
            Target->GetScript<ER_DataScript_Character>()->GetStatusEffect()->
                ActiveEffect((UINT)eStatus_Effect::DECREASE_SPD, DebufTime, SpeedValue);
            

            // 스킬 타격
            tSkill_Info* skill = m_Data->GetSkill((UINT)SKILLIDX::Q_1);
            BATTLE_SKILL(GetOwner(), Target, ER_ActionScript_Hyunwoo, SkillQ, skill, 0);
        }
    }

    if (Animator3D()->IsFinish())
    {
        SetStateGrade(eAccessGrade::BASIC);
        ChangeState(ER_CHAR_ACT::WAIT);
        param.iData[0] = 0;
    }
}

void ER_ActionScript_Hyunwoo::Skill_QExit(tFSMData& param)
{
    param.bData[0] = false;
    param.bData[1] = false;
}

void ER_ActionScript_Hyunwoo::Skill_WEnter(tFSMData& param)
{
    tSkill_Info* Skill = m_Data->GetSkill((UINT)SKILLIDX::W_1);

    if (Skill->Use(&GetStatus()->iSP, true))
    {
        // 공격력/방어력증가 버프
        int AtkValue = Skill->Int1();
        int DefValue = (int)((GetStatus()->iDefense * Skill->Float1()));
        float ActionTime = Skill->ActionTime();

        m_Data->GetStatusEffect()->ActiveEffect((UINT)eStatus_Effect::INCREASE_ATK, ActionTime, (float)AtkValue);
        m_Data->GetStatusEffect()->ActiveEffect((UINT)eStatus_Effect::INCREASE_DEF, ActionTime, (float)DefValue);

        // 이펙트 효과 재생
        ERCHARSOUND(SKILLW);

        GetOwner()->GetScript<ER_HyunwooWEffect>()->SpawnEffect(Transform()->GetRelativePos(), Transform()->GetRelativeRot());
    }
    else
        ChangeState(ER_CHAR_ACT::WAIT);
}

void ER_ActionScript_Hyunwoo::Skill_WUpdate(tFSMData& param)
{
    SetStateGrade(eAccessGrade::BASIC);

    if ((UINT)ER_CHAR_ACT::MOVE == m_iPrevState)
        ChangeState(ER_CHAR_ACT::MOVE);
    else
        ChangeState(ER_CHAR_ACT::WAIT);
}

void ER_ActionScript_Hyunwoo::Skill_WExit(tFSMData& param)
{
}

void ER_ActionScript_Hyunwoo::Skill_EEnter(tFSMData& param)
{
    tSkill_Info* Skill = m_Data->GetSkill((UINT)SKILLIDX::E_1);

    if (Skill->Use(&GetStatus()->iSP, true))
    {
        Animator3D()->SelectAnimation(L"Hyunwoo_SkillE_Start", false);
        SetRotationToTarget(param.v4Data);

        // 방향 저장
        Vec3 vPos = GetOwner()->Transform()->GetRelativePos();
        Vec3 vDir = (param.v4Data - vPos).Normalize();

        // param.v4Data 에 받아온 방향정보를 v2Data로 이동
        // 스킬 동작 여부
        param.bData[0] = true;

        // 충돌 여부
        param.bData[1] = false; 

        param.v2Data.x = vDir.x;
        param.v2Data.y = vDir.z;

        param.v4Data = Vec4(0.f,0.f,0.f,0.f);

        param.fData[0] = 15.f;                                                        // 스킬 거리
        float ClearDist = GetClearDistanceByWall(vDir, param.fData[0]);
        
        param.fData[1] = ClearDist;                                                   // 이동 가능 거리
        param.fData[2] = (float)Animator3D()->GetCurAnim()->GetAnimClip().dEndTime;   // 전체 애니메이션 재생 시간
        param.fData[3] = 0.f;                                                         // 이동한 거리 초기화.


        // 일정거리(20%) 텔레포트판정
        Vec3 vMoveDir(param.v2Data.x, 0.f, param.v2Data.y);

        float fMoveDist = param.fData[1] * 0.1f;
        param.fData[3] += fMoveDist;
        vPos += vMoveDir * fMoveDist;

        // 캐릭터 이동
        Transform()->SetRelativePos(vPos);

        SetStateGrade(eAccessGrade::UTMOST);

        ERCHARSOUND(SKILLE_SLIDE);
    }
    else
        ChangeState(ER_CHAR_ACT::WAIT);
}

void ER_ActionScript_Hyunwoo::Skill_EUpdate(tFSMData& param)
{
    // 이동한거리가 이동가능거리를 넘지 않았는지 판단.
    if (param.fData[3] < param.fData[1])
    {
        // param.fData[0] : 스킬 거리(속도)
        // param.fData[1] : 이동 가능 거리
        // param.fData[2] : 전체 애니메이션 재생 시간
        // param.fData[3] : 이동한 거리

        Vec3 vPos = Transform()->GetRelativePos();
        Vec3 vDir(param.v2Data.x, 0.f, param.v2Data.y);

        float speed = param.fData[0];

        if (param.fData[3] > (param.fData[1] / 5.f))
            speed = param.fData[0] * 1.5f;

        float CurFrameMoveDist = speed * param.fData[2] * DT;

        param.fData[3] += CurFrameMoveDist;

        vPos += vDir * CurFrameMoveDist;

        // 캐릭터 이동
        Transform()->SetRelativePos(vPos);
    }


    if (Animator3D()->IsFinish() || param.fData[3] >= param.fData[1])
    {
        // 캐릭터와 충돌했고, 이동불가지점(벽)에 부딪힌 경우
        // clearDist = param.fData[1]이 충돌안전거리인 10.f 보다 작은 경우 벽과충돌로 판단
        
        if (param.bData[1] && param.fData[1] <= 10.f)
        {
            Vec3 vPos = Transform()->GetRelativePos();
            vector<CGameObject*> vecChar = ER_GameSystem::GetInst()->GetCharacters();
            
            ERCHARSOUND(SKILLE_HIT);

            for (auto Target : vecChar)
            {
                // 본인 검사
                if (Target == GetOwner())
                    continue;

                // 거리 검사
                Vec3 TargetPos = Target->Transform()->GetRelativePos();
                float dist = Vec3::Distance(vPos, TargetPos);

                if (2.f < dist)
                    continue;
                
                tSkill_Info* skill = m_Data->GetSkill((UINT)SKILLIDX::E_1);
                BATTLE_SKILL(GetOwner(), Target, ER_ActionScript_Hyunwoo, SkillEWall, skill, 1);
            }
        }

        SetStateGrade(eAccessGrade::BASIC);
        // clear
        param.bData[0] = false;
        param.bData[1] = false;

        param.fData[0] = 0.f;
        param.fData[1] = 0.f;
        param.fData[2] = 0.f;
        param.fData[3] = 0.f;
        param.v2Data = Vec2();
        param.iData[0] = 0;

        ChangeState(ER_CHAR_ACT::WAIT);
    }
}

void ER_ActionScript_Hyunwoo::Skill_EExit(tFSMData& param)
{
}

void ER_ActionScript_Hyunwoo::Skill_REnter(tFSMData& param)
{
    tSkill_Info* Skill = m_Data->GetSkill((UINT)SKILLIDX::R_1);

    if (Skill->Use(&GetStatus()->iSP, true))
    {
        param.bData[0] = true;
        param.bData[1] = false;

        param.iData[0] = 0;
        
        // ratio
        param.fData[0] = 0.f;

        Animator3D()->SelectAnimation(L"Hyunwoo_SkillR_Start", false);
        // 이펙트 효과 재생

        SetRotationToTarget(param.v4Data);
        param.v4Data = GetFocusDir();

        ERCHARSOUND(SKILLR_CHARGING);
        SetStateGrade(eAccessGrade::UTMOST);
    }
    else
        ChangeState(ER_CHAR_ACT::WAIT);
}

void ER_ActionScript_Hyunwoo::Skill_RUpdate(tFSMData& param)
{
    if (param.iData[0] <= 1)
    {
        if (KEY_PRESSED(KEY::R))
        {
            param.fData[0] += DT;
        }
        else if (KEY_RELEASE(KEY::R))
        {
            STOPSOUND(SKILLR_CHARGING);
            ERCHARSOUND(SKILLR_HIT);
            Animator3D()->SelectAnimation(L"Hyunwoo_SkillR_End", false);
            param.iData[0] = 2;
        }
    }

    switch (param.iData[0])
    {
    case 0:
    {

        if (Animator3D()->IsFinish())
        {
            Animator3D()->SelectAnimation(L"Hyunwoo_SkillR_Loop", false);
            param.iData[0]++;
        }

        break;
    }
    case 1:
    {
        if (Animator3D()->IsFinish())
        {
            ERCHARSOUND(SKILLR_HIT);
            param.fData[0] = 4.f;
            Animator3D()->SelectAnimation(L"Hyunwoo_SkillR_End", false);
            param.iData[0]++;
        }
        break;
    }
    case 2:
    {
        if (!param.bData[1] && 3.f < Animator3D()->GetCurFrame())
        {
            tSkill_Info* skill = m_Data->GetSkill((UINT)SKILLIDX::R_1);

            vector<CGameObject*> vecChar = ER_GameSystem::GetInst()->GetCharacters();
            Vec3 vPos = Transform()->GetRelativePos();
            Vec3 SkillPos = vPos + (param.v4Data * 0.5f);

            for (auto Target : vecChar)
            {
                if (Target == GetOwner())
                    continue;

                Vec3 TargetPos = Target->Transform()->GetRelativePos();
                float dist = Vec3::Distance(SkillPos, TargetPos);

                float DebufTime = 6.f;
                float DecreaseDef = skill->Float2();

                if (2.f < dist)
                    continue;

                // 스킬 타격
                tSkill_Info* skill = m_Data->GetSkill((UINT)SKILLIDX::R_1);
                BATTLE_SKILL(GetOwner(), Target, ER_ActionScript_Hyunwoo, SkillR, skill, 0);

                // 방어력 감소 디버프
                Target->GetScript<ER_DataScript_Character>()->GetStatusEffect()->
                    ActiveEffect((UINT)eStatus_Effect::DECREASE_DEF, DebufTime, DecreaseDef);
            }

            param.bData[1] = true;
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

void ER_ActionScript_Hyunwoo::Skill_RExit(tFSMData& param)
{
    param.bData[0] = false;
    param.bData[1] = false;
    param.iData[0] = 0;
    param.fData[0] = 0.f;
}

FSMState* ER_ActionScript_Hyunwoo::CreateWait()
{
    FSMState* state = new FSMState(this);

    STATEDELEGATE_ENTER(state, Hyunwoo, Wait);
    STATEDELEGATE_UPDATE(state, Hyunwoo, Wait);
    STATEDELEGATE_EXIT(state, Hyunwoo, Wait);

    return state;
}

FSMState* ER_ActionScript_Hyunwoo::CreateMove()
{
    FSMState* state = new FSMState(this);

    STATEDELEGATE_ENTER(state, Hyunwoo, Move);
    STATEDELEGATE_UPDATE(state, Hyunwoo, Move);
    STATEDELEGATE_EXIT(state, Hyunwoo, Move);

    return state;
}

FSMState* ER_ActionScript_Hyunwoo::CreateFarming()
{
    FSMState* state = new FSMState(this);

    STATEDELEGATE_ENTER(state, Hyunwoo, Farming);
    STATEDELEGATE_UPDATE(state, Hyunwoo, Farming);
    STATEDELEGATE_EXIT(state, Hyunwoo, Farming);

    return state;
}

FSMState* ER_ActionScript_Hyunwoo::CreateCraft()
{
    FSMState* state = new FSMState(this);

    STATEDELEGATE_ENTER(state, Hyunwoo, Craft);
    STATEDELEGATE_UPDATE(state, Hyunwoo, Craft);
    STATEDELEGATE_EXIT(state, Hyunwoo, Craft);

    return state;
}

FSMState* ER_ActionScript_Hyunwoo::CreateRest()
{
    FSMState* state = new FSMState(this);

    STATEDELEGATE_ENTER(state, Hyunwoo, Rest);
    STATEDELEGATE_UPDATE(state, Hyunwoo, Rest);
    STATEDELEGATE_EXIT(state, Hyunwoo, Rest);

    return state;
}

FSMState* ER_ActionScript_Hyunwoo::CreateAttack()
{
    FSMState* state = new FSMState(this);

    STATEDELEGATE_ENTER(state, Hyunwoo, Attack);
    STATEDELEGATE_UPDATE(state, Hyunwoo, Attack);
    STATEDELEGATE_EXIT(state, Hyunwoo, Attack);

    return state;
}

FSMState* ER_ActionScript_Hyunwoo::CreateArrive()
{
    FSMState* state = new FSMState(this);

    STATEDELEGATE_ENTER(state, Hyunwoo, Arrive);
    STATEDELEGATE_UPDATE(state, Hyunwoo, Arrive);
    STATEDELEGATE_EXIT(state, Hyunwoo, Arrive);

    return state;
}

FSMState* ER_ActionScript_Hyunwoo::CreateDead()
{
    FSMState* state = new FSMState(this);

    STATEDELEGATE_ENTER(state, Hyunwoo, Dead);
    STATEDELEGATE_UPDATE(state, Hyunwoo, Dead);
    STATEDELEGATE_EXIT(state, Hyunwoo, Dead);

    return state;
}

FSMState* ER_ActionScript_Hyunwoo::CreateSkill_Q()
{
    FSMState* state = new FSMState(this);

    STATEDELEGATE_ENTER(state, Hyunwoo, Skill_Q);
    STATEDELEGATE_UPDATE(state, Hyunwoo, Skill_Q);
    STATEDELEGATE_EXIT(state, Hyunwoo, Skill_Q);

    return state;
}

FSMState* ER_ActionScript_Hyunwoo::CreateSkill_W()
{
    FSMState* state = new FSMState(this);

    STATEDELEGATE_ENTER(state, Hyunwoo, Skill_W);
    STATEDELEGATE_UPDATE(state, Hyunwoo, Skill_W);
    STATEDELEGATE_EXIT(state, Hyunwoo, Skill_W);

    return state;
}

FSMState* ER_ActionScript_Hyunwoo::CreateSkill_E()
{
    FSMState* state = new FSMState(this);

    STATEDELEGATE_ENTER(state, Hyunwoo, Skill_E);
    STATEDELEGATE_UPDATE(state, Hyunwoo, Skill_E);
    STATEDELEGATE_EXIT(state, Hyunwoo, Skill_E);

    return state;
}

FSMState* ER_ActionScript_Hyunwoo::CreateSkill_R()
{
    FSMState* state = new FSMState(this);

    STATEDELEGATE_ENTER(state, Hyunwoo, Skill_R);
    STATEDELEGATE_UPDATE(state, Hyunwoo, Skill_R);
    STATEDELEGATE_EXIT(state, Hyunwoo, Skill_R);

    return state;
}


int ER_ActionScript_Hyunwoo::SkillQ()
{
    tSkill_Info* skill = m_Data->GetSkill((UINT)SKILLIDX::Q_1);
    int Dmg = (int)(skill->Int1() + GetStatus()->iAttackPower * 0.4f);

    return Dmg;
}

int ER_ActionScript_Hyunwoo::SkillE()
{
    tSkill_Info* skill = m_Data->GetSkill((UINT)SKILLIDX::E_1);
    int Dmg = (int)(skill->Int1() + GetStatus()->iAttackPower * 0.55f);

    return Dmg;
}

int ER_ActionScript_Hyunwoo::SkillEWall()
{
    tSkill_Info* skill = m_Data->GetSkill((UINT)SKILLIDX::E_1);
    int Dmg = (int)(skill->Int2() + GetStatus()->iAttackPower * 0.55f);

    return Dmg;
}

int ER_ActionScript_Hyunwoo::SkillR()
{
    tFSMData SkillR = STATEDATA_GET(SKILL_R);

    tSkill_Info* skill = m_Data->GetSkill((UINT)SKILLIDX::R_1);
    
    int MinDmg = (int)(skill->Int1() + GetStatus()->iAttackPower * 0.35f);
    int MaxDmg = (int)(skill->Int2() + GetStatus()->iAttackPower * 1.05f);
    
    float fratio = SkillR.fData[0] / 4.f;

    // 차징한 만큼 0 ~ 1까지의 비율을 곱해서 데미지에 더해준다
    int Dmg = (int)(MinDmg + ((MaxDmg - MinDmg) * fratio));

    return Dmg;
}

void ER_ActionScript_Hyunwoo::BeginOverlap(CCollider3D* _Other)
{
    tFSMData SkillE = STATEDATA_GET(SKILL_E);
    CGameObject* Target = _Other->GetOwner();

    if (SkillE.bData[0] && IsCharacter(Target))
    {
        // E Skill 1타 데미지
        tSkill_Info* skill = m_Data->GetSkill((UINT)SKILLIDX::E_1);
        BATTLE_SKILL(GetOwner(), Target, ER_ActionScript_Hyunwoo, SkillE, skill, 0);
    }
}

void ER_ActionScript_Hyunwoo::OnOverlap(CCollider3D* _Other)
{
    tFSMData SkillE = STATEDATA_GET(SKILL_E);
    CGameObject* Target = _Other->GetOwner();

    // 스킬이 실행중이고 충돌판정이 켜져있을 때
    if (SkillE.bData[0] && IsCharacter(Target))
    {
        // 충돌한 캐릭터가 있는경우 안전거리 확보
        if (!SkillE.bData[1])
        {
            // 충돌함으로 표시
            SkillE.bData[1] = true;
            SkillE.fData[1] = SkillE.fData[1] - 1.2f <= 0.f ? 0.f : SkillE.fData[1] - 1.2f;
            STATEDATA_SET(SKILL_E, SkillE);
        }

        // 이동 방향
        // 이동 거리
        Vec3 vPos = Target->Transform()->GetRelativePos();
        Vec3 vDir(SkillE.v2Data.x, 0.f, SkillE.v2Data.y);

        float speed = SkillE.fData[0];

        if (SkillE.fData[3] > (SkillE.fData[1] / 5.f))
            speed = SkillE.fData[0] * 1.5f;

        float CurFrameMoveDist = speed * SkillE.fData[2] * DT;

        // 최대 이동가능 거리 이상으로 이동하는 경우
        if (SkillE.fData[1] < SkillE.fData[3] + CurFrameMoveDist)
            return;

        vPos += vDir * CurFrameMoveDist;

        // 캐릭터 이동
        Target->Transform()->SetRelativePos(vPos);
    }
}

void ER_ActionScript_Hyunwoo::EndOverlap(CCollider3D* _Other)
{
}

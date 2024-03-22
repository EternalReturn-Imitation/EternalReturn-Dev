#include "pch.h"
#include "ER_ActionScript_Jackie.h"
#include "ER_DataScript_Character.h"

#include "ER_UIMgr.h"
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

void ER_ActionScript_Jackie::MoveEnter(tFSMData& param)
{
    tSkill_Info* WSkill = m_Data->GetSkill((UINT)SKILLIDX::W_1);

    if(WSkill->IsAction)
        GetOwner()->Animator3D()->SelectAnimation(L"Jackie_SkillW", true);
    else
        GetOwner()->Animator3D()->SelectAnimation(L"Jackie_Run", true);

    SetStateGrade(eAccessGrade::BASIC);

    Vec3 DestPos = param.v4Data;

    CFindPath* findpathcomp = GetOwner()->FindPath();
    findpathcomp->FindPath(DestPos);
}

void ER_ActionScript_Jackie::MoveUpdate(tFSMData& param)
{
    tFSMData Atkdata = STATEDATA_GET(ATTACK);

    // 스킬정보 얻어오고 지금 스킬상태 따라서 세팅해주기.
    tSkill_Info* WSkill = m_Data->GetSkill((UINT)SKILLIDX::W_1);
    bool DefaultRun = GetOwner()->Animator3D()->GetCurAnim()->GetName() != "Jackie_Run";
    
    if (DefaultRun && !WSkill->IsAction)
        GetOwner()->Animator3D()->SelectAnimation(L"Jackie_Run", true);

    // 공격추적상태라면
    if (Atkdata.bData[0])
    {
        CGameObject* TargetObj = (CGameObject*)Atkdata.lParam;
        float AtkRange = m_Data->GetStatus()->fAtakRange;

        if (IsInRange(TargetObj, AtkRange))
        {
            Atkdata.bData[0] = false;
            GetOwner()->FindPath()->ClearPath();
            ChangeState(ER_CHAR_ACT::ATTACK, eAccessGrade::DISABLE);
            return;
        }
    }

    // 캐릭터 속도 얻어와서 넣어주기
    tStatus_Effect* statusefc = m_Data->GetStatusEffect();
    float speed = m_Data->GetStatus()->fMovementSpeed;
    speed += (speed * statusefc->GetIncSPD()) - (speed * statusefc->GetDecSPD());

    // 다음 이동지점이 없다면 대기상태로 전환
    if (!GetOwner()->FindPath()->PathMove(speed))
        ChangeState(ER_CHAR_ACT::WAIT);
}

void ER_ActionScript_Jackie::MoveExit(tFSMData& param)
{
}

void ER_ActionScript_Jackie::FarmingEnter(tFSMData& param)
{
    if (0 == STATEDATA_GET(SKILL_W).iData)
        GetOwner()->Animator3D()->SelectAnimation(L"Jackie_Run", true);
    else
        GetOwner()->Animator3D()->SelectAnimation(L"Jackie_SkillW", true);

    SetStateGrade(eAccessGrade::BASIC);

    Vec3 DestPos = param.v4Data;

    CFindPath* findpathcomp = GetOwner()->FindPath();
    findpathcomp->FindPath(((CGameObject*)param.lParam)->Transform()->GetRelativePos());

    m_pFarmingObject = (CGameObject*)param.lParam;

    m_bFarmingTrigger = true;
}

void ER_ActionScript_Jackie::FarmingUpdate(tFSMData& param)
{
    float speed = m_Data->GetStatus()->fMovementSpeed;

    // 다음 이동지점이 없다면 대기상태로 전환
    if (!GetOwner()->FindPath()->PathMove(speed))
        GetOwner()->Animator3D()->SelectAnimation(L"Jackie_Wait", true);

    Vec3 ownerPos = GetOwner()->Transform()->GetRelativePos();
    Vec3 ObjectPos = ((CGameObject*)param.lParam)->Transform()->GetRelativePos();

    XMVECTOR vRangeScale = XMVector3Length(ownerPos - ObjectPos);
    float rangeScale = XMVectorGetX(vRangeScale);

    if (abs(rangeScale) < 2.0f && m_bFarmingTrigger) {
        Vec3 posResult = ER_UIMgr::GetInst()->WorldPosToUIPos(GetOwner()->Transform()->GetRelativePos());
        ER_UIMgr::GetInst()->GetItemBoxBackground()->SetEnable(true);
        ER_UIMgr::GetInst()->GetItemBoxBackground()->Transform()->SetRelativePos(Vec3(posResult.x, posResult.y - 100.f, -1.0f));

        vector<CGameObject*> itemList = ((CGameObject*)param.lParam)->GetScript<ER_DataScript_ItemBox>()->GetItemList();
        for (int i = 0; i < itemList.size(); ++i) {
            if (itemList[i]) {
                std::pair<CGameObject*, CGameObject*> itemLists = ER_UIMgr::GetInst()->GetItemBoxList((int)i / 4, (int)i % 4);

                itemLists.first->SetEnable(true);
                itemLists.second->SetEnable(true);

                itemLists.first->MeshRender()->GetMaterial(0)->SetTexParam(TEX_PARAM::TEX_0, ER_UIMgr::GetInst()->GetGradeTexture(itemList[i]->GetScript<ER_DataScript_Item>()->GetGrade()));
                itemLists.second->MeshRender()->GetMaterial(0)->SetTexParam(TEX_PARAM::TEX_0, itemList[i]->GetScript<ER_DataScript_Item>()->GetItemTex().Get());
            }
        }

        m_bFarmingTrigger = false;
    }
}

void ER_ActionScript_Jackie::FarmingExit(tFSMData& param)
{
    ER_UIMgr::GetInst()->GetItemBoxBackground()->SetEnable(false);

    for (int i = 0; i < 2; ++i) {
        for (int j = 0; j < 4; ++j) {
            std::pair<CGameObject*, CGameObject*> itemLists = ER_UIMgr::GetInst()->GetItemBoxList(i, j);

            itemLists.first->SetEnable(false);
            itemLists.second->SetEnable(false);
        }
    }

    m_pFarmingObject = nullptr;
}


void ER_ActionScript_Jackie::CraftEnter(tFSMData& param)
{
}

void ER_ActionScript_Jackie::CraftUpdate(tFSMData& param)
{
}

void ER_ActionScript_Jackie::CraftExit(tFSMData& param)
{
}

void ER_ActionScript_Jackie::WaitEnter(tFSMData& param)
{
    GetOwner()->Animator3D()->SelectAnimation(L"Jackie_Wait", true);

    SetStateGrade(eAccessGrade::BASIC);
}

void ER_ActionScript_Jackie::WaitUpdate(tFSMData& param)
{
}

void ER_ActionScript_Jackie::WaitExit(tFSMData& param)
{
}

void ER_ActionScript_Jackie::ArriveEnter(tFSMData& param)
{
    GetOwner()->Animator3D()->SelectAnimation(L"Jackie_Arrive", false);
}

void ER_ActionScript_Jackie::ArriveUpdate(tFSMData& param)
{
    if (GetOwner()->Animator3D()->IsFinish())
        ChangeState(ER_CHAR_ACT::WAIT);
}

void ER_ActionScript_Jackie::ArriveExit(tFSMData& param)
{
}

void ER_ActionScript_Jackie::AttackEnter(tFSMData& param)
{
    param.bData[0] = false;
    param.bData[2] = false;
    param.bData[3] = !param.bData[3];


    if (STATEDATA_GET(SKILL_R).bData[0])
    {
        if (param.bData[3])
            GetOwner()->Animator3D()->SelectAnimation(L"Jackie_R_Attack0", false);
        else
            GetOwner()->Animator3D()->SelectAnimation(L"Jackie_R_Attack1", false);
    }
    else
    {
        if (param.bData[3])
            GetOwner()->Animator3D()->SelectAnimation(L"Jackie_Attack0", false);
        else
            GetOwner()->Animator3D()->SelectAnimation(L"Jackie_Attack1", false);
    }


    SetRotationToTarget(((CGameObject*)param.lParam)->Transform()->GetRelativePos());
}

void ER_ActionScript_Jackie::AttackUpdate(tFSMData& param)
{
    // bData[0] : 공격 대상 추적상태
    // bData[1] : -
    // bData[2] : Hit판정 실행여부
    // bData[3] : 공격모션 변경

    CAnimator3D* animator = GetOwner()->Animator3D();
    float Atkspd = m_Data->GetStatus()->fAttackSpeed;
    tStatus_Effect* statusefc = m_Data->GetStatusEffect();
    Atkspd += (Atkspd * statusefc->GetIncAPD()) - (Atkspd * statusefc->GetDecAPD());

    // 애니메이션 속도 증가
    animator->PlaySpeedValue(Atkspd);

    // 공격판정
    int HitFrame = param.bData[3] ? 8 : 8;
    if (!param.bData[2] && animator->GetCurFrame() < HitFrame)
    {
        BATTLE_COMMON(GetOwner(), param.lParam);
        param.bData[2] = true;
    }


    if (animator->IsFinish())
    {
        CGameObject* Target = (CGameObject*)param.lParam;
        // 사망 판단
        bool IsDead = Target->GetScript<ER_DataScript_Character>()->IsDeadState();

        if (IsDead)
            ChangeState(ER_CHAR_ACT::WAIT);
        else
        {
            // 거리 판단
            float AtkRange = m_Data->GetStatus()->fAtakRange;

            if (IsInRange(Target, AtkRange))
                AttackEnter(param);
            else
                Attack(param);
        }
    }
}

void ER_ActionScript_Jackie::AttackExit(tFSMData& param)
{
}

void ER_ActionScript_Jackie::RestEnter(tFSMData& param)
{
    GetOwner()->Animator3D()->SelectAnimation(L"Jackie_Rest_Start", false);

    param.iData = 0;
}

void ER_ActionScript_Jackie::RestUpdate(tFSMData& param)
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
            animator->SelectAnimation(L"Jackie_Rest_Loop");

            // 상태변경불가
            SetStateGrade(eAccessGrade::DISABLE);
            param.iData++;
        }
        break;
    }
    case 1: // 시전 중
    {
        // 캔슬 불가
        if (KEY_TAP(KEY::RBTN) || KEY_TAP(KEY::X))
        {
            animator->SelectAnimation(L"Jackie_Rest_End", false);
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
            SetStateGrade(eAccessGrade::BASIC);
            ChangeState(ER_CHAR_ACT::WAIT);
            param.iData = 0;
        }
        break;
    }
    }
}

void ER_ActionScript_Jackie::RestExit(tFSMData& param)
{
}

void ER_ActionScript_Jackie::Skill_QEnter(tFSMData& param)
{
    tSkill_Info* Skill = m_Data->GetSkill((UINT)SKILLIDX::Q_1);

    GetOwner()->Animator3D()->SelectAnimation(L"Jackie_Wait", true);
    param.iData = 1; // 0. 기본, 1. 스킬 조준, 2. 스킬 공격
}

void ER_ActionScript_Jackie::Skill_QUpdate(tFSMData& param)
{
    switch (param.iData)
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
            SetStateGrade(eAccessGrade::DISABLE);
            param.iData++;
        }
        break;
    }
    case 2:
    {

        // 특정 프레임에 공격 모션과 함께 전투 매니저 호출

        if (GetOwner()->Animator3D()->IsFinish())
            ChangeState(ER_CHAR_ACT::WAIT, eAccessGrade::DISABLE);

        break;
    }
    default:
        break;
    }
}

void ER_ActionScript_Jackie::Skill_QExit(tFSMData& param)
{
    param.iData = 0;
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
    param.iData = 1; // 0. 기본, 1. 스킬 조준, 2. 스킬 공격
}

void ER_ActionScript_Jackie::Skill_EUpdate(tFSMData& param)
{
    CTransform* transform = GetOwner()->Transform();
    CAnimator3D* animator = GetOwner()->Animator3D();

    switch (param.iData)
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
            param.iData++;
            SetStateGrade(eAccessGrade::DISABLE);
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
            param.iData++;
        }

        break;
    }
    case 3:
    {
        // 찍기
        // 

        if (GetOwner()->Animator3D()->IsFinish())
            ChangeState(ER_CHAR_ACT::WAIT, eAccessGrade::DISABLE);

        break;
    }

    default:
        break;
    }
}

void ER_ActionScript_Jackie::Skill_EExit(tFSMData& param)
{
    param.iData = 0;
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

void ER_ActionScript_Jackie::DeadEnter(tFSMData& param)
{
    GetOwner()->Animator3D()->SelectAnimation(L"Jackie_Death", true);
}

void ER_ActionScript_Jackie::DeadUpdate(tFSMData& param)
{
}

void ER_ActionScript_Jackie::DeadExit(tFSMData& param)
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

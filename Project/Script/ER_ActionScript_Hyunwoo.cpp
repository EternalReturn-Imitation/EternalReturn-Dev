#include "pch.h"
#include "ER_ActionScript_Hyunwoo.h"
#include "ER_DataScript_Character.h"

#include "ER_UIMgr.h"
#include "ER_DataScript_ItemBox.h"
#include "ER_DataScript_Item.h"

#include <Engine/CAnim3D.h>

ER_ActionScript_Hyunwoo::ER_ActionScript_Hyunwoo()
    : ER_ActionScript_Character(SCRIPT_TYPE::ER_ACTIONSCRIPT_HYUNWOO)
{
}

ER_ActionScript_Hyunwoo::~ER_ActionScript_Hyunwoo()
{
}

void ER_ActionScript_Hyunwoo::Skill_Q(tFSMData& _Data)
{
    STATEDATA_SET(SKILL_Q, _Data);

    ChangeState(ER_CHAR_ACT::SKILL_Q);
}

void ER_ActionScript_Hyunwoo::Skill_W(tFSMData& _Data)
{
}

void ER_ActionScript_Hyunwoo::Skill_E(tFSMData& _Data)
{
    STATEDATA_SET(SKILL_E, _Data);

    ChangeState(ER_CHAR_ACT::SKILL_E, eAccessGrade::ADVANCED);
}

void ER_ActionScript_Hyunwoo::Skill_R(tFSMData& _Data)
{
    STATEDATA_SET(SKILL_R, _Data);

    ChangeState(ER_CHAR_ACT::SKILL_R, eAccessGrade::ADVANCED);
}

void ER_ActionScript_Hyunwoo::MoveEnter(tFSMData& param)
{
    GetOwner()->Animator3D()->SelectAnimation(L"Hyunwoo_Run");
    SetStateGrade(eAccessGrade::BASIC);

    Vec3 DestPos = param.v4Data;

    CFindPath* findpathcomp = GetOwner()->FindPath();
    findpathcomp->FindPath(DestPos);
}

void ER_ActionScript_Hyunwoo::MoveExit(tFSMData& param)
{
}

void ER_ActionScript_Hyunwoo::MoveUpdate(tFSMData& param)
{
    tFSMData Atkdata = STATEDATA_GET(ATTACK);

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

void ER_ActionScript_Hyunwoo::FarmingEnter(tFSMData& param)
{
    GetOwner()->Animator3D()->SelectAnimation(L"Hyunwoo_Run", true);

    SetStateGrade(eAccessGrade::BASIC);

    Vec3 DestPos = param.v4Data;

    CFindPath* findpathcomp = GetOwner()->FindPath();
    findpathcomp->FindPath(((CGameObject*)param.lParam)->Transform()->GetRelativePos());

    m_pFarmingObject = (CGameObject*)param.lParam;

    m_bFarmingTrigger = true;
}

void ER_ActionScript_Hyunwoo::FarmingUpdate(tFSMData& param)
{
    float speed = m_Data->GetStatus()->fMovementSpeed;

    // 다음 이동지점이 없다면 대기상태로 전환
    if (!GetOwner()->FindPath()->PathMove(speed))
        GetOwner()->Animator3D()->SelectAnimation(L"Hyunwoo_Wait", true);

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

void ER_ActionScript_Hyunwoo::FarmingExit(tFSMData& param)
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



void ER_ActionScript_Hyunwoo::WaitEnter(tFSMData& param)
{
    GetOwner()->Animator3D()->SelectAnimation(L"Hyunwoo_Wait");

    SetStateGrade(eAccessGrade::BASIC);
}

void ER_ActionScript_Hyunwoo::WaitUpdate(tFSMData& param)
{
}

void ER_ActionScript_Hyunwoo::WaitExit(tFSMData& param)
{
}

void ER_ActionScript_Hyunwoo::ArriveEnter(tFSMData& param)
{
    GetOwner()->Animator3D()->SelectAnimation(L"Hyunwoo_Arrive", false);
}

void ER_ActionScript_Hyunwoo::ArriveUpdate(tFSMData& param)
{
    if (GetOwner()->Animator3D()->IsFinish())
        ChangeState(ER_CHAR_ACT::WAIT);
}

void ER_ActionScript_Hyunwoo::ArriveExit(tFSMData& param)
{
}

void ER_ActionScript_Hyunwoo::AttackEnter(tFSMData& param)
{
    param.bData[0] = false;
    param.bData[2] = false;
    param.bData[3] = !param.bData[3];

    if (param.bData[3])
        GetOwner()->Animator3D()->SelectAnimation(L"Hyunwoo_Attack0", false);
    else
        GetOwner()->Animator3D()->SelectAnimation(L"Hyunwoo_Attack1", false);


    SetRotationToTarget(((CGameObject*)param.lParam)->Transform()->GetRelativePos());
    // 공격모션 2개 번갈아가면서
}

void ER_ActionScript_Hyunwoo::AttackUpdate(tFSMData& param)
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

void ER_ActionScript_Hyunwoo::AttackExit(tFSMData& param)
{
}

void ER_ActionScript_Hyunwoo::CraftEnter(tFSMData& param)
{
    GetOwner()->Animator3D()->SelectAnimation(L"Hyunwoo_Craft", false);
}

void ER_ActionScript_Hyunwoo::CraftUpdate(tFSMData& param)
{
}

void ER_ActionScript_Hyunwoo::CraftExit(tFSMData& param)
{
}

void ER_ActionScript_Hyunwoo::RestEnter(tFSMData& param)
{
    GetOwner()->Animator3D()->SelectAnimation(L"Hyunwoo_Rest_Start", false);

    param.iData = 0;
}

void ER_ActionScript_Hyunwoo::RestUpdate(tFSMData& param)
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
            animator->SelectAnimation(L"Hyunwoo_Rest_Loop");

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
            animator->SelectAnimation(L"Hyunwoo_Rest_End", false);
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

void ER_ActionScript_Hyunwoo::RestExit(tFSMData& param)
{
}

void ER_ActionScript_Hyunwoo::Skill_QEnter(tFSMData& param)
{
    tSkill_Info* Skill = m_Data->GetSkill((UINT)SKILLIDX::Q_1);
    if (Skill->Use(&GetStatus()->iSP, true))
    {
        float SpdValue = Skill->Float1();
        float Time = Skill->fActionTime;
        m_Data->GetStatusEffect()->ActiveEffect((UINT)eStatus_Effect::INCREASE_SPD, Time, SpdValue);
        GetOwner()->Animator3D()->SelectAnimation(L"Hyunwoo_SkillQ", false);
    }
    else
        GetOwner()->Animator3D()->SelectAnimation(L"Hyunwoo_Wait", false);

    SetRotationToTarget(param.v4Data);
    SetStateGrade(eAccessGrade::DISABLE);

    SetRotationToTarget(param.v4Data);
}

void ER_ActionScript_Hyunwoo::Skill_QUpdate(tFSMData& param)
{
    CAnimator3D* animator = GetOwner()->Animator3D();

    int curFrame = animator->GetCurFrame();

    if (animator->IsFinish())
    {
        SetStateGrade(eAccessGrade::BASIC);
        ChangeState(ER_CHAR_ACT::WAIT);
        param.iData = 0;
    }

    if (curFrame > 10) {
        SetStateGrade(eAccessGrade::BASIC);
        param.iData = 0;
    }
}

void ER_ActionScript_Hyunwoo::Skill_QExit(tFSMData& param)
{
}

void ER_ActionScript_Hyunwoo::Skill_WEnter(tFSMData& param)
{
}

void ER_ActionScript_Hyunwoo::Skill_WUpdate(tFSMData& param)
{
}

void ER_ActionScript_Hyunwoo::Skill_WExit(tFSMData& param)
{
}

void ER_ActionScript_Hyunwoo::Skill_EEnter(tFSMData& param)
{
    CAnimator3D* Animator = GetOwner()->Animator3D();
    Animator->SelectAnimation(L"Hyunwoo_SkillE_Start", false);

    SetRotationToTarget(param.v4Data);

    // 방향 저장
    Vec3 vPos = GetOwner()->Transform()->GetRelativePos();
    Vec3 vDir = (param.v4Data - vPos).Normalize();

    // param.v4Data 에 받아온 방향정보를 v2Data로 이동
    param.v2Data.x = vDir.x;
    param.v2Data.y = vDir.z;

    param.v4Data[0] = 30.f;                                              // 스킬 거리
    float ClearDist = GetClearDistanceByWall(vDir, param.v4Data[0]);
    param.v4Data[1] = ClearDist;                                        // 이동 가능 거리
    param.v4Data[2] = Animator->GetCurAnim()->GetAnimClip().dEndTime;   // 전체 애니메이션 재생 시간
    param.v4Data[3] = 0.f;                                              // 이동한 거리 초기화.

    SetStateGrade(eAccessGrade::ADVANCED);
}

void ER_ActionScript_Hyunwoo::Skill_EUpdate(tFSMData& param)
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
            speed = param.v4Data[0] * 1.5;

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
        param.iData = 0;
        ChangeState(ER_CHAR_ACT::WAIT);
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

void ER_ActionScript_Hyunwoo::Skill_EExit(tFSMData& param)
{
}

void ER_ActionScript_Hyunwoo::Skill_REnter(tFSMData& param)
{
    CAnimator3D* Animator = GetOwner()->Animator3D();
    Animator->SelectAnimation(L"Hyunwoo_SkillR_Start", false);

    SetRotationToTarget(param.v4Data);

    param.iData = 0;

    SetStateGrade(eAccessGrade::DISABLE);
}

void ER_ActionScript_Hyunwoo::Skill_RUpdate(tFSMData& param)
{
    CAnimator3D* Animator = GetOwner()->Animator3D();
    int curFrame = GetOwner()->Animator3D()->GetCurFrame();

    if (KEY_PRESSED(KEY::R)) {
        if (Animator->IsFinish()) {
            //Start가 끝났을때,
            if (param.iData == 0) {
                Animator->SelectAnimation(L"Hyunwoo_SkillR_Loop", false);
            }
            else if (param.iData == 1) {
                Animator->SelectAnimation(L"Hyunwoo_SkillR_End", false);
            }
            ++param.iData;
        }
    }
    else {
        if (param.iData != 2) {
            Animator->SelectAnimation(L"Hyunwoo_SkillR_End", false);
            param.iData = 2;
        }
    }

    if (param.iData == 2 && curFrame > 16) {
        SetStateGrade(eAccessGrade::BASIC);
    }

    if (param.iData == 2 && Animator->IsFinish()) {
        SetStateGrade(eAccessGrade::BASIC);
        param.iData = 0;

        ChangeState(ER_CHAR_ACT::WAIT);
    }
}

void ER_ActionScript_Hyunwoo::Skill_RExit(tFSMData& param)
{
}

void ER_ActionScript_Hyunwoo::DeadEnter(tFSMData& param)
{
    GetOwner()->Animator3D()->SelectAnimation(L"Hyunwoo_Death", false);
}

void ER_ActionScript_Hyunwoo::DeadUpdate(tFSMData& param)
{
}

void ER_ActionScript_Hyunwoo::DeadExit(tFSMData& param)
{
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

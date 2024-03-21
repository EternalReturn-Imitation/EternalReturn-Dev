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

FSMState* ER_ActionScript_Jackie::CreateWait()
{
    FSMState* state = new FSMState(this);

    STATEDELEGATE_ENTER(state, Jackie, Wait);

    return state;
}

FSMState* ER_ActionScript_Jackie::CreateMove()
{
    FSMState* state = new FSMState(this);

    STATEDELEGATE_ENTER(state, Jackie, Move);
    STATEDELEGATE_UPDATE(state, Jackie, Move);

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

    return state;
}

FSMState* ER_ActionScript_Jackie::CreateRest()
{
    FSMState* state = new FSMState(this);

    STATEDELEGATE_ENTER(state, Jackie, Rest);
    STATEDELEGATE_UPDATE(state, Jackie, Rest);

    return state;
}

FSMState* ER_ActionScript_Jackie::CreateAttack()
{
    FSMState* state = new FSMState(this);

    return state;
}

FSMState* ER_ActionScript_Jackie::CreateArrive()
{
    FSMState* state = new FSMState(this);

    STATEDELEGATE_ENTER(state, Jackie, Arrive);
    STATEDELEGATE_UPDATE(state, Jackie, Arrive);

    return state;
}

FSMState* ER_ActionScript_Jackie::CreateDeath()
{
    FSMState* state = new FSMState(this);

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

void ER_ActionScript_Jackie::tick()
{
    // W스킬 지속시간 체크
    {
        tFSMData skillW = STATEDATA_GET(SKILL_W);
        tFSMData skillR = STATEDATA_GET(SKILL_R);

        if (skillW.iData)
        {
            skillW.fData -= DT;
            
            // W스킬 지속시간이 끝난 경우
            if (skillW.fData <= 0)
            {
                skillW.iData = 0;
                skillW.fData = 0.f;
                
                // 스킬 지속시간이 끝났을 때 MOVE 상태라면 이동모션 변경
                if(m_iCurState == ER_CHAR_ACT::MOVE)
                    GetOwner()->Animator3D()->SelectAnimation(L"Jackie_Run", true);
            }

            STATEDATA_SET(SKILL_W, skillW);
        }

        if (skillR.iData)
        {
            skillR.fData -= DT;

            // W스킬 지속시간이 끝난 경우
            if (skillR.fData <= 0)
            {
                skillR.iData = 0;
                skillR.fData = 0.f;

                // 스킬 지속시간이 끝났을 때 MOVE 상태라면 이동모션 변경
                if (m_iCurState == ER_CHAR_ACT::MOVE)
                    GetOwner()->Animator3D()->SelectAnimation(L"Jackie_Run", true);
            }

            STATEDATA_SET(SKILL_R, skillR);
        }


    }

    ER_ActionScript_Character::tick();
}

void ER_ActionScript_Jackie::Attack(tFSMData& _Data)
{
}

void ER_ActionScript_Jackie::Wait(tFSMData& _Data)
{
    ChangeState(ER_CHAR_ACT::WAIT);
}

void ER_ActionScript_Jackie::Move(tFSMData& _Data)
{
    STATEDATA_SET(MOVE, _Data);

    ER_ActionScript_Character::Move(_Data);
}

void ER_ActionScript_Jackie::Farming(tFSMData& _Data)
{
    STATEDATA_SET(FARMING, _Data);

    ChangeState(ER_CHAR_ACT::FARMING);
}

void ER_ActionScript_Jackie::Craft(tFSMData& _Data)
{
}

void ER_ActionScript_Jackie::Rest(tFSMData& _Data)
{
    ChangeState(ER_CHAR_ACT::REST);
}

void ER_ActionScript_Jackie::Skill_Q(tFSMData& _Data)
{
    ChangeState(ER_CHAR_ACT::SKILL_Q);
}

void ER_ActionScript_Jackie::Skill_W(tFSMData& _Data)
{
    ChangeState(ER_CHAR_ACT::SKILL_W);
}

void ER_ActionScript_Jackie::Skill_E(tFSMData& _Data)
{
    // 시전중이 아니라면
    if (!STATEDATA_GET(SKILL_E).iData)
        STATEDATA_SET(SKILL_E, _Data);

    ChangeState(ER_CHAR_ACT::SKILL_E);
}

void ER_ActionScript_Jackie::Skill_R(tFSMData& _Data)
{
    ChangeState(ER_CHAR_ACT::SKILL_R);
}

void ER_ActionScript_Jackie::MoveEnter(tFSMData& param)
{
    if(0 == STATEDATA_GET(SKILL_W).iData)
        GetOwner()->Animator3D()->SelectAnimation(L"Jackie_Run", true);
    else
        GetOwner()->Animator3D()->SelectAnimation(L"Jackie_SkillW", true);

    SetAbleToCancle(bAbleChange::COMMON);

    Vec3 DestPos = param.v4Data;

    CFindPath* findpathcomp = GetOwner()->FindPath();
    findpathcomp->FindPath(DestPos);
}

void ER_ActionScript_Jackie::MoveUpdate(tFSMData& param)
{
    // 캐릭터 속도 얻어와서 넣어주기
    float speed = m_Data->GetStatus()->fMovementSpeed;

    // W스킬 켜져있는경우 이동속도 증가
    if (1 == STATEDATA_GET(SKILL_W).iData)
        speed *= 1.3f;

    // 다음 이동지점이 없다면 대기상태로 전환
    if (!GetOwner()->FindPath()->PathMove(speed))
        ChangeState(ER_CHAR_ACT::WAIT);
}

void ER_ActionScript_Jackie::FarmingEnter(tFSMData& param)
{
    if (0 == STATEDATA_GET(SKILL_W).iData)
        GetOwner()->Animator3D()->SelectAnimation(L"Jackie_Run", true);
    else
        GetOwner()->Animator3D()->SelectAnimation(L"Jackie_SkillW", true);

    SetAbleToCancle(bAbleChange::COMMON);

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

void ER_ActionScript_Jackie::WaitEnter(tFSMData& param)
{
    GetOwner()->Animator3D()->SelectAnimation(L"Jackie_Wait", true);

    SetAbleToCancle(bAbleChange::COMMON);
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
            SetAbleToCancle(bAbleChange::COMMON);
            ChangeState(ER_CHAR_ACT::WAIT);
            param.iData = 0;
        }
        break;
    }
    }
}

void ER_ActionScript_Jackie::Skill_QEnter(tFSMData& param)
{
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
            SetAbleToCancle(bAbleChange::DISABLE);
            param.iData++;
        }
        break;
    }
    case 2:
    {

        // 특정 프레임에 공격 모션과 함께 전투 매니저 호출

        if (GetOwner()->Animator3D()->IsFinish())
            ChangeState(ER_CHAR_ACT::WAIT, bAbleChange::DISABLE);

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
    param.iData = 1;    // 스킬 ON
    param.fData = 3.f;  // 지속시간
}

void ER_ActionScript_Jackie::Skill_WUpdate(tFSMData& param)
{
    SetAbleToCancle(bAbleChange::COMMON);

    if (ER_CHAR_ACT::MOVE == m_iPrevState)
        ChangeState(ER_CHAR_ACT::MOVE);
    else
        ChangeState(ER_CHAR_ACT::WAIT);
}

void ER_ActionScript_Jackie::Skill_EEnter(tFSMData& param)
{
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
            SetAbleToCancle(bAbleChange::DISABLE);
            param.iData++;
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
            ChangeState(ER_CHAR_ACT::WAIT, bAbleChange::DISABLE);

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
    param.iData = 1;    // 스킬 ON
    param.fData = 5.f;  // 지속시간
}

void ER_ActionScript_Jackie::Skill_RUpdate(tFSMData& param)
{
}

void ER_ActionScript_Jackie::Skill_RExit(tFSMData& param)
{
}

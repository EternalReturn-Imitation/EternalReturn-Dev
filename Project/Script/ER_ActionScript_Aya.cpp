#include "pch.h"
#include "ER_ActionScript_Aya.h"
#include "ER_DataScript_Character.h"

#include "ER_UIMgr.h"
#include "ER_DataScript_ItemBox.h"
#include "ER_DataScript_Item.h"

#include <Engine/CAnim3D.h>

ER_ActionScript_Aya::ER_ActionScript_Aya()
    : ER_ActionScript_Character(SCRIPT_TYPE::ER_ACTIONSCRIPT_AYA)
    , m_fSec(0.f)
{
}

ER_ActionScript_Aya::~ER_ActionScript_Aya()
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

void ER_ActionScript_Aya::Attack(tFSMData& _Data)
{
    // 공격 추적상태가 아님
    if (!_Data.bData[0])
    {
        // 공격 추적상태 전환
        _Data.bData[0] = true;
    }

    STATEDATA_SET(ATTACK, _Data);

    CGameObject* TargetObj = (CGameObject*)_Data.lParam;
    float AtkRange = m_Data->GetStatus()->fAtakRange;
    if (IsInRange(TargetObj, AtkRange))
    {
        _Data.bData[0] = false;
        ChangeState(ER_CHAR_ACT::ATTACK);
    }
    else
    {
        // 사정거리 밖이기때문에 타겟방향으로 이동한다.
        tFSMData MoveData = STATEDATA_GET(MOVE);
        MoveData.v4Data = TargetObj->Transform()->GetRelativePos();
        Move(MoveData);
    }
}

void ER_ActionScript_Aya::Wait(tFSMData& _Data)
{
    ChangeState(ER_CHAR_ACT::WAIT);
}

void ER_ActionScript_Aya::Move(tFSMData& _Data)
{
    STATEDATA_SET(MOVE, _Data);

    ER_ActionScript_Character::Move(_Data);
}

void ER_ActionScript_Aya::Farming(tFSMData& _Data)
{
    STATEDATA_SET(FARMING, _Data);

    ChangeState(ER_CHAR_ACT::FARMING);
}

void ER_ActionScript_Aya::Craft(tFSMData& _Data)
{
    ChangeState(ER_CHAR_ACT::CRAFT);
}

void ER_ActionScript_Aya::Rest(tFSMData& _Data)
{
    ChangeState(ER_CHAR_ACT::REST);
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

    ChangeState(ER_CHAR_ACT::SKILL_E, bAbleChange::ABSOUTE);
}

void ER_ActionScript_Aya::Skill_R(tFSMData& _Data)
{
    ChangeState(ER_CHAR_ACT::SKILL_R);
}

void ER_ActionScript_Aya::MoveEnter(tFSMData& param)
{
    GetOwner()->Animator3D()->SelectAnimation(L"Aya_Run");
    
    SetAbleToCancle(bAbleChange::COMMON);

    Vec3 DestPos = param.v4Data;

    CFindPath* findpathcomp = GetOwner()->FindPath();
    bool bMove = findpathcomp->FindPath(DestPos);
}

void ER_ActionScript_Aya::MoveUpdate(tFSMData& param)
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
            ChangeState(ER_CHAR_ACT::ATTACK, bAbleChange::DISABLE);
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

void ER_ActionScript_Aya::MoveExit(tFSMData& param)
{

}

void ER_ActionScript_Aya::FarmingEnter(tFSMData& param)
{
    GetOwner()->Animator3D()->SelectAnimation(L"Aya_Run", true);

    SetAbleToCancle(bAbleChange::COMMON);

    Vec3 DestPos = param.v4Data;

    CFindPath* findpathcomp = GetOwner()->FindPath();
    findpathcomp->FindPath(((CGameObject*)param.lParam)->Transform()->GetRelativePos());

    m_pFarmingObject = (CGameObject*)param.lParam;

    m_bFarmingTrigger = true;
}

void ER_ActionScript_Aya::FarmingUpdate(tFSMData& param)
{
    float speed = m_Data->GetStatus()->fMovementSpeed;

    // 다음 이동지점이 없다면 대기상태로 전환
    if (!GetOwner()->FindPath()->PathMove(speed))
        GetOwner()->Animator3D()->SelectAnimation(L"Aya_Idle", true);

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

void ER_ActionScript_Aya::FarmingExit(tFSMData& param)
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

void ER_ActionScript_Aya::WaitEnter(tFSMData& param)
{
    GetOwner()->Animator3D()->SelectAnimation(L"Aya_Idle", true);

    SetAbleToCancle(bAbleChange::COMMON);
}

void ER_ActionScript_Aya::WaitUpdate(tFSMData& param)
{
}

void ER_ActionScript_Aya::WaitExit(tFSMData& param)
{
}

void ER_ActionScript_Aya::ArriveEnter(tFSMData& param)
{
    GetOwner()->Animator3D()->SelectAnimation(L"Aya_Arrive", false);
}

void ER_ActionScript_Aya::ArriveUpdate(tFSMData& param)
{
    if (GetOwner()->Animator3D()->IsFinish())
        ChangeState(ER_CHAR_ACT::WAIT);
}

void ER_ActionScript_Aya::ArriveExit(tFSMData& param)
{
}

void ER_ActionScript_Aya::AttackEnter(tFSMData& param)
{
    param.bData[0] = false;
    param.bData[2] = false;

    GetOwner()->Animator3D()->SelectAnimation(L"Aya_Attack", false);

    SetRotationToTarget(((CGameObject*)param.lParam)->Transform()->GetRelativePos());
}

void ER_ActionScript_Aya::AttackUpdate(tFSMData& param)
{
    // bData[0] : 공격 대상 추적상태
    // bData[1] : -
    // bData[2] : Hit판정 실행여부
    // bData[3] : - 

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

void ER_ActionScript_Aya::AttackExit(tFSMData& param)
{
}

void ER_ActionScript_Aya::CraftEnter(tFSMData& param)
{
    GetOwner()->Animator3D()->SelectAnimation(L"Aya_Craft", false);
}

void ER_ActionScript_Aya::CraftUpdate(tFSMData& param)
{
    if (GetOwner()->Animator3D()->IsFinish())
    {
        // 아이탬 생성함수
        ChangeState(ER_CHAR_ACT::WAIT);
    }
}

void ER_ActionScript_Aya::CraftExit(tFSMData& param)
{

}

void ER_ActionScript_Aya::RestEnter(tFSMData& param)
{
    GetOwner()->Animator3D()->SelectAnimation(L"Aya_Rest_Start", false);

    param.iData = 0;
}

void ER_ActionScript_Aya::RestUpdate(tFSMData& param)
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
            animator->SelectAnimation(L"Aya_Rest_Loop");

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
            animator->SelectAnimation(L"Aya_Rest_End", false);
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

void ER_ActionScript_Aya::RestExit(tFSMData& param)
{
}

void ER_ActionScript_Aya::Skill_QEnter(tFSMData& param)
{
    GetOwner()->Animator3D()->SelectAnimation(L"Aya_SkillQ", false);
    SetRotationToTarget(param.v4Data);
    SetAbleToCancle(bAbleChange::DISABLE);
}

void ER_ActionScript_Aya::Skill_QUpdate(tFSMData& param)
{
    CAnimator3D* animator = GetOwner()->Animator3D();

    int curFrame = animator->GetCurFrame();

    if (animator->IsFinish())
    {
        SetAbleToCancle(bAbleChange::COMMON);
        ChangeState(ER_CHAR_ACT::WAIT);
        param.iData = 0;
    }

    if (curFrame > 16) {
        SetAbleToCancle(bAbleChange::COMMON);
        param.iData = 0;
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

    param.iData = 0;
    param.fData = 0.f;

    param.bData[0] = false;

    SetAbleToCancle(bAbleChange::ABSOUTE);
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
        float angleDeg = angleRad * (180.0 / XM_PI);

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
            if (param.iData != 1) {
                Animator->SelectAnimation(L"Aya_SkillW_Forward", true);
                param.iData = 1;
            }
        }
        else if (param.fData > 45 && param.fData <= 135) {
            if (param.iData != 2) {
                Animator->SelectAnimation(L"Aya_SkillW_Left", true);
                param.iData = 2;
            }
        }
        else if (param.fData > -135 && param.fData <= -45) {
            if (param.iData != 3) {
                Animator->SelectAnimation(L"Aya_SkillW_Right", true);
                param.iData = 3;
            }
        }
        else {
            if (param.iData != 4) {
                Animator->SelectAnimation(L"Aya_SkillW_Back", true);
                param.iData = 4;
            }
        }
        param.bData[0] = false;
    }
    //같으면 그냥 쏨.
    else if((abs(vPos.x - param.v4Data.x) < 0.1f && abs(vPos.z - param.v4Data.z) < 0.1f)){
        Animator->SelectAnimation(L"Aya_SkillW_Shot", false);
        param.iData = 0;
    }

    //목적지 위치랑 현재 위치랑 같지 않을 때, 계속 이동함.
    if (vPos != param.v4Data) {
        float speed = m_Data->GetStatus()->fMovementSpeed;
        GetOwner()->FindPath()->PathMove(speed, false);
    }

    if (m_fSec > 1.5f) {
        SetAbleToCancle(bAbleChange::COMMON);
        ChangeState(ER_CHAR_ACT::WAIT);
        param.iData = 0;
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

    SetAbleToCancle(bAbleChange::ABSOUTE);
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
        SetAbleToCancle(bAbleChange::COMMON);
        param.v4Data = Vec4();
        param.v4Data = Vec2();
        param.iData = 0;
    }

    if (GetOwner()->Animator3D()->IsFinish())
    {
        SetAbleToCancle(bAbleChange::COMMON);
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
    switch (param.iData)
    {
    //시전 중 캐슬불가
    case 0:
    {
        animator->SelectAnimation(L"Aya_SkillR_Start", false);
        SetAbleToCancle(bAbleChange::DISABLE);
        ++param.iData;
    }
        break;
    //후딜 캐슬 가능
    case 1:
    {
        if (animator->IsFinish())
        {
            animator->SelectAnimation(L"Aya_SkillR_End", false);
            SetAbleToCancle(bAbleChange::COMMON);
            ++param.iData;
            param.iData = 0;
        }
    }
        break;
    case 2:
    {
        if (animator->IsFinish())
        {
            SetAbleToCancle(bAbleChange::COMMON);
            ChangeState(ER_CHAR_ACT::WAIT);
            param.iData=0;
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

void ER_ActionScript_Aya::DeadEnter(tFSMData& param)
{
    GetOwner()->Animator3D()->SelectAnimation(L"Aya_Death", false);
}

void ER_ActionScript_Aya::DeadUpdate(tFSMData& param)
{
}

void ER_ActionScript_Aya::DeadExit(tFSMData& param)
{
}

#include "pch.h"
#include "CUIComponent.h"

#include "CTransform.h"
#include "CKeyMgr.h"

#include "CScript.h"

CUIComponent::CUIComponent()
    : CComponent(COMPONENT_TYPE::UICOMPONENT)
    , m_bCsrPressed(false)
    , m_bCsrOn(false)
    , bOpen(false)
{
}

CUIComponent::CUIComponent(const CUIComponent& _Origin)
    : CComponent(COMPONENT_TYPE::UICOMPONENT)
    , m_bCsrPressed(false)
    , m_bCsrOn(false)
    , bOpen(false)
{
}

CUIComponent::~CUIComponent()
{
}

void CUIComponent::CsrOnCheck()
{
    // 사각 충돌 : OBB 응용
    Matrix WolrdMat = Transform()->GetWorldMat();
    Matrix WorldScaleMat = Transform()->GetWorldScaleMat();


    Vec2 UIPos(WolrdMat._41, WolrdMat._42);
    Vec2 UIScale(
        sqrt(WorldScaleMat._11 * WorldScaleMat._11 + WorldScaleMat._12 * WorldScaleMat._12 + WorldScaleMat._13 * WorldScaleMat._13),
        sqrt(WorldScaleMat._21 * WorldScaleMat._21 + WorldScaleMat._22 * WorldScaleMat._22 + WorldScaleMat._23 * WorldScaleMat._23));

    Vec2 MouseScale = { 1.f,1.f };
    Vec2 MousePos = CKeyMgr::GetInst()->GetUnProjectPos();
    Vec2 CenterDir = UIPos - MousePos; // 두 객체의 중심끼리의 거리벡터
    Vec2 Axis; // 기준 투영축
    float CenterProjDist; // 투영축 기준으로 두 중심점 사이의 거리 스칼라
    float r1, r2; // 비교 대상인 두 벡터의 투영길이

    //  1. Rect의 right축 투영
    Axis = Transform()->GetWorldDir(DIR_TYPE::RIGHT);
    CenterProjDist = abs(CenterDir.Dot(Axis));

    r1 = UIScale.x / 2.f;
    r2 = MouseScale.x / 2.f;

    if (r1 + r2 < CenterProjDist)
    {
        m_bCsrOn = false;
        return;
    }

    // 2. Rect의 up축 투영
    Axis = Transform()->GetWorldDir(DIR_TYPE::UP);
    CenterProjDist = abs(CenterDir.Dot(Axis));

    r1 = UIScale.y / 2.f;
    r2 = MouseScale.y / 2.f;

    if (r1 + r2 < CenterProjDist)
    {
        m_bCsrOn = false;
        return;
    }

    m_bCsrOn = true;
}

void CUIComponent::finaltick()
{
    if (!GetOwner()->IsEnable())
    {
        m_bCsrOn = false;
        m_bPrevCsrOn = false;
        return;
    }

    CsrOnCheck();

    if (m_bCsrOn && !m_bPrevCsrOn)
    {
        CsrBeginOn();
        m_bPrevCsrOn = true;
    }
    else if (m_bPrevCsrOn && !m_bCsrOn)
    {
        CsrAway();
        m_bPrevCsrOn = false;
    }
}

void CUIComponent::CsrOn()
{
    // DEBUG_LOG_COLLISION(ToString(GetOwner()->GetName()).c_str(), "CsrOn", "");
    // Script 호출
    const vector<CScript*>& vecScript = GetOwner()->GetScripts();
    for (size_t i = 0; i < vecScript.size(); ++i)
    {
        vecScript[i]->CsrOn();
    }
}

void CUIComponent::CsrTap()
{
    DEBUG_LOG_COLLISION(ToString(GetOwner()->GetName()).c_str(), "CsrTap", "");
    m_bCsrPressed = true;  
    // Script 호출
    const vector<CScript*>& vecScript = GetOwner()->GetScripts();
    for (size_t i = 0; i < vecScript.size(); ++i)
    {
        vecScript[i]->CsrTap();
    }
}

void CUIComponent::CsrRelease()
{
    DEBUG_LOG_COLLISION(ToString(GetOwner()->GetName()).c_str(), "CsrRelease", "");
    m_bCsrPressed = false; 
    // Script 호출
    const vector<CScript*>& vecScript = GetOwner()->GetScripts();
    for (size_t i = 0; i < vecScript.size(); ++i)
    {
        vecScript[i]->CsrRelease();
    }
}

void CUIComponent::CsrClick()
{
    DEBUG_LOG_COLLISION(ToString(GetOwner()->GetName()).c_str(), "CsrClick", "");
    // Script 호출
    const vector<CScript*>& vecScript = GetOwner()->GetScripts();
    for (size_t i = 0; i < vecScript.size(); ++i)
    {
        vecScript[i]->CsrClick();
    }
}

void CUIComponent::CsrBeginOn()
{
    DEBUG_LOG_COLLISION(ToString(GetOwner()->GetName()).c_str(), "CsrBeginOn", "");
    // Script 호출
    const vector<CScript*>& vecScript = GetOwner()->GetScripts();
    for (size_t i = 0; i < vecScript.size(); ++i)
    {
        vecScript[i]->CsrBeginOn();
    }
}

void CUIComponent::CsrAway()
{
    DEBUG_LOG_COLLISION(ToString(GetOwner()->GetName()).c_str(), "CsrAway", "");
    // Script 호출
    const vector<CScript*>& vecScript = GetOwner()->GetScripts();
    for (size_t i = 0; i < vecScript.size(); ++i)
    {
        vecScript[i]->CsrAway();
    }
}

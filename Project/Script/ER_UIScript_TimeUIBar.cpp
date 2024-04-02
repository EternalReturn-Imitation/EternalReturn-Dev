#include "pch.h"
#include "ER_UIScript_TimeUIBar.h"

#include "ER_GameSystem.h"

ER_UIScript_TimeUIBar::ER_UIScript_TimeUIBar()
	: CScript((UINT)SCRIPT_TYPE::ER_UISCRIPT_TIMEUIBAR)
	, m_pClockIcon(nullptr)
	, m_fGameTime(nullptr)
	, m_iMinute(0)
	, m_iSecond(0)
{
}

ER_UIScript_TimeUIBar::~ER_UIScript_TimeUIBar()
{
}

void ER_UIScript_TimeUIBar::init()
{
	GetOwner()->SetName(L"UI_TimeBar");
	AddComponents(GetOwner(), _TRANSFORM | _MESHRENDER | _TEXT);

	MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"2DUISTD_Mtrl"), 0);
	MeshRender()->GetDynamicMaterial(0);

	MeshRender()->GetMaterial(0)->SetTexParam(TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"Time_UI_Bg.png"));
	GetOwner()->LoadAllPrefabFromObjName();

	Text()->SetReference(0, (UINT)CText::eTextRefType::INT, (DWORD_PTR)&m_iMinute);
	Text()->SetReference(2, (UINT)CText::eTextRefType::INT, (DWORD_PTR)&m_iSecond);
	m_iMinute = -1;

	m_pClockIcon = onew(CGameObject);
	m_pClockIcon->SetName(L"UI_TimeBarIcon");
	AddComponents(m_pClockIcon, _TRANSFORM | _MESHRENDER);

	m_pClockIcon->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	m_pClockIcon->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"2DUISTD_Mtrl"), 0);
	m_pClockIcon->MeshRender()->GetDynamicMaterial(0);

	m_pClockIcon->MeshRender()->GetMaterial(0)->SetTexParam(TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"UI_Clock.png"));
	m_pClockIcon->LoadAllPrefabFromObjName();

	GetOwner()->AddChild(m_pClockIcon);

	m_fGameTime = ER_GameSystem::GetInst()->GetGameTime();
}

void ER_UIScript_TimeUIBar::begin()
{
}

void ER_UIScript_TimeUIBar::tick()
{
	m_iMinute = ((int)*m_fGameTime) / 60;
	m_iSecond = ((int)*m_fGameTime) % 60;
}

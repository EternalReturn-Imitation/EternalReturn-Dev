#include "pch.h"
#include "ER_UIScript_SkillUpBtn.h"

#include "ER_GameSystem.h"

#include <Engine\CGameObject.h>
#include <Engine\CResMgr.h>

ER_UIScript_SkillUpBtn::ER_UIScript_SkillUpBtn()
	: CScript((UINT)SCRIPT_TYPE::ER_UISCRIPT_SKILLUPBTN)
	, m_SkillPoint(nullptr)
	, m_CsrState(0)
	, m_Slot(nullptr)
	, m_SkillUpFunc(nullptr)
{
}

ER_UIScript_SkillUpBtn::~ER_UIScript_SkillUpBtn()
{
}

void ER_UIScript_SkillUpBtn::SetUI(int i)
{
	AddComponents(GetOwner(), _TRANSFORM | _MESHRENDER | _UICOMPONENT);
	wstring BtnName = L"UI_StatusBar_SkillUP_Btn";
	BtnName += std::to_wstring(i);
	GetOwner()->SetName(BtnName);

	// MeshRender Setting
	MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"2DUIIndicator_Mtrl"), 0);
	MeshRender()->GetDynamicMaterial(0);

	MeshRender()->GetMaterial(0)->SetTexParam(TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"Btn_LevelUp_Basic.png"));
	MeshRender()->GetMaterial(0)->SetTexParam(TEX_1, CResMgr::GetInst()->FindRes<CTexture>(L"Btn_LevelUp_MouseOver.png"));
	MeshRender()->GetMaterial(0)->SetTexParam(TEX_2, CResMgr::GetInst()->FindRes<CTexture>(L"Btn_LevelUp_Disabled.png"));

	Transform()->LoadPrefab(BtnName);
}

void ER_UIScript_SkillUpBtn::begin()
{
}

void ER_UIScript_SkillUpBtn::tick()
{
	// 스킬포인트가 없는경우 비활성화
	if (*m_SkillPoint < 1)
		GetOwner()->SetEnable(false);
}

void ER_UIScript_SkillUpBtn::CsrOn()
{
}

void ER_UIScript_SkillUpBtn::CsrTap()
{
	if (1 <= *m_SkillPoint)
	{
		m_CsrState = 2;
		MeshRender()->GetMaterial(0)->SetScalarParam(INT_0, &m_CsrState);
	}
}

void ER_UIScript_SkillUpBtn::CsrRelease()
{
	if (1 <= *m_SkillPoint)
	{
		m_CsrState = 1;
		MeshRender()->GetMaterial(0)->SetScalarParam(INT_0, &m_CsrState);
	}
}

void ER_UIScript_SkillUpBtn::CsrClick()
{
	if (m_Slot && m_SkillUpFunc)
		(m_Slot->*m_SkillUpFunc)();

	if (*m_SkillPoint < 1)
	{
		m_CsrState = 0;
		MeshRender()->GetMaterial(0)->SetScalarParam(INT_0, &m_CsrState);
	}
}

void ER_UIScript_SkillUpBtn::CsrBeginOn()
{
	if (1 <= *m_SkillPoint)
	{
		m_CsrState = 1;
		MeshRender()->GetMaterial(0)->SetScalarParam(INT_0, &m_CsrState);
	}
}

void ER_UIScript_SkillUpBtn::CsrAway()
{
	m_CsrState = 0;
	MeshRender()->GetMaterial(0)->SetScalarParam(INT_0, &m_CsrState);
}

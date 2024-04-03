#include "pch.h"
#include "ER_UIScript_SkillSlot.h"

#include "ER_struct.h"

#include "ER_GameSystem.h"
#include "ER_DataScript_Character.h"
#include "ER_UIScript_SkillUpBtn.h"

#include <Engine\CGameObject.h>
#include <Engine\CKeyMgr.h>

ER_UIScript_SkillSlot::ER_UIScript_SkillSlot()
	: CScript((UINT)SCRIPT_TYPE::ER_UISCRIPT_SKILLSLOT)
	, m_Data(nullptr)
	, m_SlotIdx(SKILLIDX::SKILLMAXSIZE)
	, m_Equipskill(nullptr)
	, m_Skill1(nullptr)
	, m_Skill2(nullptr)
	, m_SkillPoint(nullptr)
	, m_SkillUPBtn(nullptr)
	, SkillLevelIndicator(nullptr)
	, m_MaxSkillLv(0)
	, m_CurSkillLv(0)
	, m_CurSkillIdx(0)
	, m_bToggle(false)
{
}

ER_UIScript_SkillSlot::~ER_UIScript_SkillSlot()
{

}

void ER_UIScript_SkillSlot::RegistPlayerCharacetr()
{
	m_Data = ER_GameSystem::GetInst()->GetPlayerCharacter()->GetScript<ER_DataScript_Character>();
	m_Skill1 = m_Data->GetSkillAdress((UINT)m_SlotIdx);
	m_Skill2 = m_Data->GetSkillAdress((UINT)m_SlotIdx + 4);
	m_SkillPoint = m_Data->GetSkillPoint();
	m_SkillUPBtn->GetScript<ER_UIScript_SkillUpBtn>()->SetSkillPiont(m_SkillPoint);
	m_SkillUPBtn->GetScript<ER_UIScript_SkillUpBtn>()->SetSkill(this, (SKILLUP)&ER_UIScript_SkillSlot::SkillLevelUp);
	
	int skillidx = (UINT)m_SlotIdx;
	
	MeshRender()->GetMaterial(0)->SetTexParam(TEX_0, m_Data->GetSkillList()[skillidx]->TexSkill);
	MeshRender()->GetMaterial(0)->SetTexParam(TEX_1, m_Data->GetSkillList()[skillidx + 4]->TexSkill);

	MeshRender()->GetMaterial(0)->SetScalarParam(INT_0, &m_CurSkillIdx);

	m_Equipskill = (*m_Skill1);
}

void ER_UIScript_SkillSlot::begin()
{
	Text()->SetReference(1, 1, (DWORD_PTR)&m_Equipskill->iCurUseSP);
	SkillLevelIndicator->MeshRender()->GetMaterial(0)->SetScalarParam(INT_0, &m_Equipskill->iSkillLevel);

	// float cool = 0.5f;
	// MeshRender()->GetMaterial(0)->SetScalarParam(FLOAT_0, &cool);
}

void ER_UIScript_SkillSlot::tick()
{
	float CoolRatio = m_Equipskill->CurCooldown() / m_Equipskill->MaxCooldown();
	MeshRender()->GetMaterial(0)->SetScalarParam(FLOAT_0, &CoolRatio);


	if (m_Equipskill->iSkillLevel < m_Equipskill->iMaxSkillLevel && 0 < *m_SkillPoint)
		m_SkillUPBtn->SetEnable(true);
	else if(m_Equipskill->iMaxSkillLevel <= m_Equipskill->iSkillLevel)
		m_SkillUPBtn->SetEnable(false);
}

void ER_UIScript_SkillSlot::SetToggle()
{
	m_bToggle = !m_bToggle;

	// Set Skill 1
	if (!m_bToggle)
	{
		m_Equipskill = (*m_Skill1);
		m_CurSkillIdx = 0;
		MeshRender()->GetMaterial(0)->SetScalarParam(INT_0, &m_CurSkillIdx);
	}
	// Set Skill 2
	else
	{
		m_Equipskill = (*m_Skill2);
		m_CurSkillIdx = 1;
		MeshRender()->GetMaterial(0)->SetScalarParam(INT_0, &m_CurSkillIdx);
	}
	
	Text()->SetReference(1, 1, (DWORD_PTR)&m_Equipskill->iCurUseSP);
	SkillLevelIndicator->MeshRender()->GetMaterial(0)->SetScalarParam(INT_0, &m_Equipskill->iSkillLevel);
}

void ER_UIScript_SkillSlot::SetUI(int i, CGameObject* UpBtn)
{
	
	m_SlotIdx = (SKILLIDX)i;
	
	AddComponents(GetOwner(), _TRANSFORM | _MESHRENDER | _TEXT | _UICOMPONENT);
	wstring SlotName = L"UI_StatusBar_SkillSlot_";
	SlotName += std::to_wstring(i);
	GetOwner()->SetName(SlotName);

	// MeshRender Setting
	MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"2DUICoolDown_Mtrl"), 0);
	MeshRender()->GetDynamicMaterial(0);

	GetOwner()->LoadAllPrefabFromObjName();

	// 레벨인디케이터
	SkillLevelIndicator = new CGameObject;
	AddComponents(SkillLevelIndicator, _TRANSFORM | _MESHRENDER);
	SlotName = L"UI_StatusBar_SkillLevel_";
	SlotName += std::to_wstring(i);
	SkillLevelIndicator->SetName(SlotName);

	// MeshRender Setting
	SkillLevelIndicator->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	SkillLevelIndicator->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"2DUIIndicator_Mtrl"), 0);
	SkillLevelIndicator->MeshRender()->GetDynamicMaterial(0);
	
	if (m_SlotIdx == SKILLIDX::R_1)
	{
		m_MaxSkillLv = 3;
		SkillLevelIndicator->MeshRender()->GetMaterial(0)->SetTexParam(TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"UI_SkillLevelBg_Three.png"));
		SkillLevelIndicator->MeshRender()->GetMaterial(0)->SetTexParam(TEX_1, CResMgr::GetInst()->FindRes<CTexture>(L"UI_SkillLevelBg_Three_LV1.png"));
		SkillLevelIndicator->MeshRender()->GetMaterial(0)->SetTexParam(TEX_2, CResMgr::GetInst()->FindRes<CTexture>(L"UI_SkillLevelBg_Three_LV2.png"));
		SkillLevelIndicator->MeshRender()->GetMaterial(0)->SetTexParam(TEX_3, CResMgr::GetInst()->FindRes<CTexture>(L"UI_SkillLevelBg_Three_LV3.png"));
	}
	else
	{
		m_MaxSkillLv = 5;
		SkillLevelIndicator->MeshRender()->GetMaterial(0)->SetTexParam(TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"UI_SkillLevelBg_Five.png"));
		SkillLevelIndicator->MeshRender()->GetMaterial(0)->SetTexParam(TEX_1, CResMgr::GetInst()->FindRes<CTexture>(L"UI_SkillLevelBg_Five_LV1.png"));
		SkillLevelIndicator->MeshRender()->GetMaterial(0)->SetTexParam(TEX_2, CResMgr::GetInst()->FindRes<CTexture>(L"UI_SkillLevelBg_Five_LV2.png"));
		SkillLevelIndicator->MeshRender()->GetMaterial(0)->SetTexParam(TEX_3, CResMgr::GetInst()->FindRes<CTexture>(L"UI_SkillLevelBg_Five_LV3.png"));
		SkillLevelIndicator->MeshRender()->GetMaterial(0)->SetTexParam(TEX_4, CResMgr::GetInst()->FindRes<CTexture>(L"UI_SkillLevelBg_Five_LV4.png"));
		SkillLevelIndicator->MeshRender()->GetMaterial(0)->SetTexParam(TEX_5, CResMgr::GetInst()->FindRes<CTexture>(L"UI_SkillLevelBg_Five_LV5.png"));
	}
	
	SkillLevelIndicator->LoadAllPrefabFromObjName();

	m_SkillUPBtn = UpBtn;

	GetOwner()->AddChild(SkillLevelIndicator);
}

void ER_UIScript_SkillSlot::SkillLevelUp()
{
	if (m_Equipskill->iSkillLevel < m_Equipskill->iMaxSkillLevel)
	{
		(*m_Skill1)->iSkillLevel++;
		(*m_Skill2)->iSkillLevel++;
		(*m_SkillPoint) -= 1;
	}

	SkillLevelIndicator->MeshRender()->GetMaterial(0)->SetScalarParam(INT_0, &m_Equipskill->iSkillLevel);
}

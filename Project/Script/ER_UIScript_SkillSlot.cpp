#include "pch.h"
#include "ER_UIScript_SkillSlot.h"

#include <Engine\CGameObject.h>

ER_UIScript_SkillSlot::ER_UIScript_SkillSlot()
	: CScript((UINT)SCRIPT_TYPE::ER_UISCRIPT_SKILLSLOT)
	, m_Data(nullptr)
	, m_SlotIdx(SKILLIDX::SKILLMAXSIZE)
	, SkillLevelIndicator(nullptr)
{
}

ER_UIScript_SkillSlot::~ER_UIScript_SkillSlot()
{

}
void ER_UIScript_SkillSlot::begin()
{
}

void ER_UIScript_SkillSlot::tick()
{
}

void ER_UIScript_SkillSlot::SetUI(int i)
{
	m_SlotIdx = (SKILLIDX)i;
	
	AddComponents(GetOwner(), _TRANSFORM | _MESHRENDER | _TEXT | _UICOMPONENT);
	wstring SlotName = L"UI_StatusBar_SkillSlot_";
	SlotName += std::to_wstring(i);
	GetOwner()->SetName(SlotName);

	// MeshRender Setting
	MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std2DUIMtrl"), 0);
	MeshRender()->GetDynamicMaterial(0);
	MeshRender()->GetMaterial(0)->SetTexParam(TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"Img_Skill_BasicSlot.png"));

	GetOwner()->LoadAllPrefabFromObjName();

	// 레벨인디케이터
	SkillLevelIndicator = new CGameObject;
	AddComponents(SkillLevelIndicator, _TRANSFORM | _MESHRENDER);
	SlotName = L"UI_StatusBar_SkillLevel_";
	SlotName += std::to_wstring(i);
	SkillLevelIndicator->SetName(SlotName);

	// MeshRender Setting
	SkillLevelIndicator->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	SkillLevelIndicator->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std2DUIMtrl"), 0);
	SkillLevelIndicator->MeshRender()->GetDynamicMaterial(0);
	SkillLevelIndicator->MeshRender()->GetMaterial(0)->SetTexParam(TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"UI_SkillLevelBg_Five_LV5.png"));
	
	SkillLevelIndicator->LoadAllPrefabFromObjName();

	GetOwner()->AddChild(SkillLevelIndicator);
}

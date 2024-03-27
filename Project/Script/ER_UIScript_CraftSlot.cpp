#include "pch.h"
#include "ER_UIScript_CraftSlot.h"

#include "ER_UIMgr.h"
#include "ER_ItemMgr.h"
#include "ER_DataScript_Item.h"

#include "ER_GameSystem.h"
#include "ER_ActionScript_Character.h"
#include "ER_DataScript_Character.h"

ER_UIScript_CraftSlot::ER_UIScript_CraftSlot()
	: CScript((UINT)SCRIPT_TYPE::ER_UISCRIPT_CRAFTSLOT)
	, m_Idx(0)
{
}

ER_UIScript_CraftSlot::~ER_UIScript_CraftSlot()
{
}

void ER_UIScript_CraftSlot::init()
{
	AddComponents(GetOwner(), _TRANSFORM | _MESHRENDER | _UICOMPONENT);

	MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"2DUIItemSlot_Mtrl"), 0);
	MeshRender()->GetDynamicMaterial(0);
	MeshRender()->GetMaterial(0)->SetTexParam(TEX_1, CResMgr::GetInst()->FindRes<CTexture>(L"Ico_ItemGradebg_01.png"));
	MeshRender()->GetMaterial(0)->SetTexParam(TEX_2, CResMgr::GetInst()->FindRes<CTexture>(L"Ico_ItemGradebg_02.png"));
	MeshRender()->GetMaterial(0)->SetTexParam(TEX_3, CResMgr::GetInst()->FindRes<CTexture>(L"Ico_ItemGradebg_03.png"));
	MeshRender()->GetMaterial(0)->SetTexParam(TEX_4, CResMgr::GetInst()->FindRes<CTexture>(L"Ico_ItemGradebg_04.png"));

	GetOwner()->LoadAllPrefabFromObjName();

	int empty = 1;
	MeshRender()->GetMaterial(0)->SetScalarParam(INT_0, &empty);
}

void ER_UIScript_CraftSlot::tick()
{
	if (m_Idx < m_PlayerCraftList->size())
	{
		GetOwner()->SetEnable(true);
		if (m_CurItemID == (*m_PlayerCraftList)[m_Idx])
			return;

		m_CurItemID = (*m_PlayerCraftList)[m_Idx];

		ER_DataScript_Item* Item = ER_ItemMgr::GetInst()->GetItemObj(m_CurItemID)->GetScript<ER_DataScript_Item>();

		MeshRender()->GetMaterial(0)->SetTexParam(TEX_0, Item->GetItemTex().Get());
		int ItemGrade = Item->GetGrade();
		MeshRender()->GetMaterial(0)->SetScalarParam(INT_1, &ItemGrade);
	}
	else
		GetOwner()->SetEnable(false);
}

void ER_UIScript_CraftSlot::CsrOn()
{
}

void ER_UIScript_CraftSlot::CsrTap()
{
}

void ER_UIScript_CraftSlot::CsrRelease()
{
}

void ER_UIScript_CraftSlot::CsrClick()
{
	// 아아이템 아이디로 플레이어캐릭터스크립트가 가진 액션스크립트의 크래프트 아이템 호출
	tFSMData data = {};
	data.iData[0] = m_CurItemID;
	// ER_GameSystem::GetInst()->GetPlayerCharacter()->GetScript<ER_ActionScript_Character>()->Craft(data);
	ER_GameSystem::GetInst()->GetPlayerCharacter()->GetScript<ER_DataScript_Character>()->CraftItem(m_CurItemID);
}

void ER_UIScript_CraftSlot::CsrBeginOn()
{
}

void ER_UIScript_CraftSlot::CsrAway()
{
}

void ER_UIScript_CraftSlot::RegistCraftvector(const vector<UINT>* _vecCraftItem, UINT idx)
{
	m_PlayerCraftList = _vecCraftItem;
	m_Idx = idx;
}

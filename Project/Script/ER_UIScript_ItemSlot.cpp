#include "pch.h"
#include "ER_UIScript_ItemSlot.h"

#include "ER_DataScript_Item.h"

ER_UIScript_ItemSlot::ER_UIScript_ItemSlot()
	: CScript((UINT)SCRIPT_TYPE::ER_UISCRIPT_ITEMSLOT)
	, m_Slot(nullptr)
{
}

ER_UIScript_ItemSlot::~ER_UIScript_ItemSlot()
{
}

void ER_UIScript_ItemSlot::init()
{
	AddComponents(GetOwner(), _TRANSFORM | _MESHRENDER | _UICOMPONENT);

	MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"2DUISTD_Mtrl"), 0);
	MeshRender()->GetDynamicMaterial(0);
	MeshRender()->GetMaterial(0)->SetTexParam(TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"Ico_ItemGradebg_Empty.png"));
}

void ER_UIScript_ItemSlot::begin()
{
}

void ER_UIScript_ItemSlot::tick()
{
}

void ER_UIScript_ItemSlot::RegistSlotAdress(CGameObject** _ItemSlotAddress)
{
	m_Slot = _ItemSlotAddress;
}

void ER_UIScript_ItemSlot::CsrOn()
{
}

void ER_UIScript_ItemSlot::CsrTap()
{
}

void ER_UIScript_ItemSlot::CsrRelease()
{
}

void ER_UIScript_ItemSlot::CsrClick()
{
	if (m_Slot)
	{
		wstring name = (*m_Slot)->GetScript<ER_DataScript_Item>()->GetItemName();
		int a = 0;
	}
}

void ER_UIScript_ItemSlot::CsrBeginOn()
{
}

void ER_UIScript_ItemSlot::CsrAway()
{
}

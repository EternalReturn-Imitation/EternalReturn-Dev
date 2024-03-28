#include "pch.h"
#include "ER_UIScript_ItemSlot.h"

#include "ER_DataScript_Item.h"
#include "ER_DataScript_Character.h"
#include "ER_UIMgr.h"
#include "ER_GameSystem.h"

#include "ER_UIScript_ItemBox.h"

#include "ER_Cursor.h"

ER_UIScript_ItemSlot::ER_UIScript_ItemSlot()
	: CScript((UINT)SCRIPT_TYPE::ER_UISCRIPT_ITEMSLOT)
	, m_Slot(nullptr)
	, m_ItemData(nullptr)
	, m_CurItemID(-1)
	, m_PrevItemID(-1)
	, m_SlotType(eSlotType::COMMON)
{
}

ER_UIScript_ItemSlot::~ER_UIScript_ItemSlot()
{
}

void ER_UIScript_ItemSlot::init()
{
	AddComponents(GetOwner(), _TRANSFORM | _MESHRENDER | _UICOMPONENT);

	MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"2DUIItemSlot_Mtrl"), 0);
	MeshRender()->GetDynamicMaterial(0);
	MeshRender()->GetMaterial(0)->SetTexParam(TEX_1, CResMgr::GetInst()->FindRes<CTexture>(L"Ico_ItemGradebg_01.png"));
	MeshRender()->GetMaterial(0)->SetTexParam(TEX_2, CResMgr::GetInst()->FindRes<CTexture>(L"Ico_ItemGradebg_02.png"));
	MeshRender()->GetMaterial(0)->SetTexParam(TEX_3, CResMgr::GetInst()->FindRes<CTexture>(L"Ico_ItemGradebg_03.png"));
	MeshRender()->GetMaterial(0)->SetTexParam(TEX_4, CResMgr::GetInst()->FindRes<CTexture>(L"Ico_ItemGradebg_04.png"));
	MeshRender()->GetMaterial(0)->SetTexParam(TEX_5, CResMgr::GetInst()->FindRes<CTexture>(L"Ico_ItemGradebg_Empty.png"));
	
	int empty = 0;
	MeshRender()->GetMaterial(0)->SetScalarParam(INT_0, &empty);
}

void ER_UIScript_ItemSlot::begin()
{
}

void ER_UIScript_ItemSlot::tick()
{
	if (!(GetOwner()->GetParent()->IsEnable()))
		return;
	
	if ((*m_Slot))
	{
		m_ItemData = (*m_Slot)->GetScript<ER_DataScript_Item>();
		m_CurItemID = m_ItemData->GetCode();
	}
	else
	{
		m_ItemData = nullptr;
		m_CurItemID = -1;
	}

	// 이전과 아이디가 달라졌으므로 아이템이 변경되었다.
	if(m_CurItemID != m_PrevItemID)
	{
		int bEmpty = 0;
		// 아이템슬롯이 비었다.
		if (-1 == m_CurItemID)
		{
			MeshRender()->GetMaterial(0)->SetScalarParam(INT_0, &bEmpty);
		}
		else
		{
			bEmpty = 1;
			MeshRender()->GetMaterial(0)->SetScalarParam(INT_0, &bEmpty);

			int ItemGrade = m_ItemData->GetGrade();
			MeshRender()->GetMaterial(0)->SetScalarParam(INT_1, &ItemGrade);

			MeshRender()->GetMaterial(0)->SetTexParam(TEX_0, m_ItemData->GetItemTex().Get());
		}

		m_PrevItemID = m_CurItemID;
	}
}

void ER_UIScript_ItemSlot::RegistSlotAdress(CGameObject** _ItemSlotAddress, eSlotType _SlotType)
{
	m_Slot = _ItemSlotAddress;
	m_SlotType = _SlotType;
}

bool ER_UIScript_ItemSlot::IsItemInSlot()
{
	if((*m_Slot))
		return true;
	else
		return false;
}

void ER_UIScript_ItemSlot::CsrOn()
{
	if (KEY_RELEASE(KEY::LBTN))
	{
		ER_UIMgr::GetInst()->RegistDropItemSlot(this);
	}
	
	// 아이템이 있는경우의 마우스 오른쪽클리 동작
	if (!(KEY_PRESSED(KEY::LBTN)) && (*m_Slot) && KEY_TAP(KEY::RBTN))
	{
		ER_DataScript_Item* Item = (*m_Slot)->GetScript<ER_DataScript_Item>();

		ER_UIScript_ItemBox* ItemBox = GetOwner()->GetParent()->GetScript<ER_UIScript_ItemBox>();
		ER_DataScript_Character* PlayerCharacter = ER_GameSystem::GetInst()->GetPlayerCharacter()->GetScript<ER_DataScript_Character>();

		// Item Box
		if (ItemBox)
		{
			PlayerCharacter->AcquireItem(m_Slot);
		}
		// EquipMent
		else if(Item->IsEquiped())
		{
			// 필요 동작코드 동일
			PlayerCharacter->AcquireItem(m_Slot);
		}
		// Inventory
		else
		{
			PlayerCharacter->SwapItem(m_Slot, PlayerCharacter->GetEquipItem(Item->GetSlot()));
		}
	}
}

void ER_UIScript_ItemSlot::CsrTap()
{
	if ((*m_Slot) && !(KEY_PRESSED(KEY::RBTN)))
	{
		ER_UIMgr::GetInst()->RegistDragItemSlot(this);
		ER_GameSystem::GetInst()->GetCursor()->GetDragItemTex()->MeshRender()->SetMaterial(MeshRender()->GetMaterial(0), 0);
	}
}

void ER_UIScript_ItemSlot::CsrRelease()
{
	ER_GameSystem::GetInst()->GetCursor()->GetDragItemTex()->MeshRender()->SetMaterial(nullptr, 0);
}

void ER_UIScript_ItemSlot::CsrClick()
{
}

void ER_UIScript_ItemSlot::CsrBeginOn()
{
}

void ER_UIScript_ItemSlot::CsrAway()
{
}

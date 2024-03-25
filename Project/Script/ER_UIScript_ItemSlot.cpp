#include "pch.h"
#include "ER_UIScript_ItemSlot.h"

#include "ER_DataScript_Item.h"

ER_UIScript_ItemSlot::ER_UIScript_ItemSlot()
	: CScript((UINT)SCRIPT_TYPE::ER_UISCRIPT_ITEMSLOT)
	, m_Slot(nullptr)
	, m_ItemData(nullptr)
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

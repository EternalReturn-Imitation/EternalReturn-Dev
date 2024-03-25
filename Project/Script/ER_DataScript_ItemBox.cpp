#include "pch.h"
#include "ER_DataScript_ItemBox.h"

#include <Engine\CResMgr.h>
#include <Engine\CRenderMgr.h>
#include <Engine\CTexture.h>
#include <Engine\CDevice.h>
#include <Engine\define.h>

#include "ER_ItemMgr.h"
#include "ER_ActionScript_Character.h"
#include "ER_DataScript_Item.h"

#include "CUIScript_Button.h"

ER_DataScript_ItemBox::ER_DataScript_ItemBox()
	: CScript((UINT)SCRIPT_TYPE::ER_DATASCRIPT_ITEMBOX)
	, m_pItemList{}
	, m_pItemBoxUI(nullptr)
	, m_UIBoxTag(nullptr)
{
}

ER_DataScript_ItemBox::~ER_DataScript_ItemBox()
{
}

void ER_DataScript_ItemBox::init()
{
	CreateBoxUI();
}

void ER_DataScript_ItemBox::begin()
{
	if (GetOwner()->GetRenderComponent() != nullptr && GetOwner()->GetRenderComponent()->GetMaterial(0) != nullptr)
	{
		int a = 1;
		GetOwner()->GetRenderComponent()->GetMaterial(0)->SetScalarParam(INT_3, &a);
	}
}

void ER_DataScript_ItemBox::tick()
{
	// if (m_UIBoxTag) {
	// Vec3 resultPos = ER_UIMgr::GetInst()->WorldPosToUIPos(GetOwner()->Transform()->GetRelativePos());
	// m_pBoxIcon->Transform()->SetRelativePos(Vec3(resultPos.x, resultPos.y + 150.f, -1.1f));
	// }
	if (m_UIBoxTag)
	{
		Vec3 WorldPos = GetOwner()->Transform()->GetWorldPos();
		XMVECTOR pos = XMLoadFloat3(&WorldPos);

		// 뷰 매트릭스와 프로젝션 매트릭스 로드
		XMMATRIX view = CRenderMgr::GetInst()->GetMainCam()->GetViewMat();
		XMMATRIX proj = CRenderMgr::GetInst()->GetMainCam()->GetProjMat();

		Vec2 vResol = CDevice::GetInst()->GetRenderResolution();

		// 스크린 좌표로 변환
		XMVECTOR screenPos = XMVector3Project(pos, 0.0f, 0.0f, vResol.x, vResol.y, 0.0f, 1.0f, proj, view, XMMatrixIdentity());

		Vec3 FinalPos;
		XMStoreFloat3(&FinalPos, screenPos);

		FinalPos.x -= (vResol.x / 2.f);
		FinalPos.y -= (vResol.y / 2.f);
		FinalPos.y = -FinalPos.y;
		FinalPos.z = 0.f;

		m_UIBoxTag->Transform()->SetRelativePos(Vec3(FinalPos.x, FinalPos.y + 150.f, -1.1f));
	}
}

void ER_DataScript_ItemBox::BeginRayOverlap()
{
	int a = 2;
	if (GetOwner()->GetRenderComponent() != nullptr && GetOwner()->GetRenderComponent()->GetMaterial(0) != nullptr)
	{
		GetOwner()->GetRenderComponent()->GetMaterial(0)->SetScalarParam(INT_3, &a);
	}
}

void ER_DataScript_ItemBox::OnRayOverlap()
{
}

void ER_DataScript_ItemBox::EndRayOverlap()
{	
	if (GetOwner()->GetRenderComponent() != nullptr && GetOwner()->GetRenderComponent()->GetMaterial(0) != nullptr)
	{
		int a = 1;
		GetOwner()->GetRenderComponent()->GetMaterial(0)->SetScalarParam(INT_3, &a);
	}
}

void ER_DataScript_ItemBox::CreateBoxUI()
{
	m_pItemBoxUI;
	
	m_UIBoxTag = new CGameObject;
	m_UIBoxTag->SetName(GetOwner()->GetName() + L"_TagUI");
	AddComponents(m_UIBoxTag, _TRANSFORM | _MESHRENDER);

	m_UIBoxTag->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	m_UIBoxTag->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std2DMtrl"), 0);
	m_UIBoxTag->MeshRender()->GetMaterial(0)->SetTexParam(TEX_PARAM::TEX_0, CResMgr::GetInst()->GetInst()->FindRes<CTexture>(L"Img_ItemBox_Icon_Copy.png"));
	m_UIBoxTag->MeshRender()->GetDynamicMaterial(0);

	m_UIBoxTag->Transform()->SetRelativeScale(Vec3(100.f, 150.f, 1.f));

	SpawnGameObject(m_UIBoxTag, L"InGameUI");
}

// void ER_DataScript_ItemBox::CreateBoxIcon()
// {
	// m_pBoxIcon = ER_ItemMgr::GetInst()->GetItemObj(85)->Clone();
	// m_pBoxIcon->SetName(L"UI_BoxIcon");
	// 
	// m_pBoxIcon->AddComponent(new CTransform);
	// m_pBoxIcon->AddComponent(new CMeshRender);
	// m_pBoxIcon->AddComponent(new CUIComponent);
	// m_pBoxIcon->AddComponent(new CUIScript_Button);
	// 
	// m_pBoxIcon->Transform()->SetRelativeScale(Vec3(100.f, 150.f, 1.f));
	// m_pBoxIcon->Transform()->SetRelativeScale(Vec3(2.f, 3.f, 1.f));
	// m_pBoxIcon->Transform()->SetRelativeRot(Vec3(XMConvertToRadians(54.f), XMConvertToRadians(-45.f), 0.f));
	// 
	// m_pBoxIcon->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	// m_pBoxIcon->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std2DMtrl"), 0);
	// m_pBoxIcon->MeshRender()->GetMaterial(0)->SetTexParam(TEX_PARAM::TEX_0, CResMgr::GetInst()->GetInst()->FindRes<CTexture>(L"Img_ItemBox_Icon_Copy.png"));
	// m_pBoxIcon->MeshRender()->GetDynamicMaterial(0);
	
	// Vec3 resultPos = ER_UIMgr::GetInst()->WorldPosToUIPos(GetOwner()->Transform()->GetRelativePos());
	
	 // SpawnGameObject(m_pBoxIcon, Vec3(resultPos.x, resultPos.y + 120.f, -1.1f), L"UI");
	//SpawnGameObject(m_pBoxIcon, Vec3(GetOwner()->Transform()->GetRelativePos().x, GetOwner()->Transform()->GetRelativePos().y+4.f, GetOwner()->Transform()->GetRelativePos().z), L"ItemBox");
// }

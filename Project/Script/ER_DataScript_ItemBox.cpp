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

ER_DataScript_ItemBox::ER_DataScript_ItemBox()
	: CScript((UINT)SCRIPT_TYPE::ER_DATASCRIPT_ITEMBOX)
	, m_pItemList{}
	, m_UIBoxTag(nullptr)
	, m_SetItemCnt(0)
{
}

ER_DataScript_ItemBox::~ER_DataScript_ItemBox()
{
	Safe_Del_Array(m_pItemList);
}

Vec3 ER_DataScript_ItemBox::GetUIPos()
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

	return FinalPos;
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
	Vec3 vPos = GetUIPos();

	if (m_UIBoxTag)
	{
		m_UIBoxTag->Transform()->SetRelativePos(Vec3(vPos.x, vPos.y + 150.f, -1.1f));
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

bool ER_DataScript_ItemBox::RegistItem(UINT _ItemID)
{
	if (m_SetItemCnt == 5)
		return false;

	for (int i = m_SetItemCnt; i < (UINT)ITEMBOXSLOT::END; ++i)
	{
		// 슬롯이 비어있는 경우 아이템매니저에서 클론으로 생성해준다.
		if (!m_pItemList[i])
		{
			m_pItemList[i] = ER_ItemMgr::GetInst()->GetItemObj(_ItemID)->Clone();
			m_SetItemCnt = i + 1;
			return true;
		}
	}
	
	return false;
}

void ER_DataScript_ItemBox::CreateBoxUI()
{
	m_UIBoxTag = onew(CGameObject);
	m_UIBoxTag->SetName(GetOwner()->GetName() + L"_TagUI");
	AddComponents(m_UIBoxTag, _TRANSFORM | _MESHRENDER);

	m_UIBoxTag->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	m_UIBoxTag->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std2DMtrl"), 0);
	m_UIBoxTag->MeshRender()->GetMaterial(0)->SetTexParam(TEX_PARAM::TEX_0, CResMgr::GetInst()->GetInst()->FindRes<CTexture>(L"Img_ItemBox_Icon_Copy.png"));
	m_UIBoxTag->MeshRender()->GetDynamicMaterial(0);

	m_UIBoxTag->Transform()->SetRelativeScale(Vec3(100.f, 150.f, 1.f));

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

	SpawnGameObject(m_UIBoxTag, L"InGameUI");
}
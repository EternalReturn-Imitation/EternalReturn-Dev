#include "pch.h"
#include "ER_DataScript_ItemBox.h"

#include <Engine\CResMgr.h>
#include <Engine\CTexture.h>
#include <Engine\define.h>
#include "ER_ItemMgr.h"
#include "ER_ActionScript_Character.h"
#include "ER_UIMgr.h"
#include "ER_DataScript_Item.h"

#include "CUIScript_Button.h"

ER_DataScript_ItemBox::ER_DataScript_ItemBox()
	: CScript((UINT)SCRIPT_TYPE::ER_DATASCRIPT_ITEMBOX)
{
}

ER_DataScript_ItemBox::~ER_DataScript_ItemBox()
{
}

void ER_DataScript_ItemBox::init()
{
}

void ER_DataScript_ItemBox::begin()
{
	if (GetOwner()->GetRenderComponent() != nullptr && GetOwner()->GetRenderComponent()->GetMaterial(0) != nullptr)
	{
		int a = 1;
		GetOwner()->GetRenderComponent()->GetMaterial(0)->SetScalarParam(INT_3, &a);
	}

	CreateRandomItems();

	CreateBoxIcon();
}

void ER_DataScript_ItemBox::tick()
{
	if (m_pBoxIcon) {
		Vec3 resultPos = ER_UIMgr::GetInst()->WorldPosToUIPos(GetOwner()->Transform()->GetRelativePos());
		m_pBoxIcon->Transform()->SetRelativePos(Vec3(resultPos.x, resultPos.y + 120.f, -1.1f));
	}
}

void ER_DataScript_ItemBox::BeginOverlap(CCollider3D* _Other)
{
}

void ER_DataScript_ItemBox::OnOverlap(CCollider3D* _Other)
{
}

void ER_DataScript_ItemBox::EndOverlap(CCollider3D* _Other)
{
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

void ER_DataScript_ItemBox::CreateRandomItems()
{
	// 난수 생성기 초기화
	std::random_device rd;  // 하드웨어 난수 생성기를 사용하여 시드 생성
	std::mt19937 gen(rd()); // 시드 값으로 난수 생성 엔진 초기화
	std::uniform_int_distribution<> dis01(0, 41); // 0과 41을 포함하는 범위에서 정수형 난수 생성
	std::uniform_int_distribution<> dis02(1, 4); // 0과 41을 포함하는 범위에서 정수형 난수 생성

	// 랜덤한 숫자 생성 및 출력
	int random_number01 = dis01(gen);
	int random_number02 = dis02(gen);

	for (int i = 0; i < 2; ++i) {
		for (int j = 0; j < 4; ++j) {
			if (i == 1 && j > random_number02)
				break;

			m_vItemList.push_back(ER_ItemMgr::GetInst()->GetItemObj(random_number01)->Clone());

			random_number01 = dis01(gen);
		}
	}

	int a = 0;
}

void ER_DataScript_ItemBox::CreateBoxIcon()
{
	m_pBoxIcon = ER_ItemMgr::GetInst()->GetItemObj(85)->Clone();
	m_pBoxIcon->SetName(L"UI_BoxIcon");
	
	m_pBoxIcon->AddComponent(new CTransform);
	m_pBoxIcon->AddComponent(new CMeshRender);
	m_pBoxIcon->AddComponent(new CUI_Button);
	m_pBoxIcon->AddComponent(new CUIScript_Button);
	
	m_pBoxIcon->Transform()->SetRelativeScale(Vec3(100.f, 150.f, 1.f));
	//m_pBoxIcon->Transform()->SetRelativeScale(Vec3(2.f, 3.f, 1.f));
	//m_pBoxIcon->Transform()->SetRelativeRot(Vec3(XMConvertToRadians(54.f), XMConvertToRadians(-45.f), 0.f));
	
	m_pBoxIcon->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	m_pBoxIcon->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std2DMtrl"), 0);
	m_pBoxIcon->MeshRender()->GetMaterial(0)->SetTexParam(TEX_PARAM::TEX_0, CResMgr::GetInst()->GetInst()->FindRes<CTexture>(L"Img_ItemBox_Icon_Copy.png"));
	m_pBoxIcon->MeshRender()->GetDynamicMaterial(0);
	
	Vec3 resultPos = ER_UIMgr::GetInst()->WorldPosToUIPos(GetOwner()->Transform()->GetRelativePos());
	
	SpawnGameObject(m_pBoxIcon, Vec3(resultPos.x, resultPos.y + 120.f, -1.1f), L"UI");
	//SpawnGameObject(m_pBoxIcon, Vec3(GetOwner()->Transform()->GetRelativePos().x, GetOwner()->Transform()->GetRelativePos().y+4.f, GetOwner()->Transform()->GetRelativePos().z), L"ItemBox");
}

#include "pch.h"
#include "ER_DataScript_ItemBox.h"

#include <Engine\CResMgr.h>
#include <Engine\CTexture.h>
#include <Engine\define.h>

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
	int a = 1;
	if (GetOwner()->GetRenderComponent() != nullptr && GetOwner()->GetRenderComponent()->GetMaterial(0) != nullptr)
	{
		GetOwner()->GetRenderComponent()->GetMaterial(0)->SetScalarParam(INT_3, &a);
	}
}

void ER_DataScript_ItemBox::tick()
{
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
	int a = 1;
	if (GetOwner()->GetRenderComponent() != nullptr && GetOwner()->GetRenderComponent()->GetMaterial(0) != nullptr)
	{
		GetOwner()->GetRenderComponent()->GetMaterial(0)->SetScalarParam(INT_3, &a);
	}
}
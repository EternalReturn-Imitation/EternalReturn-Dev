#include "pch.h"
#include "ER_ItemBoxScript.h"

#include <Engine\CResMgr.h>
#include <Engine\CTexture.h>
#include <Engine\define.h>

ER_ItemBoxScript::ER_ItemBoxScript()
	: CScript((UINT)SCRIPT_TYPE::ER_ITEMBOXSCRIPT)
{
}

ER_ItemBoxScript::~ER_ItemBoxScript()
{
}

void ER_ItemBoxScript::init()
{
}

void ER_ItemBoxScript::begin()
{
	int a = 1;
	if (GetOwner()->GetRenderComponent() != nullptr && GetOwner()->GetRenderComponent()->GetMaterial(0) != nullptr)
	{
		GetOwner()->GetRenderComponent()->GetMaterial(0)->SetScalarParam(INT_3, &a);
	}
}

void ER_ItemBoxScript::tick()
{
}

void ER_ItemBoxScript::BeginOverlap(CCollider3D* _Other)
{
}

void ER_ItemBoxScript::OnOverlap(CCollider3D* _Other)
{
}

void ER_ItemBoxScript::EndOverlap(CCollider3D* _Other)
{
}

void ER_ItemBoxScript::BeginRayOverlap()
{
	int a = 2;
	if (GetOwner()->GetRenderComponent() != nullptr && GetOwner()->GetRenderComponent()->GetMaterial(0) != nullptr)
	{
		GetOwner()->GetRenderComponent()->GetMaterial(0)->SetScalarParam(INT_3, &a);
	}
}

void ER_ItemBoxScript::OnRayOverlap()
{
	
}

void ER_ItemBoxScript::EndRayOverlap()
{
	int a = 1;
	if (GetOwner()->GetRenderComponent() != nullptr && GetOwner()->GetRenderComponent()->GetMaterial(0) != nullptr)
	{
		GetOwner()->GetRenderComponent()->GetMaterial(0)->SetScalarParam(INT_3, &a);
	}
}

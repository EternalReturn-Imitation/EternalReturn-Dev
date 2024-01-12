#include "pch.h"
#include "CCloudScript.h"


CCloudScript::CCloudScript()
	: CScript((UINT)SCRIPT_TYPE::CLOUDSCRIPT)
{
}

CCloudScript::~CCloudScript()
{
}

void CCloudScript::begin()
{
}

void CCloudScript::tick()
{
	Vec3 vRot = GetOwner()->Transform()->GetRelativeRot();
	vRot.x += 0.5f * DT;

	if (2.f * XM_PI < vRot.x)
		vRot.x = vRot.x - 2.f * XM_PI;

	GetOwner()->Transform()->SetRelativeRot(vRot);
}

void CCloudScript::SaveToLevelFile(FILE* _File)
{
}

void CCloudScript::LoadFromLevelFile(FILE* _FILE)
{
}


#include "pch.h"
#include "CCircularMotionScript.h"

#include <Engine\func.h>
#include <Engine\CMeshRender.h>

#define __PI 3.1415926535f

CCircularMotionScript::CCircularMotionScript()
	: CScript((UINT)SCRIPT_TYPE::CIRCULARMOTIONSCRIPT)
	, m_pParentPlanet(nullptr)
	, m_fRadius(0.f)
	, m_fHertz(1.f)
{
}

CCircularMotionScript::~CCircularMotionScript()
{
}

void CCircularMotionScript::begin()
{
}

void CCircularMotionScript::tick()
{
	Vec3 vCenterPos = {};
	if (m_pParentPlanet == nullptr)
		vCenterPos = m_pParentPlanet->Transform()->GetRelativePos();
	
	Vec3 vPos = GetOwner()->Transform()->GetRelativePos();
	Vec3 vDir = vPos - vCenterPos;
	
	vDir.Normalize();
	Vec3 vecX(1.f, 0.f, 0.f);
	
	float theta = acos((vDir.x * vecX.x) + (vDir.y * vecX.y) + (vDir.z * vecX.z));

	Vec2(m_fRadius * cos(theta), m_fRadius * sin(theta));
	
}

void CCircularMotionScript::SaveToLevelFile(FILE* _File)
{
	// fwrite(&m_fSpeed, sizeof(float), 1, _File);
}

void CCircularMotionScript::LoadFromLevelFile(FILE* _FILE)
{
	// fread(&m_fSpeed, sizeof(float), 1, _File);
}


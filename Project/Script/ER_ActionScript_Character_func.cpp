#include "pch.h"
#include "ER_ActionScript_Character.h"
#include "ER_DataScript_Character.h"
#include "ER_ProjectilePool.h"
#include "ER_ProjectileScript.h"

#include <Engine/CPathFindMgr.h>
#include <Engine/CRenderMgr.h>
#include <Engine/CCollisionMgr.h>

tIngame_Stats* ER_ActionScript_Character::GetStatus()
{
	return m_Data->GetStatus();
}

tStatus_Effect* ER_ActionScript_Character::GetStatusEffect()
{
	return m_Data->GetStatusEffect();
}

Vec3 ER_ActionScript_Character::GetFocusPoint()
{
	// 비교용 Map Collider 받기
	CGameObject* Map = CPathFindMgr::GetInst()->GetMapCollider();

	// 비교용 Main Camera의 Ray 받기
	CCamera* mainCam = CRenderMgr::GetInst()->GetMainCam();
	tRay ray = mainCam->GetRay();

	// 교차점 위치 얻기
	IntersectResult result = CCollisionMgr::GetInst()->IsCollidingBtwRayRect(ray, Map);
	Vec3 TargetPos = result.vCrossPoint;

	return TargetPos;
}

Vec3 ER_ActionScript_Character::GetClearPoint(const Vec3& vDir, float dist)
{
	Vec3 vPos	= GetOwner()->Transform()->GetRelativePos();
	vPos += GetOwner()->FindPath()->findMaxClearPoint(vDir, 0.f, dist);

	return vPos;
}

float ER_ActionScript_Character::GetClearDistance(const Vec3& vDir, float dist)
{
	float ClearDist = 0.f;
	ClearDist = GetOwner()->FindPath()->findMaxClearDistance(vDir, 0.f, dist);

	return ClearDist;
}

float ER_ActionScript_Character::GetClearDistanceByWall(const Vec3& vDir, float dist)
{
	float ClearDist = 0.f;
	ClearDist = GetOwner()->FindPath()->findMaxClearDistanceByWall(vDir, 0.f, dist);

	return ClearDist;
}

Vec3 ER_ActionScript_Character::SetRotationToTarget(const Vec3& vTarget)
{
	Vec3 vPos = GetOwner()->Transform()->GetRelativePos();
	Vec3 vTargetDir = (vTarget - vPos).Normalize();	// 다음 방향 벡터

	// 앞쪽 방향 계산
	float yRad = atan2(-DirectX::XMVectorGetX(vTargetDir),
		sqrt(DirectX::XMVectorGetY(vTargetDir) *
			DirectX::XMVectorGetY(vTargetDir) +
			DirectX::XMVectorGetZ(vTargetDir) *
			DirectX::XMVectorGetZ(vTargetDir)));

	// 벡터가 양수인 경우 180도 회전
	if (vTargetDir.z > 0.0f)
		yRad = (DirectX::XM_PI - yRad);

	Vec3 FinalDir(0.f, yRad, 0.f);
	
	GetOwner()->Transform()->SetRelativeRot(FinalDir);

	return FinalDir;
}

bool ER_ActionScript_Character::IsInRange(CGameObject* Target, float _fRange)
{
	// 사거리판단
	Vec3 TargetPos = Target->Transform()->GetRelativePos();
	Vec3 OwnerPos = GetOwner()->Transform()->GetRelativePos();
	TargetPos.y = 0;
	OwnerPos.y = 0;

	float fDist = Vec3::Distance(TargetPos, OwnerPos);

	if (fDist <= _fRange)
		return true;

	return false;
}

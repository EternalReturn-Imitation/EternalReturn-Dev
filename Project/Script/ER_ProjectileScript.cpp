#include "pch.h"
#include "ER_ProjectileScript.h"
#include "ER_ProjectilePool.h"

#include <Engine\CRenderMgr.h>
#include <Engine\CPathFindMgr.h>
#include <Engine\CCollisionMgr.h>



#define PROJECTILE_LAYER 14

ER_ProjectileScript::ER_ProjectileScript()
	: CScript((UINT)SCRIPT_TYPE::ER_PROJECTILESCRIPT)
	, m_pShooter(nullptr)
	, m_pTarget(nullptr)
	, m_vSpawnPos{}
	, m_vDir{}
	, m_fFrontDir()
	, m_ProjectileType()
	, m_vProjectileScale{}
	, m_DmgType()
	, m_SkillInst(nullptr)
	, m_SkiilFunc(nullptr)
	, m_iPoolIdx(-1)
{
}

ER_ProjectileScript::~ER_ProjectileScript()
{
}

void ER_ProjectileScript::SetMeshData(Ptr<CMesh> _pMesh, Ptr<CMaterial> _pMtrl)
{
	MeshRender()->SetMesh(_pMesh.Get());
	MeshRender()->SetMaterial(_pMtrl, 0);
}

void ER_ProjectileScript::Dead()
{
	EraseObject(GetOwner());

	// 오브젝트풀에서 본인인덱스 사용중 false로 변경하기
	ER_ProjectilePool::GetInst()->SetDead(m_iPoolIdx);

	// Data 초기화 클리어
	
	m_pShooter = nullptr;
	m_pTarget = nullptr;
	m_vSpawnPos = {};
	m_fFrontDir = 0.f;
	m_vDir = {};
	m_vProjectileScale = {};

	m_DmgType = eDmgType::END;
	m_ProjectileType = eProjectileType::END;
	
	m_fSpeed = 0.f;
	m_fLifetTime = 0.f;

	m_SkillInst = nullptr;
	m_SkiilFunc = nullptr;
}

float ER_ProjectileScript::SetRotationToTaret(Vec3 _OwnerPos, Vec3 _TargetPos)
{
	Vec3 vPos = _OwnerPos;
	Vec3 vTargetDir = (_TargetPos - vPos).Normalize();	// 다음 방향 벡터
	
	// 앞쪽 방향 계산
	float yRad = atan2(-DirectX::XMVectorGetX(vTargetDir),
		sqrt(DirectX::XMVectorGetY(vTargetDir) *
			DirectX::XMVectorGetY(vTargetDir) +
			DirectX::XMVectorGetZ(vTargetDir) *
			DirectX::XMVectorGetZ(vTargetDir)));

	// 벡터가 양수인 경우 180도 회전
	if (vTargetDir.z > 0.0f)
		yRad = (DirectX::XM_PI - yRad);

	return yRad;
}

void ER_ProjectileScript::Shot()
{
	// 프로젝타일 스크립트에서 본인 사용으로 변경
	ER_ProjectilePool::GetInst()->SetUse(m_iPoolIdx);

	Vec3 vTargetPos = m_pTarget->Transform()->GetRelativePos();
	Vec3 vRot = Transform()->GetRelativeRot();
	vRot.y = SetRotationToTaret(m_vSpawnPos, vTargetPos);
	Transform()->SetRelativeRot(vRot);

	// 레이어에 본인 추가
	SpawnGameObject(GetOwner(), m_vSpawnPos, PROJECTILE_LAYER);
	GetOwner()->SetOutOfLayer(false);
}

void ER_ProjectileScript::ShotTarget(CGameObject* _Shooter, CGameObject* _Target, Vec3 _SpawnPos, eDmgType _DmgType, float _fSpeed)
{
	m_pShooter = _Shooter;
	m_pTarget = _Target;
	m_vSpawnPos = _SpawnPos;
	m_DmgType = _DmgType;
	m_fSpeed = _fSpeed;
}

void ER_ProjectileScript::ShotDir(CGameObject* _Shooter, Vec3 _SpawnPos, Vec3 _Dir, eDmgType _DmgType, float _fSpeed, float _fLifeTime)
{
}

void ER_ProjectileScript::begin()
{
	
}

void ER_ProjectileScript::tick()
{
	if (GetOwner()->IsOutofLayer())
	{
		Dead();
		return;
	}

	Vec3 vPos = Transform()->GetRelativePos();
	Vec3 vRot = Transform()->GetRelativeRot();
	Vec3 vTargetPos = m_pTarget->Transform()->GetRelativePos();
	
	// Vec3 vFrontDir = Transform()->GetWorldDir(DIR_TYPE::UP);	// 메시의 UP방향이 앞쪽방향임
	// vFrontDir *= 1.f * m_fFrontDir;

	/*
	switch (m_ProjectileType)
	{
	case ER_ProjectileScript::USELIFETIME:
	{
		m_fLifetTime -= DT;
		if (m_fLifetTime <= 0)
		{
			Dead();
			return;
		}

		break;
	}
	case ER_ProjectileScript::GUIDED:
	{
		// if (!m_pTarget || !m_pTarget->IsEnable())
		// {
		// 	Dead();
		// 	return;
		// }
		

		// 타겟 포인트
		break;
	}
	};*/

	// if (m_pTarget)
	vRot.y = SetRotationToTaret(m_vSpawnPos, vTargetPos);
	Transform()->SetRelativeRot(vRot);
	vPos += (vTargetPos - vPos).Normalize() * DT * m_fSpeed;
	
	vPos.y = m_vSpawnPos.y;

	Transform()->SetRelativePos(vPos);
	
}

void ER_ProjectileScript::BeginOverlap(CCollider3D* _Other)
{
	if (_Other->GetOwner() == m_pTarget)
	{
		GetOwner()->SetOutOfLayer(true);
	}
}

void ER_ProjectileScript::OnOverlap(CCollider3D* _Other)
{
}

void ER_ProjectileScript::EndOverlap(CCollider3D* _Other)
{
}



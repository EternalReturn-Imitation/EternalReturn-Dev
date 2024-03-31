#include "pch.h"
#include "ER_DataScript_Projectile.h"

#include "ER_DataScript_Character.h"

#define PROJECTILE_LAYER 14

ER_DataScript_Projectile::ER_DataScript_Projectile(SCRIPT_TYPE _type)
	: CScript((UINT)_type)
    , m_pShooter(nullptr)
	, m_pTarget(nullptr)
	, m_vSpawnPos{}
	, m_vDir{}
	, m_DmgType(eDmgType::END)
	, m_SkillInst(nullptr)
	, m_SkillFunc(nullptr)
	, m_fSpeed(0.f)
	, m_fLifeTime(0.f)
	, m_SkillData(nullptr)
	, m_SkillHitStep(0)
{
}

ER_DataScript_Projectile::~ER_DataScript_Projectile()
{
}


void ER_DataScript_Projectile::init()
{
	AddComponents(GetOwner(), _TRANSFORM | _COLLIDER3D | _MESHRENDER);
}

float ER_DataScript_Projectile::SetRotationToTarget(Vec3 _OwnerPos, Vec3 _TargetPos)
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

void ER_DataScript_Projectile::begin()
{
}

void ER_DataScript_Projectile::tick()
{
	Vec3 vPos = Transform()->GetRelativePos();
	Vec3 vRot = Transform()->GetRelativePos();

	// 타겟 추적 이동
	if (m_pTarget)
	{
		Vec3 vTargetPos = m_pTarget->Transform()->GetRelativePos();
		vTargetPos.y = m_vSpawnPos.y;

		
		vPos += (vTargetPos - vPos).Normalize() * DT * m_fSpeed;
	}
	// 방향 이동
	else
	{
		m_fLifeTime -= DT;

		if (m_fLifeTime <= 0)
		{
			m_fLifeTime = 0.f;
			DestroyObject(GetOwner());
			return;
		}

		vPos += m_vDir * DT * m_fSpeed;
	}

	Transform()->SetRelativePos(vPos);
}

void ER_DataScript_Projectile::Spawn()
{
	// 방향 진행방향으로 회전
	Vec3 vRot = Transform()->GetRelativeRot();

	if (m_pTarget)
	{
		Vec3 vTargetPos = m_pTarget->Transform()->GetRelativePos();
		vTargetPos.y = 0.f;
		Vec3 vShooterPos = m_pShooter->Transform()->GetRelativePos();
		vShooterPos.y = 0.f;

		vRot.y = SetRotationToTarget(vShooterPos, vTargetPos);
	}
	else
	{
		Vec3 vTargetPos = m_vSpawnPos;
		vTargetPos.y = 0.f;
		Vec3 vShooterPos = m_pShooter->Transform()->GetRelativePos();
		vShooterPos.y = 0.f;

		m_vDir = (vTargetPos - vShooterPos).Normalize();

		vRot.y = SetRotationToTarget(vShooterPos, vTargetPos);
	}

	Transform()->SetRelativeRot(vRot);

	// 레이어에 본인 추가
	SpawnGameObject(GetOwner(), m_vSpawnPos, PROJECTILE_LAYER);
}

void ER_DataScript_Projectile::SetForTarget(CGameObject* _Shooter, CGameObject* _Target, Vec3 _SpawnPos, float _fSpeed)
{
	m_pShooter = _Shooter;
	m_pTarget = _Target;
	m_vSpawnPos = _SpawnPos;
	m_DmgType = eDmgType::NORMAL;
	m_fSpeed = _fSpeed;
}

void ER_DataScript_Projectile::SetForDir(CGameObject* _Shooter, Vec3 _SpawnPos, float _fSpeed, float _fLifeTime)
{
	m_pShooter = _Shooter;
	m_vSpawnPos = _SpawnPos;
	m_DmgType = eDmgType::NORMAL;
	m_fSpeed = _fSpeed;
	m_fLifeTime = _fLifeTime;
}

void ER_DataScript_Projectile::BeginOverlap(CCollider3D* _Other)
{
	// 타겟오브젝트와 충돌한 오브젝트가 일치한다.
	CGameObject* Target = _Other->GetOwner();
	
	if (m_pTarget && _Other->GetOwner() == m_pTarget)
	{
		// 데미지연산함수
		BATTLE_COMMON(m_pShooter, m_pTarget);
		
		// 사운드 재생
		if (nullptr != m_pSound)
			m_pSound->Play(1, 0.5f, true);
		
		// 레이어에서 삭제예정으로 지정
		DestroyObject(GetOwner());
	}
	// 발사한 객체와 충돌한경우는 예외한다.
	
	if (!m_pTarget && _Other->GetOwner() != m_pShooter)
	{
		ER_DataScript_Character* CharacterScript = _Other->GetOwner()->GetScript<ER_DataScript_Character>();

		if (CharacterScript && !CharacterScript->IsDeadState())
		{
			// 데미지 타입별로 연산함수 
			if (eDmgType::NORMAL == m_DmgType)
			{
				// 데미지연산함수
				BATTLE_COMMON(m_pShooter, m_pTarget);

				// 사운드 재생
				if (nullptr != m_pSound)
					m_pSound->Play(1, 0.5f, true);

				DestroyObject(GetOwner());
			}
			else if (eDmgType::SKILL == m_DmgType)
			{
				if (m_SkillInst && m_SkillFunc)
				{
					ER_BattleSystem::GetInst()->Battle_Skill(m_pShooter, _Other->GetOwner(), m_SkillInst, m_SkillFunc, m_SkillData, m_SkillHitStep);

					// 사운드 재생
					if (nullptr != m_pSound)
						m_pSound->Play(1, 0.5f, true);

					DestroyObject(GetOwner());
				}
			}
		}
	}
}

void ER_DataScript_Projectile::OnOverlap(CCollider3D* _Other)
{
}

void ER_DataScript_Projectile::EndOverlap(CCollider3D* _Other)
{
}
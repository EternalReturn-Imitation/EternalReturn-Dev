#include "pch.h"
#include "ER_ProjectilePool.h"

#include "ER_ProjectileScript.h"

#define PROJECTILECNT 50

ER_ProjectilePool::ER_ProjectilePool()
{
}

ER_ProjectilePool::~ER_ProjectilePool()
{
	for (size_t i = 0; i < m_vecProjectilePool.size(); ++i)
	{
		if (nullptr != m_vecProjectilePool[i].second && !m_vecProjectilePool[i].first)
		{
			delete m_vecProjectilePool[i].second;
		}
	}
	
	m_vecProjectilePool.clear();
}

bool ER_ProjectilePool::SetUse(int _PoolIdx)
{

	if (!m_vecProjectilePool[_PoolIdx].first)
	{
		m_vecProjectilePool[_PoolIdx].first = true;

		return true;
	}

	return false;
}

bool ER_ProjectilePool::SetDead(int _PoolIdx)
{

	if (m_vecProjectilePool[_PoolIdx].first)
	{
		m_vecProjectilePool[_PoolIdx].first = false;

		return true;
	}

	return false;
}

ER_ProjectileScript* ER_ProjectilePool::GetProjectile(eProjType _type)
{
	ER_ProjectileScript* projectile = nullptr;

	// GetProjectil
	for (int i = m_iLastIdx + 1; i <= PROJECTILECNT; ++i)
	{
		// 사용중이지 않다면 ProjectileScript를 얻어온다.
		if (!m_vecProjectilePool[i].first)
		{
			projectile = m_vecProjectilePool[i].second->GetScript<ER_ProjectileScript>();
			m_iLastIdx = i;
			break;
		}

		// 마지막 순서까지 돌았지만 찾지 못했을때 첫번재로 돌아간다.
		if (PROJECTILECNT <= i + 1)
		{
			i = 0;
		}
		// 한바퀴를 돌고 탐색을 시작지점까지 돌아왔을경우 사용가능한 오브젝트가 없기때문에 nullptr 반환
		// 풀사이즈를 늘리는 방법도 있지만 구현하지 않을 예정
		else if (i == m_iLastIdx)
		{
			return nullptr;
		}
	}

	switch (_type)
	{
	case ER_ProjectilePool::eProjType::ARROW:
	{
		projectile->SetMeshData(m_pProjMesh[(UINT)eProjType::ARROW], m_pProjMtrl[(UINT)eProjType::ARROW]);
		projectile->Transform()->SetRelativeRot(Vec3(Deg2Rad(90.f), 0.f, 0.f));
		projectile->Transform()->SetRelativeScale(m_pProjScale[(UINT)eProjType::ARROW]);
		projectile->SetFrontDir(-1.f);
		break;
	}
	case ER_ProjectilePool::eProjType::BULLET:
	{
		projectile->SetMeshData(m_pProjMesh[(UINT)eProjType::BULLET], m_pProjMtrl[(UINT)eProjType::BULLET]);
		projectile->Transform()->SetRelativeRot(Vec3(Deg2Rad(-90.f), 0.f, 0.f));
		projectile->Transform()->SetRelativeScale(m_pProjScale[(UINT)eProjType::BULLET]);
		projectile->SetFrontDir(1.f);
		break;
	}
	}


    return projectile;
}

void ER_ProjectilePool::init()
{
	m_vecProjectilePool.resize(PROJECTILECNT);

	wstring strName = L"ProjecTile";

	// 오브젝트 풀 생성
	for (int i = 0; i < PROJECTILECNT; ++i)
	{
		CGameObject* Projectile = new CGameObject;
		AddComponents(Projectile, _TRANSFORM | _MESHRENDER | _COLLIDER3D);
		Projectile->AddComponent(new ER_ProjectileScript());
		Projectile->GetScript<ER_ProjectileScript>()->SetPoolIdx(i);
		
		Projectile->Collider3D()->SetCollider3DType(COLLIDER3D_TYPE::CUBE);
		Projectile->Collider3D()->SetOffsetScale(Vec3(0.2f, 0.2f, 0.2f));

		m_vecProjectilePool[i].first = false;
		m_vecProjectilePool[i].second = Projectile;

		Projectile->SetName(strName + std::to_wstring(i));
	}

	// MeshData Info Setting
	m_pProjMesh[(UINT)eProjType::ARROW] = CResMgr::GetInst()->FindRes<CMesh>(L"Rio_000_Arrow.mesh");
	m_pProjMtrl[(UINT)eProjType::ARROW] = CResMgr::GetInst()->FindRes<CMaterial>(L"Rio_01_LOD1.mtrl");
	
	m_pProjMesh[(UINT)eProjType::BULLET] = CResMgr::GetInst()->FindRes<CMesh>(L"Rozzi_Bullet.mesh");
	m_pProjMtrl[(UINT)eProjType::BULLET] = CResMgr::GetInst()->FindRes<CMaterial>(L"BulletMtrl.mtrl");

	m_pProjScale[(UINT)eProjType::ARROW] = Vec3(2.f, 1.f, 2.f);
	m_pProjScale[(UINT)eProjType::BULLET] = Vec3(0.2f, 0.2f, 0.2f);

}
#include "pch.h"
#include "ER_DataScript_Bullet.h"


ER_DataScript_Bullet::ER_DataScript_Bullet()
	:ER_DataScript_Projectile(SCRIPT_TYPE::ER_DATASCRIPT_BULLET)
{
}

ER_DataScript_Bullet::~ER_DataScript_Bullet()
{
}

void ER_DataScript_Bullet::init()
{
	ER_DataScript_Projectile::init();
	
	GetOwner()->SetName(L"Bullet");

	Collider3D()->SetCollider3DType(COLLIDER3D_TYPE::CUBE);
	Collider3D()->SetOffsetScale(Vec3(0.2f, 0.2f, 0.2f));

	MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"Rozzi_Bullet.mesh"));
	MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"BulletMtrl.mtrl"), 0);

	Transform()->SetRelativeScale(Vec3(0.2f, 0.2f, 0.2f));
	Transform()->SetRelativeRot(Vec3(Deg2Rad(-90.f), 0.f, 0.f));

	m_pSound = CResMgr::GetInst()->FindRes<CSound>(L"Pistol_Hit.wav");
}

void ER_DataScript_Bullet::begin()
{
}
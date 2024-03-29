#include "pch.h"
#include "ER_DataScript_Arrow.h"

ER_DataScript_Arrow::ER_DataScript_Arrow()
	: ER_DataScript_Projectile(SCRIPT_TYPE::ER_DATASCRIPT_ARROW)
{
}

ER_DataScript_Arrow::~ER_DataScript_Arrow()
{
}

void ER_DataScript_Arrow::init()
{
	ER_DataScript_Projectile::init();

	GetOwner()->SetName(L"Arrow");

	Collider3D()->SetCollider3DType(COLLIDER3D_TYPE::CUBE);
	Collider3D()->SetOffsetScale(Vec3(0.2f, 0.2f, 0.2f));
	
	MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"Rio_000_Arrow.mesh"));
	MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Rio_01_LOD1.mtrl"), 0);

	Transform()->SetRelativeScale(Vec3(2.f, 1.f, 2.f));
	Transform()->SetRelativeRot(Vec3(Deg2Rad(90.f), 0.f, 0.f));

	m_pSound = CResMgr::GetInst()->FindRes<CSound>(L"Rio_ShortBow_Hit_01.wav");
}


void ER_DataScript_Arrow::begin()
{
}
#include "pch.h"
#include "ER_PlayerScript.h"
#include "ER_PlayerScript_Range.h"

ER_PlayerScript_Range::ER_PlayerScript_Range()
	: CScript((UINT)SCRIPT_TYPE::ER_PLAYERSCRIPT_RANGE)
{
}

ER_PlayerScript_Range::~ER_PlayerScript_Range()
{
}


void ER_PlayerScript_Range::init()
{
	CGameObject* Owner = GetOwner();
	AddComponents(Owner, _TRANSFORM | _DECAL | _COLLIDER3D);

	Owner->Collider3D()->SetCollider3DType(COLLIDER3D_TYPE::SPHERE);
	Owner->Collider3D()->SetOffsetScale(Vec3(1.0f, 0.1f, 1.0f));
	Owner->Collider3D()->SetOffsetPos(Vec3(0.f, 0.0f, 0.f));

	Owner->Decal()->SetDeferredDecal(false);
	Owner->SetEnable(true);
}

void ER_PlayerScript_Range::begin()
{
}

void ER_PlayerScript_Range::tick()
{
	Transform()->SetRelativePos(m_pOwner->Transform()->GetRelativePos());
	// Transform()->SetRelativeRot(m_pOwner->Transform()->GetRelativeRot());
}

void ER_PlayerScript_Range::SetOwnerObj(CGameObject* _owner)
{
	m_pOwner = _owner;
	m_pOwner->GetScript<ER_PlayerScript>()->SetRangeObject(GetOwner());
}

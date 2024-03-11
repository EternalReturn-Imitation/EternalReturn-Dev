#include "pch.h"
#include "CCollider3D.h"

CCollider3D::CCollider3D()
	: CComponent(COMPONENT_TYPE::COLLIDER3D)
{
}

CCollider3D::CCollider3D(const CCollider3D& _other)
	: CComponent(COMPONENT_TYPE::COLLIDER3D)
{
}

CCollider3D::~CCollider3D()
{
}

void CCollider3D::finaltick()
{
}

void CCollider3D::BeginOverlap(CCollider3D* _Other)
{
}

void CCollider3D::OnOverlap(CCollider3D* _Other)
{
}

void CCollider3D::EndOverlap(CCollider3D* _Other)
{
}

void CCollider3D::BeginRayOverlap()
{
}

void CCollider3D::OnRayOverlap()
{
}

void CCollider3D::EndRayOverlap()
{
}

void CCollider3D::SaveToLevelFile(FILE* _File)
{
}

void CCollider3D::LoadFromLevelFile(FILE* _File)
{
}

void CCollider3D::SaveToDB(int _gameObjectID, COMPONENT_TYPE _componentType)
{
}

void CCollider3D::LoadFromDB(int _gameObjectID)
{
}


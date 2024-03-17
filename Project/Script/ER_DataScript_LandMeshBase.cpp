#include "pch.h"
#include "ER_DataScript_LandMeshBase.h"

#include <Engine\CResMgr.h>

int ER_DataScript_LandMeshBase::m_iRoofCount = 0;

ER_DataScript_LandMeshBase::ER_DataScript_LandMeshBase()
	: CScript((UINT)SCRIPT_TYPE::ER_DATASCRIPT_LANDMESHBASE)
{
}

ER_DataScript_LandMeshBase::~ER_DataScript_LandMeshBase()
{
}

void ER_DataScript_LandMeshBase::init()
{
}

void ER_DataScript_LandMeshBase::begin()
{
}

void ER_DataScript_LandMeshBase::tick()
{
}

void ER_DataScript_LandMeshBase::BeginOverlap(CCollider3D* _Other)
{	
	if (_Other->GetOwner()->GetLayerIndex() == 13) {
		++m_iRoofCount;
		m_pRoof->SetEnable(false);
	}
}

void ER_DataScript_LandMeshBase::OnOverlap(CCollider3D* _Other)
{
	
}

void ER_DataScript_LandMeshBase::EndOverlap(CCollider3D* _Other)
{
	if (_Other->GetOwner()->GetLayerIndex() == 13) {
		--m_iRoofCount;
		if(m_iRoofCount == 0)
			m_pRoof->SetEnable(true);
	}
}

void ER_DataScript_LandMeshBase::BeginRayOverlap()
{
}

void ER_DataScript_LandMeshBase::OnRayOverlap()
{
}

void ER_DataScript_LandMeshBase::EndRayOverlap()
{
}
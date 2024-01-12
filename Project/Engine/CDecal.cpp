#include "pch.h"
#include "CDecal.h"

#include "CResMgr.h"
#include "CMesh.h"
#include "CMaterial.h"

#include "CTransform.h"

CDecal::CDecal()
	: CRenderComponent(COMPONENT_TYPE::DECAL)
	, m_bDeferred(false)
	, m_bEmissive(false)
{
	SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"CubeMesh"));
	SetDeferredDecal(m_bDeferred);
}

CDecal::~CDecal()
{

}


void CDecal::finaltick()
{
	DrawDebugCube(Transform()->GetWorldMat(), Vec4(0.f, 1.f, 0.f, 1.f), 0.f, true);
}

void CDecal::render()
{
	if (nullptr == GetMaterial(0) || nullptr == GetMesh())
		return;

	Transform()->UpdateData();

	int IsEmissive = m_bEmissive;
	GetMaterial(0)->SetScalarParam(INT_0, &IsEmissive);
	GetMaterial(0)->UpdateData();

	GetMesh()->render(0);
}



void CDecal::SetDeferredDecal(bool _bDeferred)
{
	m_bDeferred = _bDeferred;

	if (m_bDeferred)
	{
		SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"DeferredDecalMtrl"), 0);
	}
	else
	{
		SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"DecalMtrl"), 0);
	}

	GetMaterial(0)->SetTexParam(TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"PositionTargetTex"));

}
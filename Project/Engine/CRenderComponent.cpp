#include "pch.h"
#include "CRenderComponent.h"

#include "CResMgr.h"
#include "CTransform.h"
#include "CAnimator3D.h"
#include "CStructuredBuffer.h"

CRenderComponent::CRenderComponent(COMPONENT_TYPE _type)
	: CComponent(_type)
	, m_fBounding(500.f)
	, m_bFrustumCheck(true)
	, m_bDynamicShadow(false)
	, m_fBoundingBoxScale(0.f)
	, m_fBoundingBoxOffsetScale(0.f)
{
	
}

CRenderComponent::~CRenderComponent()
{
	if (!m_vecMtrls.empty())
	{
		for (auto Mtrls : m_vecMtrls)
		{
			Mtrls.pCurMtrl = nullptr;
			Mtrls.pDynamicMtrl = nullptr;
			Mtrls.pSharedMtrl = nullptr;
		}
	}
}

void CRenderComponent::render_shadowmap()
{
	//Ptr<CMaterial> pShadowMapMtrl = CResMgr::GetInst()->FindRes<CMaterial>(L"ShadowMapMtrl");
	//
	//Transform()->UpdateData();
	//
	//pShadowMapMtrl->UpdateData();
	//
	//GetMesh()->render(0);
	Ptr<CMaterial> pShadowMapMtrl = CResMgr::GetInst()->FindRes<CMaterial>(L"ShadowMapMtrl");

	Transform()->UpdateData();

	// Animator3D 업데이트
	if (Animator3D())
	{
		Animator3D()->GetFinalBoneMat()->UpdateData(30, PIPELINE_STAGE::PS_VERTEX);
		pShadowMapMtrl->SetAnim3D(true);
	}
	else {
		pShadowMapMtrl->SetAnim3D(false);
	}

	pShadowMapMtrl->UpdateData();

	UINT iSubsetCount = GetMesh()->GetSubsetCount();

	for (UINT i = 0; i < iSubsetCount; i++) {
		if (nullptr != GetMaterial(i))
		{
			GetMesh()->render(i);
		}
	}

	if (Animator3D()) {
		Animator3D()->GetFinalBoneMat()->Clear();
		Animator3D()->ClearData();
	}

	//GetMaterial(0)->GetShader()->SetRSType(RS_TYPE::WIRE_FRAME);
}


void CRenderComponent::SetMesh(Ptr<CMesh> _Mesh)
{
	m_pMesh = _Mesh;

	if (!m_vecMtrls.empty())
	{
		m_vecMtrls.clear();
		vector<tMtrlSet> vecMtrls;
		m_vecMtrls.swap(vecMtrls);
	}


	if (nullptr != m_pMesh)
		m_vecMtrls.resize(m_pMesh->GetSubsetCount());
}

void CRenderComponent::SetMaterial(Ptr<CMaterial> _Mtrl, UINT _idx)
{
	m_vecMtrls[_idx].pSharedMtrl = _Mtrl;
	m_vecMtrls[_idx].pCurMtrl = _Mtrl;
}

Ptr<CMaterial> CRenderComponent::GetMaterial(UINT _idx)
{
	if (nullptr == m_vecMtrls[_idx].pCurMtrl)
	{
		m_vecMtrls[_idx].pCurMtrl = m_vecMtrls[_idx].pSharedMtrl;
	}

	return m_vecMtrls[_idx].pCurMtrl;
}

Ptr<CMaterial> CRenderComponent::GetSharedMaterial(UINT _idx)
{
	m_vecMtrls[_idx].pCurMtrl = m_vecMtrls[_idx].pSharedMtrl;

	if (m_vecMtrls[_idx].pDynamicMtrl.Get())
	{
		m_vecMtrls[_idx].pDynamicMtrl = nullptr;
	}

	return m_vecMtrls[_idx].pSharedMtrl;
}

Ptr<CMaterial> CRenderComponent::GetDynamicMaterial(UINT _idx)
{
	// 원본 재질이 없다 -> Nullptr 반환
	if (nullptr == m_vecMtrls[_idx].pSharedMtrl)
	{
		m_vecMtrls[_idx].pCurMtrl = nullptr;
		return m_vecMtrls[_idx].pCurMtrl;
	}

	if (nullptr == m_vecMtrls[_idx].pDynamicMtrl)
	{
		m_vecMtrls[_idx].pDynamicMtrl = m_vecMtrls[_idx].pSharedMtrl->Clone();
		m_vecMtrls[_idx].pDynamicMtrl->SetName(m_vecMtrls[_idx].pSharedMtrl->GetName() + L"_Clone");
		m_vecMtrls[_idx].pCurMtrl = m_vecMtrls[_idx].pDynamicMtrl;
	}

	return m_vecMtrls[_idx].pCurMtrl;
}

ULONG64 CRenderComponent::GetInstID(UINT _iMtrlIdx)
{
	if (m_pMesh == NULL || m_vecMtrls[_iMtrlIdx].pCurMtrl == NULL)
		return 0;

	uInstID id{ (UINT)m_pMesh->GetID(), (WORD)m_vecMtrls[_iMtrlIdx].pCurMtrl->GetID(), (WORD)_iMtrlIdx };
	return id.llID;
}


void CRenderComponent::SaveToLevelFile(FILE* _File)
{
	SaveResRef(m_pMesh.Get(), _File);

	UINT iMtrlCount = GetMtrlCount();
	fwrite(&iMtrlCount, sizeof(UINT), 1, _File);

	for (UINT i = 0; i < iMtrlCount; ++i)
	{
		SaveResRef(m_vecMtrls[i].pSharedMtrl.Get(), _File);
	}

	fwrite(&m_bDynamicShadow, 1, 1, _File);
	fwrite(&m_bFrustumCheck, 1, 1, _File);
	fwrite(&m_fBounding, 1, 1, _File);
}

void CRenderComponent::LoadFromLevelFile(FILE* _File)
{
	LoadResRef(m_pMesh, _File);

	SetMesh(m_pMesh);

	UINT iMtrlCount = GetMtrlCount();
	fread(&iMtrlCount, sizeof(UINT), 1, _File);

	for (UINT i = 0; i < iMtrlCount; ++i)
	{
		Ptr<CMaterial> pMtrl;
		LoadResRef(pMtrl, _File);
		SetMaterial(pMtrl, i);
	}

	fread(&m_bDynamicShadow, 1, 1, _File);
	fread(&m_bFrustumCheck, 1, 1, _File);
	fread(&m_fBounding, 1, 1, _File);
}
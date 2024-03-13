#include "pch.h"
#include "CLight3D.h"

#include "CResMgr.h"

#include "CRenderMgr.h"
#include "CTransform.h"
#include "CCamera.h"

CCamera* CLight3D::GetLightRenderCam()
{
	return m_pCamObj->Camera();
}

CLight3D::CLight3D()
	: CComponent(COMPONENT_TYPE::LIGHT3D)
	, m_bShowRange(false)
	, m_LightIdx(-1)
	, m_pCamObj(nullptr)
	, m_bDebug(false)
{

	m_pCamObj = new CGameObject;
	m_pCamObj->AddComponent(new CTransform);
	m_pCamObj->AddComponent(new CCamera);

	m_pCamObj->Camera()->SetLayerMaskAll(true);
	m_pCamObj->Camera()->SetLayerMask(31, false);

	SetLightType(LIGHT_TYPE::DIRECTIONAL);
	SetLightColor(Vec3(1.f, 1.f, 1.f));
	SetLightAmbient(Vec3(0.f, 0.f, 0.f));
}

CLight3D::CLight3D(const CLight3D& _Origin)
	: CComponent(_Origin)
	, m_bShowRange(_Origin.m_bShowRange)
	, m_LightIdx(-1)
	, m_pCamObj(nullptr)
{
	m_pCamObj = new CGameObject(*_Origin.m_pCamObj);
}

CLight3D::~CLight3D()
{
	if (nullptr != m_pCamObj)
		delete m_pCamObj;
}

void CLight3D::finaltick()
{
	m_LightInfo.vWorldPos = Transform()->GetWorldPos();
	m_LightInfo.vWorldDir = Transform()->GetWorldDir(DIR_TYPE::FRONT);

	m_LightIdx = (UINT)CRenderMgr::GetInst()->RegisterLight3D(this, m_LightInfo);

	if (m_bShowRange || m_bDebug)//범위 보이는게 true일떄만 보이게
	{
		if ((UINT)LIGHT_TYPE::POINT == m_LightInfo.LightType)
		{
			DrawDebugSphere(Transform()->GetWorldMat(), Vec4(0.2f, 1.f, 0.2f, 1.f), 0.f, true);
		}
		else if ((UINT)LIGHT_TYPE::SPOT == m_LightInfo.LightType)
		{
			//DrawDebugCone(Transform()->GetWorldMat(), Vec4(0.2f, 1.f, 0.2f, 1.f), 0.f, true);
		}
	}

	// 광원에 부착한 카메라 오브젝트도 위치를 광원 위치랑 동일하게..
	// finaltick 호출시켜서 카메라 오브젝트의 카메라 컴포넌트의 view, proj 행렬 연산할수 있게 함
	*m_pCamObj->Transform() = *Transform();
	m_pCamObj->finaltick_module();
}

void CLight3D::render()
{
	Transform()->UpdateData();

	// Light 재질 업데이트
	m_Mtrl->SetScalarParam(INT_0, &m_LightIdx);
	m_Mtrl->UpdateData();

	if (m_LightInfo.LightType == (UINT)LIGHT_TYPE::DIRECTIONAL)
	{
		Matrix matVP = m_pCamObj->Camera()->GetViewMat() * m_pCamObj->Camera()->GetProjMat();
		m_Mtrl->SetScalarParam(MAT_0, &matVP);
		m_Mtrl->SetTexParam(TEX_2, CResMgr::GetInst()->FindRes<CTexture>(L"DynamicShadowMapTex"));
	}

	// 볼륨 메시 렌더
	m_Mesh->render(0);
}

void CLight3D::render_shadowmap()
{
	m_pCamObj->Camera()->SortObject_Shadow();

	m_pCamObj->Camera()->render_shadowmap();
}

void CLight3D::SetRadius(float _Radius)
{
	m_LightInfo.Radius = _Radius;

	// SphereMesh 의 로컬 반지름이 0.5f 이기 때문에 2배로 적용
	if (m_LightInfo.LightType != (UINT)LIGHT_TYPE::DIRECTIONAL)
		Transform()->SetRelativeScale(Vec3(_Radius * 2.f, _Radius * 2.f, _Radius * 2.f));
}

void CLight3D::SetLightType(LIGHT_TYPE _type)
{
	m_LightInfo.LightType = (int)_type;

	if (LIGHT_TYPE::DIRECTIONAL == (LIGHT_TYPE)m_LightInfo.LightType)
	{
		// 광원을 렌더링 할 때, 광원의 영향범위를 형상화 할 수 있는 메쉬(볼륨메쉬) 를 선택
		m_Mesh = CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh");
		m_Mtrl = CResMgr::GetInst()->FindRes<CMaterial>(L"DirLightMtrl");

		m_pCamObj->Camera()->SetFar(100000.f);
		m_pCamObj->Camera()->SetProjType(PROJ_TYPE::ORTHOGRAPHIC);
		m_pCamObj->Camera()->SetOrthoWidth(4000.f);
		m_pCamObj->Camera()->SetOrthoHeight(4000.f);
	}

	else if (LIGHT_TYPE::POINT == (LIGHT_TYPE)m_LightInfo.LightType)
	{
		m_Mesh = CResMgr::GetInst()->FindRes<CMesh>(L"SphereMesh");
		m_Mtrl = CResMgr::GetInst()->FindRes<CMaterial>(L"PointLightMtrl");
	}

	else
	{
		m_Mesh = CResMgr::GetInst()->FindRes<CMesh>(L"ConeMesh");
		m_Mtrl = CResMgr::GetInst()->FindRes<CMaterial>(L"SpotLightMtrl");
	}

	if (nullptr != m_Mtrl)
	{
		m_Mtrl->SetTexParam(TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"NormalTargetTex"));
		m_Mtrl->SetTexParam(TEX_1, CResMgr::GetInst()->FindRes<CTexture>(L"PositionTargetTex"));
	}
}

void CLight3D::SaveToLevelFile(FILE* _File)
{
	fwrite(&m_LightInfo.Color.vAmbient, sizeof(Vec4), 1, _File);
	fwrite(&m_LightInfo.Color.vDiffuse, sizeof(Vec4), 1, _File);
	fwrite(&m_LightInfo.vWorldPos,sizeof(Vec4), 1, _File);
	fwrite(&m_LightInfo.vWorldDir,sizeof(Vec4), 1, _File);
	fwrite(&m_LightInfo.LightType,sizeof(UINT), 1, _File);
	fwrite(&m_LightInfo.Radius,sizeof(float), 1, _File);
	fwrite(&m_LightInfo.Angle,sizeof(float), 1, _File);
	fwrite(&m_LightInfo.padding, sizeof(int), 1, _File);
	fwrite(&m_LightIdx, sizeof(float), 1, _File);

	m_pCamObj->Camera()->SaveToLevelFile(_File);
}

void CLight3D::LoadFromLevelFile(FILE* _File)
{
	fread(&m_LightInfo.Color.vAmbient, sizeof(Vec4), 1, _File);
	fread(&m_LightInfo.Color.vDiffuse, sizeof(Vec4), 1, _File);
	fread(&m_LightInfo.vWorldPos, sizeof(Vec4), 1, _File);
	fread(&m_LightInfo.vWorldDir, sizeof(Vec4), 1, _File);
	fread(&m_LightInfo.LightType, sizeof(UINT), 1, _File);
	fread(&m_LightInfo.Radius, sizeof(float), 1, _File);
	fread(&m_LightInfo.Angle, sizeof(float), 1, _File);
	fread(&m_LightInfo.padding, sizeof(int), 1, _File);
	fread(&m_LightIdx, sizeof(float), 1, _File);

	m_pCamObj->Camera()->LoadFromLevelFile(_File);
}
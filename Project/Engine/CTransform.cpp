#include "pch.h"
#include "CTransform.h"

#include "CDevice.h"
#include "CConstBuffer.h"
#include "CRenderComponent.h"
#include "CPathMgr.h"

CTransform::CTransform()
	: CComponent(COMPONENT_TYPE::TRANSFORM)
	, m_vRelativeScale(Vec3(1.f, 1.f, 1.f))
	, m_vOffsetRelativePos(Vec3(0.f,0.f,0.f))
	, m_bAbsolute(false)	
	, m_vRelativeDir{
		  Vec3(1.f, 0.f, 0.f)
		, Vec3(0.f, 1.f, 0.f)
		, Vec3(0.f, 0.f, 1.f)}	
	, b_GizmoOnSet(true)
	, OffsetTrigger(false)
{
	SetName(L"Transform");
}

CTransform::~CTransform()
{
}

void CTransform::finaltick()
{
	m_matWorldScale = XMMatrixIdentity();
	m_matWorldScale = XMMatrixScaling(m_vRelativeScale.x, m_vRelativeScale.y, m_vRelativeScale.z);
	
	// Bounding Box 행렬 구하기
	float x = m_vRelativeScale.x ;
	float y = m_vRelativeScale.y ;
	float z = m_vRelativeScale.z ;

	m_fBoundingRadius = (float)sqrt(pow(x, 2) + pow(y, 2) + pow(z, 2));

	m_matWorldBoundingScale = XMMatrixIdentity();
	m_matWorldBoundingScale = XMMatrixScaling(m_fBoundingRadius, m_fBoundingRadius, m_fBoundingRadius);
	
	m_matWorldRot = XMMatrixIdentity();
	m_matWorldRot = XMMatrixRotationX(m_vRelativeRot.x);
	m_matWorldRot *= XMMatrixRotationY(m_vRelativeRot.y);
	m_matWorldRot *= XMMatrixRotationZ(m_vRelativeRot.z);

	m_matWorldPos = XMMatrixTranslation(m_vRelativePos.x, m_vRelativePos.y, m_vRelativePos.z);
	if (OffsetTrigger) {
		Matrix matTranslationOffset = XMMatrixTranslation(m_vRelativePos.x - m_vOffsetRelativePos.x, m_vRelativePos.y - m_vOffsetRelativePos.y, m_vRelativePos.z - m_vOffsetRelativePos.z);
		m_matOffsetWorld = m_matWorldScale * m_matWorldRot * matTranslationOffset;
		m_matWorld = m_matWorldScale * m_matWorldRot * m_matWorldPos;
		m_matWorldForGizmo = m_matWorld;
		m_matWorldBoundingBox = m_matWorldBoundingScale * m_matWorldRot * m_matWorldPos;
	}
	else {
		m_matWorld = m_matWorldScale * m_matWorldRot * m_matWorldPos;
		m_matWorldForGizmo = m_matWorld;
		m_matWorldBoundingBox = m_matWorldBoundingScale * m_matWorldRot * m_matWorldPos;
	}

	Vec3 vDefaultDir[3] = {
		  Vec3(1.f, 0.f, 0.f)
		, Vec3(0.f, 1.f, 0.f)
		, Vec3(0.f, 0.f, 1.f)
	};

	for (int i = 0; i < 3; ++i)
	{
		m_vWorldDir[i] = m_vRelativeDir[i] = XMVector3TransformNormal(vDefaultDir[i], m_matWorldRot);
	}
	
	m_fBoundingRadius = m_matWorldBoundingScale._11;

	// 부모 오브젝트 확인
	CGameObject* pParent = GetOwner()->GetParent();
	if (pParent)
	{
		if (m_bAbsolute)
		{
			Matrix matParentWorld = pParent->Transform()->m_matWorld;
			Matrix matParentScale = pParent->Transform()->m_matWorldScale;
			Matrix matParentScaleInv = XMMatrixInverse(nullptr, matParentScale);

			// 월드 = 로컬월드 * 부모크기 역 * 부모 월드(크기/회전/이동)
			m_matWorld = m_matWorld * matParentScaleInv * matParentWorld;
		}
		else
		{
			m_matWorld *= pParent->Transform()->m_matWorld;
			m_matWorldScale *= pParent->Transform()->m_matWorldScale;
		}


		for (int i = 0; i < 3; ++i)
		{
			m_vWorldDir[i] = XMVector3TransformNormal(vDefaultDir[i], m_matWorld);
			m_vWorldDir[i].Normalize();
		}
	}

	m_matWorldInv = XMMatrixInverse(nullptr, m_matWorld);
	if (OffsetTrigger) {
		m_matOffsetWorldInv = XMMatrixInverse(nullptr, m_matOffsetWorld);
	}

}

void CTransform::UpdateData()
{
	// 위치값을 상수버퍼에 전달 및 바인딩		
	CConstBuffer* pTransformBuffer = CDevice::GetInst()->GetConstBuffer(CB_TYPE::TRANSFORM);

	if (OffsetTrigger) {
		g_transform.matWorld = m_matOffsetWorld;
		g_transform.matWorldInv = m_matOffsetWorldInv;
		g_transform.matWV = g_transform.matWorld * g_transform.matView;
		g_transform.matWVP = g_transform.matWV * g_transform.matProj;
	}else{
		g_transform.matWorld = m_matWorld;
		g_transform.matWorldInv = m_matWorldInv;
		g_transform.matWV = g_transform.matWorld * g_transform.matView;
		g_transform.matWVP = g_transform.matWV * g_transform.matProj;
	}


	pTransformBuffer->SetData(&g_transform);
	pTransformBuffer->UpdateData();
}

void CTransform::SetOriginAlignment()
{
	Vtx* vertices = GetOwner()->GetRenderComponent()->GetMesh()->GetVtxSysMem();

	vector<Vector4> vVtx;

	float xMin = 99999.f;
	float xMax = -99999.f;
	float yMin = 99999.f;
	float yMax = -99999.f;
	float zMin = 99999.f;
	float zMax = -99999.f;

	for (auto& indexInfo : GetOwner()->GetRenderComponent()->GetMesh()->GetIdxInfo()) {
		UINT* indices = (UINT*)indexInfo.pIdxSysMem; // 인덱스 데이터 접근
		vVtx.resize(indexInfo.iIdxCount);
		for (UINT i = 0; i < indexInfo.iIdxCount; i += 3) {
			// 삼각형 구성
			Vtx v1 = vertices[indices[i]];
			Vtx v2 = vertices[indices[i + 1]];
			Vtx v3 = vertices[indices[i + 2]];
			vVtx[i] = v1.vPos;
			if (vVtx[i].x < xMin)
				xMin = vVtx[i].x;
			if (vVtx[i].x > xMax)
				xMax = vVtx[i].x;
			if (vVtx[i].y < yMin)
				yMin = vVtx[i].y;
			if (vVtx[i].y > yMax)
				yMax = vVtx[i].y;
			if (vVtx[i].z < zMin)
				zMin = vVtx[i].z;
			if (vVtx[i].z > zMax)
				zMax = vVtx[i].z;

			vVtx[i + 1] = v2.vPos;
			if (vVtx[i + 1].x < xMin)
				xMin = vVtx[i + 1].x;
			if (vVtx[i + 1].x > xMax)
				xMax = vVtx[i].x;
			if (vVtx[i + 1].y < yMin)
				yMin = vVtx[i + 1].y;
			if (vVtx[i + 1].y > yMax)
				yMax = vVtx[i + 1].y;
			if (vVtx[i + 1].z < zMin)
				zMin = vVtx[i + 1].z;
			if (vVtx[i + 1].z > zMax)
				zMax = vVtx[i + 1].z;

			vVtx[i + 2] = v3.vPos;
			if (vVtx[i + 2].x < xMin)
				xMin = vVtx[i + 2].x;
			if (vVtx[i + 2].x > xMax)
				xMax = vVtx[i + 2].x;
			if (vVtx[i + 2].y < yMin)
				yMin = vVtx[i + 2].y;
			if (vVtx[i + 2].y > yMax)
				yMax = vVtx[i + 2].y;
			if (vVtx[i + 2].z < zMin)
				zMin = vVtx[i + 2].z;
			if (vVtx[i + 2].z > zMax)
				zMax = vVtx[i + 2].z;
		}
	}
	SetOffsetRelativePos(Vec3(xMin, yMin, zMin));
	SetOffsetTrigger(true);
}

void CTransform::SavePrefab(const wstring& _key)
{
	// 상대경로 저장
	wstring RelativePath = L"prefab\\transform\\";
	RelativePath += _key;
	RelativePath += L".trfprp";	// 확장자

	// 파일 경로 만들기
	wstring strFilePath = CPathMgr::GetInst()->GetContentPath() + RelativePath;

	path path_content = CPathMgr::GetInst()->GetContentPath();
	path path_prefab = path_content.wstring() + L"prefab\\transform\\";

	if (false == exists(path_prefab))
	{
		create_directory(path_prefab);
	}

	FILE* pFile = nullptr;
	errno_t err = _wfopen_s(&pFile, strFilePath.c_str(), L"wb");
	assert(pFile);

	// 키값
	SaveWString(m_PrefabKey, pFile);

	fwrite(&m_vRelativePos, sizeof(Vec3), 1, pFile);
	fwrite(&m_vRelativeScale, sizeof(Vec3), 1, pFile);
	fwrite(&m_vRelativeRot, sizeof(Vec3), 1, pFile);
	fwrite(&m_bAbsolute, sizeof(bool), 1, pFile);

	fclose(pFile);
}

void CTransform::LoadPrefab(const wstring& _key)
{
	// 읽기모드로 파일열기
	FILE* pFile = nullptr;

	wstring RelativePath = L"prefab\\transform\\";
	RelativePath += _key;
	RelativePath += L".trfprp";	// 확장자

	wstring strFilePath = CPathMgr::GetInst()->GetContentPath() + RelativePath;

	_wfopen_s(&pFile, strFilePath.c_str(), L"rb");

	// 키값
	LoadWString(m_PrefabKey, pFile);

	fread(&m_vRelativePos, sizeof(Vec3), 1, pFile);
	fread(&m_vRelativeScale, sizeof(Vec3), 1, pFile);
	fread(&m_vRelativeRot, sizeof(Vec3), 1, pFile);
	fread(&m_bAbsolute, sizeof(bool), 1, pFile);

	fclose(pFile);
}

void CTransform::SaveToLevelFile(FILE* _File)
{
	fwrite(&m_vRelativePos	, sizeof(Vec3), 1, _File);
	fwrite(&m_vRelativeScale, sizeof(Vec3), 1, _File);
	fwrite(&m_vRelativeRot	, sizeof(Vec3), 1, _File);
	fwrite(&m_bAbsolute, sizeof(bool), 1, _File);
}

void CTransform::LoadFromLevelFile(FILE* _FILE)
{	
	fread(&m_vRelativePos, sizeof(Vec3), 1, _FILE);
	fread(&m_vRelativeScale, sizeof(Vec3), 1, _FILE);
	fread(&m_vRelativeRot, sizeof(Vec3), 1, _FILE);
	fread(&m_bAbsolute, sizeof(bool), 1, _FILE);
}
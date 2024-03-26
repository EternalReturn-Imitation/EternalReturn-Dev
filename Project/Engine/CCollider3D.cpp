#include "pch.h"
#include "CCollider3D.h"
#include "CScript.h"
#include "components.h"
#include "CPathFindMgr.h"

CCollider3D::CCollider3D()
	: CComponent(COMPONENT_TYPE::COLLIDER3D)
	, m_Shape(COLLIDER3D_TYPE::SPHERE)
	, m_bAbsolute(false)
	, m_iCollisionCount(0)
	, m_bDrawCollision(true)
{
}

CCollider3D::CCollider3D(const CCollider3D& _other)
	: CComponent(COMPONENT_TYPE::COLLIDER3D)
	, m_vOffsetPos(_other.m_vOffsetPos)
	, m_vOffsetScale(_other.m_vOffsetScale)
	, m_bAbsolute(_other.m_bAbsolute)
	, m_Shape(_other.m_Shape)
	, m_bDrawCollision(_other.m_bDrawCollision)
{
}

CCollider3D::~CCollider3D()
{
}


void CCollider3D::begin()
{
	CPathFindMgr::GetInst()->AddColliderObjects(GetOwner());
}

void CCollider3D::finaltick()
{
	// 충돌 횟수가 음수인 경우
	assert(0 <= m_iCollisionCount);
	m_matColliderScale = XMMatrixIdentity();
	m_matColliderScale = XMMatrixScaling(m_vOffsetScale.x, m_vOffsetScale.y, m_vOffsetScale.z);
	m_matColliderPos = XMMatrixIdentity();
	m_matColliderPos = XMMatrixTranslation(m_vOffsetPos.x, m_vOffsetPos.y, m_vOffsetPos.z);


	m_matCollider3D = XMMatrixScaling(m_vOffsetScale.x, m_vOffsetScale.y, m_vOffsetScale.z);
	m_matCollider3D *= XMMatrixTranslation(m_vOffsetPos.x, m_vOffsetPos.y, m_vOffsetPos.z);
	// 크기 X 회전 X 이동 ( 회전은 안함)
	const Matrix& matWorld = Transform()->GetWorldMat(); // 최종 월드 행렬

	if (m_bAbsolute)
	{
		// 부모의 Scale 없애기
		Matrix matParentScaleInv = XMMatrixInverse(nullptr, Transform()->GetWorldScaleMat());
		// Collider의 Offset * 부모 크기의 역행렬 * 부모
		m_matCollider3D = m_matCollider3D * matParentScaleInv * matWorld;

	}
	else
	{
		// 충돌체 월드 * 오브젝트 월드
		m_matCollider3D *= matWorld;

		// 충돌체 scale update
		m_matColliderScale *= Transform()->GetWorldScaleMat();
		m_matColliderPos *= Transform()->GetWorldPosMat();
	}


	// DeabugShape 요청
	Vec4 vColor = Vec4(0.f, 1.f, 0.f, 1.f);
	if (0 < m_iCollisionCount)
		vColor = Vec4(1.f, 0.f, 0.f, 1.f);

	// 마지막에 0.f로 설정해 GameObject가 사라지면 Collider도 사라진다.
	if (m_bDrawCollision)
	{
		if (COLLIDER3D_TYPE::SPHERE == m_Shape)
			DrawDebugSphere(m_matCollider3D, vColor, 0.f);
		else
			DrawDebugCube(m_matCollider3D, vColor, 0.f);
	}
}



void CCollider3D::BeginOverlap(CCollider3D* _Other)
{
	m_iCollisionCount += 1;

	// Script 호출
	const vector<CScript*>& vecScript = GetOwner()->GetScripts();
	for (size_t i = 0; i < vecScript.size(); ++i)
	{
		vecScript[i]->BeginOverlap(_Other);
	}
}

void CCollider3D::OnOverlap(CCollider3D* _Other)
{
	// Script 호출
	const vector<CScript*>& vecScript = GetOwner()->GetScripts();
	for (size_t i = 0; i < vecScript.size(); ++i)
	{
		vecScript[i]->OnOverlap(_Other);
	}
}

void CCollider3D::EndOverlap(CCollider3D* _Other)
{
	m_iCollisionCount -= 1;

	// Script 호출
	const vector<CScript*>& vecScript = GetOwner()->GetScripts();
	for (size_t i = 0; i < vecScript.size(); ++i)
	{
		vecScript[i]->EndOverlap(_Other);
	}
}

void CCollider3D::BeginRayOverlap()
{
	// Script 호출
	const vector<CScript*>& vecScript = GetOwner()->GetScripts();
	for (size_t i = 0; i < vecScript.size(); ++i)
	{
		vecScript[i]->BeginRayOverlap();
	}
}

void CCollider3D::OnRayOverlap()
{
	// Script 호출
	const vector<CScript*>& vecScript = GetOwner()->GetScripts();
	for (size_t i = 0; i < vecScript.size(); ++i)
	{
		vecScript[i]->OnRayOverlap();
	}
}

void CCollider3D::EndRayOverlap()
{
	// Script 호출
	const vector<CScript*>& vecScript = GetOwner()->GetScripts();
	for (size_t i = 0; i < vecScript.size(); ++i)
	{
		vecScript[i]->EndRayOverlap();
	}
}

void CCollider3D::SavePrefab(const wstring& _key)
{
	// 상대경로 저장
	wstring RelativePath = L"prefab\\collider3d\\";
	RelativePath += _key;
	RelativePath += L".col3prp";	// 확장자

	// 파일 경로 만들기
	wstring strFilePath = CPathMgr::GetInst()->GetContentPath() + RelativePath;

	path path_content = CPathMgr::GetInst()->GetContentPath();
	path path_prefab = path_content.wstring() + L"prefab\\collider3d\\";

	if (false == exists(path_prefab))
	{
		create_directory(path_prefab);
	}

	FILE* pFile = nullptr;
	errno_t err = _wfopen_s(&pFile, strFilePath.c_str(), L"wb");
	assert(pFile);

	// 키값
	SaveWString(m_PrefabKey, pFile);

	fwrite(&m_vOffsetPos, sizeof(Vec3), 1, pFile);
	fwrite(&m_vOffsetScale, sizeof(Vec3), 1, pFile);
	fwrite(&m_bAbsolute, sizeof(bool), 1, pFile);
	fwrite(&m_Shape, sizeof(UINT), 1, pFile);
	fwrite(&m_bDrawCollision, sizeof(bool), 1, pFile);

	fclose(pFile);
}

void CCollider3D::LoadPrefab(const wstring& _key)
{
	// 읽기모드로 파일열기
	FILE* pFile = nullptr;

	wstring RelativePath = L"prefab\\collider3d\\";
	RelativePath += _key;
	RelativePath += L".col3prp";	// 확장자

	wstring strFilePath = CPathMgr::GetInst()->GetContentPath() + RelativePath;

	_wfopen_s(&pFile, strFilePath.c_str(), L"rb");

	if (pFile == nullptr)
	{
		return;
	}

	// 키값
	LoadWString(m_PrefabKey, pFile);

	fread(&m_vOffsetPos, sizeof(Vec3), 1, pFile);
	fread(&m_vOffsetScale, sizeof(Vec3), 1, pFile);
	fread(&m_bAbsolute, sizeof(bool), 1, pFile);
	fread(&m_Shape, sizeof(UINT), 1, pFile);
	fread(&m_bDrawCollision, sizeof(bool), 1, pFile);

	fclose(pFile);
}

void CCollider3D::SaveToLevelFile(FILE* _File)
{
	fwrite(&m_vOffsetPos, sizeof(Vec3), 1, _File);
	fwrite(&m_vOffsetScale, sizeof(Vec3), 1, _File);
	fwrite(&m_bAbsolute, sizeof(bool), 1, _File);
	fwrite(&m_Shape, sizeof(UINT), 1, _File);
	fwrite(&m_bDrawCollision, sizeof(bool), 1, _File);
}

void CCollider3D::LoadFromLevelFile(FILE* _File)
{
	fread(&m_vOffsetPos, sizeof(Vec3), 1, _File);
	fread(&m_vOffsetScale, sizeof(Vec3), 1, _File);
	fread(&m_bAbsolute, sizeof(bool), 1, _File);
	fread(&m_Shape, sizeof(UINT), 1, _File);
	fread(&m_bDrawCollision, sizeof(bool), 1, _File);
}

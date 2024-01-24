#include "pch.h"
#include "CRenderComponent.h"

#include "CResMgr.h"
#include "CTransform.h"

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
}

void CRenderComponent::render_shadowmap()
{
	Ptr<CMaterial> pShadowMapMtrl = CResMgr::GetInst()->FindRes<CMaterial>(L"ShadowMapMtrl");

	Transform()->UpdateData();

	pShadowMapMtrl->UpdateData();

	GetMesh()->render(0);
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
	COMPONENT_TYPE type = GetType();
	fwrite(&type, sizeof(UINT), 1, _File);


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

void CRenderComponent::SaveToDB(int _gameObjectID, COMPONENT_TYPE _componentType)
{
	sqlite3* db = CSQLMgr::GetInst()->GetDB();

	// 쿼리 문자열 준비
	const char* szQuery = "INSERT INTO RENDERCOMPONENT(GameObject_ID, Mesh_Key, Mesh_Path, SharedMtrl_Key, SharedMtrl_Path) VALUES (?, ?, ?, ?, ?)";
	sqlite3_stmt* stmt;

	// 쿼리 준비
	if (sqlite3_prepare_v2(db, szQuery, -1, &stmt, NULL) == SQLITE_OK) {
		sqlite3_bind_int(stmt, 1, _gameObjectID);

		wstring meshKey, meshPath, mtrlKey, mtrlPath;
		SaveResRefToDB(m_pMesh.Get(), meshKey, meshPath);
		SaveResRefToDB(m_pSharedMtrl.Get(), mtrlKey, mtrlPath);

		sqlite3_bind_text16(stmt, 2, meshKey.c_str(), -1, SQLITE_TRANSIENT);
		sqlite3_bind_text16(stmt, 3, meshPath.c_str(), -1, SQLITE_TRANSIENT);
		sqlite3_bind_text16(stmt, 4, mtrlKey.c_str(), -1, SQLITE_TRANSIENT);
		sqlite3_bind_text16(stmt, 5, mtrlPath.c_str(), -1, SQLITE_TRANSIENT);

		// 쿼리 실행
		if (sqlite3_step(stmt) != SQLITE_DONE) {
			// 에러 처리: 쿼리 실행에 실패했을 경우
			assert(false);
		}

		// 스테이트먼트 종료
		sqlite3_finalize(stmt);
	}
	else {
		// 쿼리 준비에 실패했을 경우의 처리
		assert(false);
	}
}

void CRenderComponent::LoadFromDB(int _gameObjectID)
{
	sqlite3* db = CSQLMgr::GetInst()->GetDB();
	const char* szQuery = "SELECT Mesh_Key, Mesh_Path, SharedMtrl_Key, SharedMtrl_Path FROM RENDERCOMPONENT WHERE GameObject_ID = ?";
	sqlite3_stmt* stmt;

	if (sqlite3_prepare_v2(db, szQuery, -1, &stmt, NULL) == SQLITE_OK) {
		sqlite3_bind_int(stmt, 1, _gameObjectID);

		if (sqlite3_step(stmt) == SQLITE_ROW) {
			const wchar_t* meshKey = static_cast<const wchar_t*>(sqlite3_column_text16(stmt, 0));
			const wchar_t* meshPath = static_cast<const wchar_t*>(sqlite3_column_text16(stmt, 1));
			const wchar_t* mtrlKey = static_cast<const wchar_t*>(sqlite3_column_text16(stmt, 2));
			const wchar_t* mtrlPath = static_cast<const wchar_t*>(sqlite3_column_text16(stmt, 3));

			LoadResRefFromDB(m_pMesh, meshKey, meshPath);
			LoadResRefFromDB(m_pSharedMtrl, mtrlKey, mtrlPath);

			SetMaterial(m_pSharedMtrl, 0);
		}
		else {
			// 에러 처리: 데이터를 찾지 못했거나 쿼리에 실패했을 경우
			assert(false);
		}

		sqlite3_finalize(stmt);
	}
	else {
		// 쿼리 준비에 실패했을 경우의 처리
		assert(false);
	}
}

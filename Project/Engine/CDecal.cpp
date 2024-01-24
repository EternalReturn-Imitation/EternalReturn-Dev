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

void CDecal::render(UINT _iSubset)
{
	render();
}

void CDecal::SaveToDB(int _gameObjectID, COMPONENT_TYPE _componentType)
{
	CRenderComponent::SaveToDB(_gameObjectID, _componentType);

	sqlite3* db = CSQLMgr::GetInst()->GetDB();

	// 쿼리 문자열 준비
	const char* szQuery = "INSERT INTO DECAL(GameObject_ID, Tex_Key, Tex_Path, Bool_Deferred, Bool_Emissive) VALUES (?, ?, ?, ?, ?)";
	sqlite3_stmt* stmt;

	// 쿼리 준비
	if (sqlite3_prepare_v2(db, szQuery, -1, &stmt, NULL) == SQLITE_OK) {
		// GameObject_ID 바인딩
		sqlite3_bind_int(stmt, 1, _gameObjectID);

		wstring texKey, texPath;
		SaveResRefToDB(m_DecalTex.Get(), texKey, texPath);

		sqlite3_bind_text16(stmt, 2, texKey.c_str(), -1, SQLITE_TRANSIENT);
		sqlite3_bind_text16(stmt, 3, texPath.c_str(), -1, SQLITE_TRANSIENT);
		
		sqlite3_bind_int(stmt, 4, m_bDeferred);
		sqlite3_bind_int(stmt, 5, m_bEmissive);

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

void CDecal::LoadFromDB(int _gameObjectID)
{
	CRenderComponent::LoadFromDB(_gameObjectID);

	sqlite3* db = CSQLMgr::GetInst()->GetDB();
	const char* szQuery = "SELECT Tex_Key, Tex_Path, Bool_Deferred, Bool_Emissive FROM DECAL WHERE GameObject_ID = ?";
	sqlite3_stmt* stmt;

	if (sqlite3_prepare_v2(db, szQuery, -1, &stmt, NULL) == SQLITE_OK) {
		sqlite3_bind_int(stmt, 1, _gameObjectID);

		if (sqlite3_step(stmt) == SQLITE_ROW) {
			// Tex_Key, Tex_Path 데이터 불러오기
			const wchar_t* texKey = static_cast<const wchar_t*>(sqlite3_column_text16(stmt, 0));
			const wchar_t* texPath = static_cast<const wchar_t*>(sqlite3_column_text16(stmt, 1));

			LoadResRefFromDB(m_DecalTex, texKey, texPath);

			// Bool_Deferred, Bool_Emissive 데이터 불러오기
			m_bDeferred = sqlite3_column_int(stmt, 2) != 0;
			m_bEmissive = sqlite3_column_int(stmt, 3) != 0;
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
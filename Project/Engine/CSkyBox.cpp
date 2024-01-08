#include "pch.h"
#include "CSkyBox.h"

#include "CResMgr.h"
#include "CMesh.h"
#include "CMaterial.h"

#include "CTransform.h"


CSkyBox::CSkyBox()
	: CRenderComponent(COMPONENT_TYPE::SKYBOX)
	, m_Type(SKYBOX_TYPE::SPHERE)
{
	SetSkyBoxType(m_Type);

	SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"SkyBoxMtrl"));
}

CSkyBox::~CSkyBox()
{
}

void CSkyBox::SetSkyBoxType(SKYBOX_TYPE _Type)
{
	m_Type = _Type;

	if (m_Type == SKYBOX_TYPE::SPHERE)
	{
		SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"SphereMesh"));
	}
	else
	{
		SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"CubeMesh"));
	}
}

void CSkyBox::SetSkyBoxTexture(Ptr<CTexture> _Tex)
{
	m_SkyBoxTex = _Tex;	
}

void CSkyBox::finaltick()
{

}

void CSkyBox::render()
{
	if (nullptr == GetMesh() || nullptr == GetMaterial())
		return;

	// Transform 에 UpdateData 요청
	Transform()->UpdateData();

	// 재질 업데이트
	GetMaterial()->SetScalarParam(INT_0, &m_Type);

	if (nullptr != m_SkyBoxTex)
	{
		if (m_SkyBoxTex->IsCube())
		{
			GetMaterial()->SetTexParam(TEX_CUBE_0, m_SkyBoxTex);
		}
		else
		{
			GetMaterial()->SetTexParam(TEX_0, m_SkyBoxTex);
		}
	}
	
	GetMaterial()->UpdateData();

	// 렌더
	GetMesh()->render();
}

void CSkyBox::SaveToDB(int _gameObjectID, COMPONENT_TYPE _componentType)
{
	CRenderComponent::SaveToDB(_gameObjectID, _componentType);

	sqlite3* db = CSQLMgr::GetInst()->GetDB();

	// 쿼리 문자열 준비
	const char* szQuery = "INSERT INTO SKYBOX(GameObject_ID, Type, Tex_Key, Tex_Path) VALUES (?, ?, ?, ?)";
	sqlite3_stmt* stmt;

	// 쿼리 준비
	if (sqlite3_prepare_v2(db, szQuery, -1, &stmt, NULL) == SQLITE_OK) {
		// GameObject_ID 바인딩
		sqlite3_bind_int(stmt, 1, _gameObjectID);

		sqlite3_bind_int(stmt, 2, (int)m_Type);

		wstring texKey, texPath;
		SaveResRefToDB(m_SkyBoxTex.Get(), texKey, texPath);

		sqlite3_bind_text16(stmt, 3, texKey.c_str(), -1, SQLITE_TRANSIENT);
		sqlite3_bind_text16(stmt, 4, texPath.c_str(), -1, SQLITE_TRANSIENT);

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

void CSkyBox::LoadFromDB(int _gameObjectID)
{
	CRenderComponent::LoadFromDB(_gameObjectID);

	sqlite3* db = CSQLMgr::GetInst()->GetDB();
	const char* szQuery = "SELECT Type, Tex_Key, Tex_Path FROM SKYBOX WHERE GameObject_ID = ?";
	sqlite3_stmt* stmt;

	if (sqlite3_prepare_v2(db, szQuery, -1, &stmt, NULL) == SQLITE_OK) {
		sqlite3_bind_int(stmt, 1, _gameObjectID);

		if (sqlite3_step(stmt) == SQLITE_ROW) {
			m_Type = static_cast<SKYBOX_TYPE>(sqlite3_column_int(stmt, 0));

			const wchar_t* texKey = static_cast<const wchar_t*>(sqlite3_column_text16(stmt, 1));
			const wchar_t* texPath = static_cast<const wchar_t*>(sqlite3_column_text16(stmt, 2));
			
			LoadResRefFromDB(m_SkyBoxTex, texKey, texPath);
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

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

	if (m_bShowRange || m_bDebug)//π¸¿ß ∫∏¿Ã¥¬∞‘ true¿œãö∏∏ ∫∏¿Ã∞‘
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

	// ±§ø¯ø° ∫Œ¬¯«— ƒ´∏ﬁ∂Û ø¿∫Í¡ß∆Æµµ ¿ßƒ°∏¶ ±§ø¯ ¿ßƒ°∂˚ µø¿œ«œ∞‘..
	// finaltick »£√‚Ω√ƒ—º≠ ƒ´∏ﬁ∂Û ø¿∫Í¡ß∆Æ¿« ƒ´∏ﬁ∂Û ƒƒ∆˜≥Õ∆Æ¿« view, proj «‡∑ƒ ø¨ªÍ«“ºˆ ¿÷∞‘ «‘
	*m_pCamObj->Transform() = *Transform();
	m_pCamObj->finaltick_module();
}

void CLight3D::render()
{
	Transform()->UpdateData();

	// Light ¿Á¡˙ æ˜µ•¿Ã∆Æ
	m_Mtrl->SetScalarParam(INT_0, &m_LightIdx);
	m_Mtrl->UpdateData();

	if (m_LightInfo.LightType == (UINT)LIGHT_TYPE::DIRECTIONAL)
	{
		Matrix matVP = m_pCamObj->Camera()->GetViewMat() * m_pCamObj->Camera()->GetProjMat();
		m_Mtrl->SetScalarParam(MAT_0, &matVP);
		m_Mtrl->SetTexParam(TEX_2, CResMgr::GetInst()->FindRes<CTexture>(L"DynamicShadowMapTex"));
	}

	// ∫º∑˝ ∏ﬁΩ√ ∑ª¥ı
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

	// SphereMesh ¿« ∑Œƒ√ π›¡ˆ∏ß¿Ã 0.5f ¿Ã±‚ ∂ßπÆø° 2πË∑Œ ¿˚øÎ
	if (m_LightInfo.LightType != (UINT)LIGHT_TYPE::DIRECTIONAL)
		Transform()->SetRelativeScale(Vec3(_Radius * 2.f, _Radius * 2.f, _Radius * 2.f));
}

void CLight3D::SetLightType(LIGHT_TYPE _type)
{
	m_LightInfo.LightType = (int)_type;

	if (LIGHT_TYPE::DIRECTIONAL == (LIGHT_TYPE)m_LightInfo.LightType)
	{
		// ±§ø¯¿ª ∑ª¥ı∏µ «“ ∂ß, ±§ø¯¿« øµ«‚π¸¿ß∏¶ «¸ªÛ»≠ «“ ºˆ ¿÷¥¬ ∏ﬁΩ¨(∫º∑˝∏ﬁΩ¨) ∏¶ º±≈√
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
}

void CLight3D::SaveToDB(int _gameObjectID, COMPONENT_TYPE _componentType)
{
	sqlite3* db = CSQLMgr::GetInst()->GetDB();

	const char* szQuery = "INSERT INTO LIGHT3D(GameObject_ID, LightInfo, Mesh_Key, Mesh_Path, Mtrl_Key, Mtrl_Path, Light_Idx) VALUES (?, ?, ?, ?, ?, ?, ?)";
	sqlite3_stmt* stmt;

	if (sqlite3_prepare_v2(db, szQuery, -1, &stmt, NULL) == SQLITE_OK) {
		sqlite3_bind_int(stmt, 1, _gameObjectID);
		sqlite3_bind_blob(stmt, 2, &m_LightInfo, sizeof(m_LightInfo), SQLITE_STATIC);

		wstring meshKey, meshPath, mtrlKey, mtrlPath;
		SaveResRefToDB(m_Mesh.Get(), meshKey, meshPath);
		SaveResRefToDB(m_Mtrl.Get(), mtrlKey, mtrlPath);

		sqlite3_bind_text16(stmt, 3, meshKey.c_str(), -1, SQLITE_TRANSIENT);
		sqlite3_bind_text16(stmt, 4, meshPath.c_str(), -1, SQLITE_TRANSIENT);
		sqlite3_bind_text16(stmt, 5, mtrlKey.c_str(), -1, SQLITE_TRANSIENT);
		sqlite3_bind_text16(stmt, 6, mtrlPath.c_str(), -1, SQLITE_TRANSIENT);

		sqlite3_bind_int(stmt, 7, m_LightIdx);

		if (sqlite3_step(stmt) != SQLITE_DONE) {
			assert(false);
		}

		sqlite3_finalize(stmt);
	}
	else {
		assert(false);
	}
}

void CLight3D::LoadFromDB(int _gameObjectID)
{
	sqlite3* db = CSQLMgr::GetInst()->GetDB();
	const char* szQuery = "SELECT LightInfo, Mesh_Key, Mesh_Path, Mtrl_Key, Mtrl_Path, Light_Idx FROM LIGHT3D WHERE GameObject_ID = ?";
	sqlite3_stmt* stmt;

	if (sqlite3_prepare_v2(db, szQuery, -1, &stmt, NULL) == SQLITE_OK) {
		sqlite3_bind_int(stmt, 1, _gameObjectID);

		if (sqlite3_step(stmt) == SQLITE_ROW) {
			const void* data = sqlite3_column_blob(stmt, 0);
			int bytes = sqlite3_column_bytes(stmt, 0);

			// tLightInfoÎ°?Î≥Ä??
			if (bytes == sizeof(tLightInfo)) {
				memcpy(&m_LightInfo, data, bytes);
			}
			else {
				assert(false);
			}

			// Mesh_Key, Mesh_Path, Mtrl_Key, Mtrl_Path ?∞Ïù¥??Î∂àÎü¨?§Í∏∞
			const wchar_t* meshKey = static_cast<const wchar_t*>(sqlite3_column_text16(stmt, 1));
			const wchar_t* meshPath = static_cast<const wchar_t*>(sqlite3_column_text16(stmt, 2));
			const wchar_t* mtrlKey = static_cast<const wchar_t*>(sqlite3_column_text16(stmt, 3));
			const wchar_t* mtrlPath = static_cast<const wchar_t*>(sqlite3_column_text16(stmt, 4));

			LoadResRefFromDB(m_Mesh, meshKey, meshPath);
			LoadResRefFromDB(m_Mtrl, mtrlKey, mtrlPath);

			// Light_Idx ?∞Ïù¥??Î∂àÎü¨?§Í∏∞
			m_LightIdx = sqlite3_column_int(stmt, 5);
		}
		else {
			assert(false);
		}
		sqlite3_finalize(stmt);
	}
	else {
		assert(false);
	}
}
#include "pch.h"
#include "CNaviMap.h"

#include "CResMgr.h"
#include "CComputeShader.h"
#include "CStructuredBuffer.h"

#include "CKeyMgr.h"
#include "CCamera.h"
#include "CRenderMgr.h"
#include "CTransform.h"
#include "CRenderComponent.h"
#include "CPathFindMgr.h"
#include "CTexture.h"

CNaviMap::CNaviMap()
	:CComponent(COMPONENT_TYPE::NAVIMAP)
	, m_bTrigger(true)
	, m_sResultPos{}
{
	begin();
}

CNaviMap::~CNaviMap()
{
	if (nullptr != m_pCrossBuffer)
		delete m_pCrossBuffer;
}

void CNaviMap::begin()
{
	// =====================
	// 레이케스팅 셰이더 생성
	// =====================
	m_pCSRaycast = (CRaycastShader*)CResMgr::GetInst()->FindRes<CComputeShader>(L"RaycastBaseShader").Get();
	if (nullptr == m_pCSRaycast)
	{
		m_pCSRaycast = new CRaycastShader(1, 1, 1);
		m_pCSRaycast->CreateComputeShader(L"shader\\RaycastBase.fx", "CS_Raycast");
		CResMgr::GetInst()->AddRes<CComputeShader>(L"RaycastBaseShader", m_pCSRaycast.Get());
	}

	//레이케스팅 용 텍스쳐 넣어주기.
	//Todo

	// 레이캐스팅 결과 받는 버퍼
	m_pCrossBuffer = new CStructuredBuffer;
	m_pCrossBuffer->Create(sizeof(tRaycastOut), 1, SB_TYPE::READ_WRITE, true);

	//네비맵 텍스트 세팅
	m_pNaviTex = CResMgr::GetInst()->FindRes<CTexture>(L"NaviMap_Result.png");
	m_pCSRaycast->SetNaviMapTex(m_pNaviTex);
}

void CNaviMap::tick()
{
}

void CNaviMap::finaltick()
{
	if (m_bTrigger) {
		//NaviMap의 정점데이터와 인덱스데이터를 가져옴
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
		m_fMinMaxArr[0] = xMin;
		m_fMinMaxArr[1] = xMax;
		m_fMinMaxArr[2] = yMin;
		m_fMinMaxArr[3] = yMax;
		m_fMinMaxArr[4] = zMin;
		m_fMinMaxArr[5] = zMax;

		m_pCSRaycast->SetXMinMax(Vec2(xMin, xMax));
		m_pCSRaycast->SetYMinMax(Vec2(yMin, yMax));
		m_pCSRaycast->SetZMinMax(Vec2(zMin, zMax));

		m_pCSRaycast->SetMaxScale(GetOwner()->Transform()->GetRelativeScale());
		m_pCSRaycast->SetVtx(vVtx);

		CPathFindMgr::GetInst()->SetNaviVtx(vVtx);
		CPathFindMgr::GetInst()->SetNavMeshScale(GetOwner()->Transform()->GetRelativeScale());

		m_bTrigger = false;

		xMin = CTruncate(xMin, 8);
		yMax = CTruncate(yMax, 8);

		GetOwner()->Transform()->SetOffsetRelativePos(Vec3(-xMin, 0.f, yMax));
		GetOwner()->Transform()->SetOffsetRelativePos(Vec3(xMin, 0.f, yMin));
		GetOwner()->Transform()->SetOffsetTrigger(true);		
	}

	m_pCSRaycast->SetPos(GetOwner()->Transform()->GetRelativePos());

	if (KEY_PRESSED(KEY::LBTN))
	{
		Raycasting();
	}

	//if (KEY_TAP(KEY::LCTRL)) {
	//	Ptr<CTexture> tex = CResMgr::GetInst()->FindRes<CTexture>(L"DataTargetTex");
	//	int a = tex->Save(L"texture\\navimap_fault");
	//
	//	int b = 0;
	//}

	//if (KEY_TAP(KEY::LCTRL) && KEY_TAP(KEY::P)) {
	//	Ptr<CTexture> tex = CResMgr::GetInst()->Load
	//}
}

void CNaviMap::Raycasting()
{
	// 시점 카메라를 가져옴
	CCamera* pMainCam = CRenderMgr::GetInst()->GetMainCam();
	if (nullptr == pMainCam)
		return;

	// 월드 기준 광선을 지형의 로컬로 보냄
	const Matrix& matWorldInv = Transform()->GetWorldInvMat();
	const tRay& ray = pMainCam->GetRay();

	tRay CamRay = {};
	CamRay.vStart = XMVector3TransformCoord(ray.vStart, matWorldInv);
	CamRay.vDir = XMVector3TransformNormal(ray.vDir, matWorldInv);
	CamRay.vDir.Normalize();

	// 지형과 카메라 Ray 의 교점을 구함
	tRaycastOut out = { Vec2(0.f, 0.f), (float)0x7fffffff, 0, Vec4(0.f,0.f,0.f,0.f) };
	m_pCrossBuffer->SetData(&out, 1);

	CCamera* camera = CRenderMgr::GetInst()->GetMainCam();
	m_pCSRaycast->SetCameraRay(camera->CalRay());
	m_pCSRaycast->SetOuputBuffer(m_pCrossBuffer);

	m_pCSRaycast->Execute();

	m_pCrossBuffer->GetData(&out);

	if (out.bSuccess) {
		Vec3 pos = GetOwner()->Transform()->GetRelativePos();
		Vec3 posA = GetOwner()->Transform()->GetOffsetRelativePos();
		m_sResultPos.resultPos.x = ((abs(m_fMinMaxArr[0]) + abs(m_fMinMaxArr[1])) * out.vUV.x) + pos.x;
		m_sResultPos.resultPos.y = ((abs(m_fMinMaxArr[4]) + abs(m_fMinMaxArr[5])) * 0.f) + pos.y;
		m_sResultPos.resultPos.z = ((abs(m_fMinMaxArr[2]) + abs(m_fMinMaxArr[3])) * out.vUV.y) + pos.z;
		m_sResultPos.bSuccess = true;
		
		CPathFindMgr::GetInst()->SetNaviResult(m_sResultPos);
	}
	else {
		m_sResultPos.resultPos.x = 0.f;
		m_sResultPos.resultPos.y = 0.f;
		m_sResultPos.resultPos.z = 0.f;
		m_sResultPos.bSuccess = false;
	}
}

void CNaviMap::SaveToLevelFile(FILE* _File)
{
}

void CNaviMap::LoadFromLevelFile(FILE* _FILE)
{
}

void CNaviMap::SaveToDB(int _gameObjectID, COMPONENT_TYPE _componentType)
{
}

void CNaviMap::LoadFromDB(int _gameObjectID)
{
}

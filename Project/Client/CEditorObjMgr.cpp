#include "pch.h"
#include "CEditorObjMgr.h"



#include "CGameObjectEx.h"
#include "CAnimEditObj.h"
#include <Engine\components.h>

#include <Engine\CResMgr.h>
#include <Engine\CRenderMgr.h>
#include <Engine\CTimeMgr.h>
#include <Engine\CKeyMgr.h>
#include <Engine\CRenderMgr.h>
#include <Engine\CMRT.h>

#include <Script\CEditCamControlScript.h>

CEditorObjMgr::CEditorObjMgr()
	: m_DebugShape{}
	, m_pTexRenderObj(nullptr)
	, m_bRenderTex(false)
{

}

CEditorObjMgr::~CEditorObjMgr()
{
	Safe_Del_Vec(m_vecEditorObj);
	Safe_Del_Array(m_DebugShape);
	
	if (nullptr != m_pTexRenderObj)
		delete m_pTexRenderObj;
}

void CEditorObjMgr::init()
{
	// 디버그 쉐이프 생성
	m_DebugShape[(UINT)SHAPE_TYPE::RECT] = new CGameObjectEx;
	m_DebugShape[(UINT)SHAPE_TYPE::RECT]->AddComponent(new CTransform);
	m_DebugShape[(UINT)SHAPE_TYPE::RECT]->AddComponent(new CMeshRender);
	m_DebugShape[(UINT)SHAPE_TYPE::RECT]->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh_Debug"));
	m_DebugShape[(UINT)SHAPE_TYPE::RECT]->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"DebugShapeMtrl"), 0);

	m_DebugShape[(UINT)SHAPE_TYPE::CIRCLE] = new CGameObjectEx;
	m_DebugShape[(UINT)SHAPE_TYPE::CIRCLE]->AddComponent(new CTransform);
	m_DebugShape[(UINT)SHAPE_TYPE::CIRCLE]->AddComponent(new CMeshRender);
	m_DebugShape[(UINT)SHAPE_TYPE::CIRCLE]->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"CircleMesh_Debug"));
	m_DebugShape[(UINT)SHAPE_TYPE::CIRCLE]->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"DebugShapeMtrl"), 0);

	m_DebugShape[(UINT)SHAPE_TYPE::CUBE] = new CGameObjectEx;
	m_DebugShape[(UINT)SHAPE_TYPE::CUBE]->AddComponent(new CTransform);
	m_DebugShape[(UINT)SHAPE_TYPE::CUBE]->AddComponent(new CMeshRender);
	m_DebugShape[(UINT)SHAPE_TYPE::CUBE]->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"CubeMesh_Debug"));
	m_DebugShape[(UINT)SHAPE_TYPE::CUBE]->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"DebugShapeMtrl"), 0);

	m_DebugShape[(UINT)SHAPE_TYPE::SPHERE] = new CGameObjectEx;
	m_DebugShape[(UINT)SHAPE_TYPE::SPHERE]->AddComponent(new CTransform);
	m_DebugShape[(UINT)SHAPE_TYPE::SPHERE]->AddComponent(new CMeshRender);
	m_DebugShape[(UINT)SHAPE_TYPE::SPHERE]->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"SphereMesh"));
	m_DebugShape[(UINT)SHAPE_TYPE::SPHERE]->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"DebugShapeSphereMtrl"), 0);

	m_DebugShape[(UINT)SHAPE_TYPE::FRUSTUM] = new CGameObjectEx;
	m_DebugShape[(UINT)SHAPE_TYPE::FRUSTUM]->AddComponent(new CTransform);
	m_DebugShape[(UINT)SHAPE_TYPE::FRUSTUM]->AddComponent(new CMeshRender);
	m_DebugShape[(UINT)SHAPE_TYPE::FRUSTUM]->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"FrustumMesh_Debug"));
	m_DebugShape[(UINT)SHAPE_TYPE::FRUSTUM]->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"DebugShapeFrustumMtrl"), 0);

	// EditorObject 생성
	CGameObjectEx* pEditorCamObj = new CGameObjectEx;
	pEditorCamObj->AddComponent(new CTransform);
	pEditorCamObj->AddComponent(new CCamera);
	pEditorCamObj->AddComponent(new CEditCamControlScript);

	pEditorCamObj->Camera()->SetFOV(Deg2Rad(30));
	pEditorCamObj->Camera()->SetLayerMaskAll(true);
	pEditorCamObj->Camera()->SetProjType(PROJ_TYPE::PERSPECTIVE);
	pEditorCamObj->Camera()->SetMainCamera();

	pEditorCamObj->Transform()->SetRelativeRot(Vec3(Deg2Rad(54.f), Deg2Rad(-45.f), 0.f));
	pEditorCamObj->Transform()->SetRelativePos(Vec3(0.f, 35.f, 0.f));
	pEditorCamObj->Camera()->SetFar(10000.f);


	m_vecEditorObj.push_back(pEditorCamObj);
	//Vec3 pos = pEditorCamObj->Transform()->GetRelativePos();
	//Vec3 rot = pEditorCamObj->Transform()->GetRelativePos();
	//rot.y = rot.y = -1.5708f;
	//pEditorCamObj->Transform()->SetRelativeRot(rot);
	CRenderMgr::GetInst()->RegisterEditorCamera(pEditorCamObj->Camera());
}

void CEditorObjMgr::progress()
{
	// DebugShape 정보 가져오기
	vector<tDebugShapeInfo>& vecInfo = CRenderMgr::GetInst()->GetDebugShapeInfo();
	m_DebugShapeInfo.insert(m_DebugShapeInfo.end(), vecInfo.begin(), vecInfo.end());
	vecInfo.clear();


	tick();

	render();
}

void CEditorObjMgr::tick()
{
	for (size_t i = 0; i < m_vecEditorObj.size(); ++i)
	{
		m_vecEditorObj[i]->tick();
	}
	

	for (size_t i = 0; i < m_vecEditorObj.size(); ++i)
	{
		m_vecEditorObj[i]->finaltick();
	}
}

void CEditorObjMgr::render()
{
	for (size_t i = 0; i < m_vecEditorObj.size(); ++i)
	{
		m_vecEditorObj[i]->render();
	}

	// DebugShape Render
	CGameObjectEx* pShapeObj = nullptr;

	vector<tDebugShapeInfo>::iterator iter = m_DebugShapeInfo.begin();
	for (; iter != m_DebugShapeInfo.end();)
	{
		switch (iter->eShape)
		{
		case SHAPE_TYPE::RECT:
			pShapeObj = m_DebugShape[(UINT)SHAPE_TYPE::RECT];
			break;
		case SHAPE_TYPE::CIRCLE:
			pShapeObj = m_DebugShape[(UINT)SHAPE_TYPE::CIRCLE];
			break;
		case SHAPE_TYPE::CUBE:
			pShapeObj = m_DebugShape[(UINT)SHAPE_TYPE::CUBE];
			break;
		case SHAPE_TYPE::SPHERE:
			pShapeObj = m_DebugShape[(UINT)SHAPE_TYPE::SPHERE];
			break;		
		case SHAPE_TYPE::FRUSTUM:
			pShapeObj = m_DebugShape[(UINT)SHAPE_TYPE::FRUSTUM];
			break;
		}

		if (iter->matWorld != XMMatrixIdentity())
		{
			pShapeObj->Transform()->SetWorldMat(iter->matWorld);
		}
		else
		{
			pShapeObj->Transform()->SetRelativePos(iter->vWorldPos);
			pShapeObj->Transform()->SetRelativeScale(iter->vWorldScale);
			pShapeObj->Transform()->SetRelativeRot(iter->vWorldRotation);
			pShapeObj->finaltick();
		}

		pShapeObj->MeshRender()->GetMaterial(0)->SetScalarParam(VEC4_0, &iter->vColor);

		if (iter->bDepthTest)
			pShapeObj->MeshRender()->GetMaterial(0)->GetShader()->SetDSType(DS_TYPE::LESS);
		else
			pShapeObj->MeshRender()->GetMaterial(0)->GetShader()->SetDSType(DS_TYPE::NO_TEST_NO_WRITE);

		pShapeObj->render();

		iter->fCurTime += DT;
		if (iter->fMaxTime < iter->fCurTime)
		{
			iter = m_DebugShapeInfo.erase(iter);
		}
		else
		{
			++iter;
		}
	}

	if (m_pTexRenderObj)
	{
		m_pTexRenderObj->tick();
		m_pTexRenderObj->update();

		m_pTexRenderObj->render();
	}
}

void CEditorObjMgr::SetTexRender(CAnimEditObj* _pObj)
{
	if (nullptr != m_pTexRenderObj)
		m_pTexRenderObj = nullptr;

	m_pTexRenderObj = _pObj;
}
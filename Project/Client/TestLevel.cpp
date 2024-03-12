#include "pch.h"
#include "TestLevel.h"

#include <Engine\CLevelMgr.h>
#include <Engine\CLevel.h>
#include <Engine\CLayer.h>
#include <Engine\CGameObject.h>
#include <Engine\components.h>

#include <Engine\CResMgr.h>
#include <Engine\CCollisionMgr.h>

#include <Script\CPlayerScript.h>
#include <Script\CMonsterScript.h>
#include <Script\CCameraMoveScript.h>

#include "CLevelSaveLoad.h"


#include <Engine/CSetColorShader.h>

#include "CEditorObjMgr.h"

#include <Script/CFindPathScript.h>
#include <Engine/CFindPath.h>
#include <Engine/CPathFindMgr.h>

#include <Engine\CCollider2D.h>
#include <Engine/CCollider3D.h>


void CreateTestLevel()
{
	CLevel* pCurLevel = CLevelMgr::GetInst()->GetCurLevel();
	pCurLevel->ChangeState(LEVEL_STATE::STOP);

	// Layer 이름설정
	pCurLevel->GetLayer(0)->SetName(L"Default");
	pCurLevel->GetLayer(1)->SetName(L"SkyBox");
	pCurLevel->GetLayer(2)->SetName(L"Base");
	pCurLevel->GetLayer(3)->SetName(L"Building");
	pCurLevel->GetLayer(4)->SetName(L"Roof");
	pCurLevel->GetLayer(5)->SetName(L"Monster");
	pCurLevel->GetLayer(30)->SetName(L"NaviMap");
	//pCurLevel->GetLayer(31)->SetName(L"ViewPort UI");


	// Main Camera Object 생성
	CGameObject* pMainCam = new CGameObject;
	pMainCam->SetName(L"MainCamera");

	pMainCam->AddComponent(new CTransform);
	pMainCam->AddComponent(new CCamera);
	pMainCam->AddComponent(new CCameraMoveScript);

	pMainCam->Camera()->SetProjType(PROJ_TYPE::PERSPECTIVE);
	pMainCam->Camera()->SetCameraIndex(0);		// MainCamera 로 설정
	pMainCam->Camera()->SetLayerMaskAll(true);	// 모든 레이어 체크
	pMainCam->Camera()->SetLayerMask(30, false);// UI Layer 는 렌더링하지 않는다.
	pMainCam->Camera()->SetLayerMask(31, false);// UI Layer 는 렌더링하지 않는다.

	SpawnGameObject(pMainCam, Vec3(0.f, 0.f, 0.f), 0);

	// UI cameara
	// CGameObject* pUICam = new CGameObject;
	// pUICam->SetName(L"UICamera");
	// 
	// pUICam->AddComponent(new CTransform);
	// pUICam->AddComponent(new CCamera);
	// 
	// pUICam->Camera()->SetProjType(PROJ_TYPE::ORTHOGRAPHIC);
	// pUICam->Camera()->SetCameraIndex(1);		// Sub 카메라로 지정
	// pUICam->Camera()->SetLayerMask(31, true);	// 31번 레이어만 체크
	// 
	// SpawnGameObject(pUICam, Vec3(0.f, 0.f, 0.f), 0);


	//SkyBox 추가11
	CGameObject* pSkyBox = new CGameObject;
	pSkyBox->SetName(L"SkyBox");

	pSkyBox->AddComponent(new CTransform);
	pSkyBox->AddComponent(new CSkyBox);

	pSkyBox->Transform()->SetRelativeScale(Vec3(100.f, 100.f, 100));
	pSkyBox->SkyBox()->SetSkyBoxType(SKYBOX_TYPE::SPHERE);
	pSkyBox->SkyBox()->SetSkyBoxTexture(CResMgr::GetInst()->FindRes<CTexture>(L"Sky01.png"));

	SpawnGameObject(pSkyBox, Vec3(0.f, 0.f, 0.f), L"SkyBox");

	// 광원 추가
	CGameObject* pLightObj = new CGameObject;
	pLightObj->SetName(L"Directional Light");

	pLightObj->AddComponent(new CTransform);
	pLightObj->AddComponent(new CLight3D);

	pLightObj->Transform()->SetRelativeRot(Vec3(XM_PI / 4.f, XM_PI / 4.f, 0.f));
	pLightObj->Light3D()->SetLightType(LIGHT_TYPE::DIRECTIONAL);
	pLightObj->Light3D()->SetRadius(500.f);
	pLightObj->Light3D()->SetLightColor(Vec3(1.f, 1.f, 1.f));
	pLightObj->Light3D()->SetLightAmbient(Vec3(0.f, 0.f, 0.f));

	//SpawnGameObject(pLightObj, Vec3(-2000, 2000.f, -2000.f), 0);
	SpawnGameObject(pLightObj, Vec3(0.f, 5000.f, 0.f), 0);

	// 오브젝트 생성
	// CGameObject* pObject = new CGameObject;
	// pObject->SetName(L"Particle");
	// pObject->AddComponent(new CTransform);
	// pObject->AddComponent(new CParticleSystem);
	// 
	// SpawnGameObject(pObject, Vec3(0.f, 100, 500.f), L"Particle");
	// 
	// pObject = new CGameObject;
	// pObject->SetName(L"Plane");
	// pObject->AddComponent(new CTransform);
	// pObject->AddComponent(new CMeshRender);
	// 
	// pObject->Transform()->SetRelativeScale(Vec3(1000.f, 1000.f, 1000.f));
	// pObject->Transform()->SetRelativeRot(Vec3(XM_PI / 2.f, 0.f, 0.f));
	// 
	// pObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	// pObject->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std3D_DeferredMtrl"), 0);
	// pObject->MeshRender()->GetMaterial(0)->SetTexParam(TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"TILE_01.tga"));
	// pObject->MeshRender()->GetMaterial(0)->SetTexParam(TEX_1, CResMgr::GetInst()->FindRes<CTexture>(L"TILE_01_N.tga"));

	// SpawnGameObject(pObject, Vec3(0.f, -100, 0.f), L"Default");

	CGameObject* pObject = new CGameObject;
	pObject->SetName(L"Plane");
	pObject->AddComponent(new CTransform);
	pObject->AddComponent(new CMeshRender);
	pObject->AddComponent(new CFindPath);
	
	pObject->Transform()->SetRelativeScale(Vec3(100.f, 100.f, 500.f));
	pObject->Transform()->SetRelativeScale(Vec3(1.f, 2.f, 1.f));
	pObject->Transform()->SetRelativeRot(Vec3(XM_PI / 2.f, 0.f, 0.f));
	
	pObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"CubeMesh"));
	pObject->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std3D_DeferredMtrl"), 0);
	
	//SpawnGameObject(pObject, Vec3(7000.f, 0.f, 6500.f), L"Monster");
	SpawnGameObject(pObject, Vec3(000.f, 0.f, 000.f), L"Monster");

	//pObject = new CGameObject;
	//pObject->SetName(L"MapCollider");
	//pObject->AddComponent(new CTransform);
	//pObject->AddComponent(new CCollider2D);
	//pObject->Collider2D()->SetOffsetRot(Vec3(XMConvertToRadians(90.f), 0.f, 0.f));
	//pObject->Collider2D()->SetOffsetScale(Vec2(2700.f, 2700.f));
	//pObject->Collider2D()->SetOffsetPos(Vec3(1125.f, 0.f, 1200.f));
	////pObject->Collider2D()->SetDrawCollision(false);
	////pObject->Transform()->SetGizmoObjExcept(true);
	//CPathFindMgr::GetInst()->SetMapCollider(pObject);
	//SpawnGameObject(pObject, Vec3(0.f, 0.f, 0.f), L"Monster");

	pObject = new CGameObject;
	pObject->SetName(L"TestCollider2D01");
	pObject->AddComponent(new CTransform);
	pObject->AddComponent(new CCollider3D);
	pObject->Collider3D()->SetOffsetScale(Vec3(10.f, 10.f, 10.f));
	pObject->Collider3D()->SetOffsetPos(Vec3(0.f, 0.f, 0.f));
	pObject->Collider3D()->SetCollider3DType(COLLIDER3D_TYPE::CUBE);
	CPathFindMgr::GetInst()->SetMapCollider(pObject);
	SpawnGameObject(pObject, Vec3(10.f, 1.f, 10.f), L"Monster");
	
	pObject = new CGameObject;
	pObject->SetName(L"TestCollider2D02");
	pObject->AddComponent(new CTransform);
	pObject->AddComponent(new CCollider3D);
	pObject->Collider3D()->SetOffsetScale(Vec3(10.f, 10.f, 10.f));
	pObject->Collider3D()->SetOffsetPos(Vec3(0.f, 0.f, 0.f));
	pObject->Collider3D()->SetCollider3DType(COLLIDER3D_TYPE::CUBE);
	CPathFindMgr::GetInst()->SetMapCollider(pObject);
	SpawnGameObject(pObject, Vec3(0.f, 1.f, 0.f), L"Monster");

	Ptr<CMeshData> pMeshData = nullptr;
	CGameObject* pObj = nullptr;
	Vec3 rot;
#pragma region Archery
	pMeshData = CResMgr::GetInst()->FindRes<CMeshData>(L"Archery_Roof.mdat");
	pObj = pMeshData->Instantiate();
	pObj->SetName(L"Archery_Roof");
	pObj->Transform()->SetRelativeScale(1.0f, 1.0f, 1.0f);
	pObj->Transform()->SetRelativeScale(0.01f, 0.01f, 0.01f);
	//SpawnGameObject(pObj, Vec3(5375.f, 980.f, 1745.f), L"Roof");
	SpawnGameObject(pObj, Vec3(-133.18f, 4.660f, 43.730f), L"Roof");
	
	pMeshData = CResMgr::GetInst()->FindRes<CMeshData>(L"Archery_Building.mdat");
	pObj = pMeshData->Instantiate();
	pObj->SetName(L"Archery_Building");
	rot = pObj->Transform()->GetRelativeRot();
	rot.x = -1.5708f;
	pObj->Transform()->SetRelativeRot(rot);
	pObj->Transform()->SetRelativeScale(1.0f, 1.0f, 1.0f);
	pObj->Transform()->SetRelativeScale(0.01f, 0.01f, 0.01f);
	//SpawnGameObject(pObj, Vec3(18755.f, 410.f, -2590.f), L"Building");
	SpawnGameObject(pObj, Vec3(0.2f, -1.f, 0.5f), L"Building");
	
	pMeshData = CResMgr::GetInst()->FindRes<CMeshData>(L"Archery_Base.mdat");
	pObj = pMeshData->Instantiate();
	pObj->SetName(L"Archery_Base");
	pObj->Transform()->SetRelativeScale(1.0f, 1.0f, 1.0f);
	pObj->Transform()->SetRelativeScale(0.01f, 0.01f, 0.01f);
	//SpawnGameObject(pObj, Vec3(0.f, 0.f, 0.f), L"Base");
	SpawnGameObject(pObj, Vec3(-187.f, -5.2f, 26.f), L"Base");
#pragma endregion

#pragma region Forest
	pMeshData = CResMgr::GetInst()->FindRes<CMeshData>(L"Forest_Base.mdat");
	pObj = pMeshData->Instantiate();
	pObj->SetName(L"Forest_Base");
	pObj->Transform()->SetRelativeScale(1.0f, 1.0f, 1.0f);
	pObj->Transform()->SetRelativeScale(0.01f, 0.01f, 0.01f);
	//SpawnGameObject(pObj, Vec3(13341.f, -389.f, -4629.f), L"Base");
	//SpawnGameObject(pObj, Vec3(9783.f, -824.f, 6715.f), L"Base");
	SpawnGameObject(pObj, Vec3(-53.680f, -8.500f, -20.202f), L"Base");
	 
	pMeshData = CResMgr::GetInst()->FindRes<CMeshData>(L"Forest_Building.mdat");
	pObj = pMeshData->Instantiate();
	pObj->SetName(L"Forest_Building");
	pObj->Transform()->SetRelativeScale(1.0f, 1.0f, 1.0f);
	pObj->Transform()->SetRelativeScale(0.01f, 0.01f, 0.01f);
	//SpawnGameObject(pObj, Vec3(17568.f, 422.f, -4707.f), L"Building");
	//SpawnGameObject(pObj, Vec3(14010.f, -13.f, 6637.f), L"Building");
	SpawnGameObject(pObj, Vec3(-11.632f, -0.427f, -21.176f), L"Building");
#pragma endregion

#pragma region Hotel
	pMeshData = CResMgr::GetInst()->FindRes<CMeshData>(L"Hotel_Base.mdat");
	pObj = pMeshData->Instantiate();
	pObj->SetName(L"Hotel_Base");
	pObj->Transform()->SetRelativeScale(1.0f, 1.0f, 1.0f);
	pObj->Transform()->SetRelativeScale(0.01f, 0.01f, 0.01f);
	//SpawnGameObject(pObj, Vec3(9070.f, 230.f, 15030.f), L"Base");
	//SpawnGameObject(pObj, Vec3(5512.f, -205.f, 26374.f), L"Base");
	SpawnGameObject(pObj, Vec3(-96.4f, -2.32f, 176.4f), L"Base");
	
	pMeshData = CResMgr::GetInst()->FindRes<CMeshData>(L"Hotel_Building.mdat");
	pObj = pMeshData->Instantiate();
	pObj->SetName(L"Hotel_Building");
	pObj->Transform()->SetRelativeScale(1.0f, 1.0f, 1.0f);
	pObj->Transform()->SetRelativeScale(0.01f, 0.01f, 0.01f);
	//SpawnGameObject(pObj, Vec3(4606.f, 94.f, -4699.f), L"Building");
	//SpawnGameObject(pObj, Vec3(1048.f, -341.f, 6645.f), L"Building");
	SpawnGameObject(pObj, Vec3(-141.f, -3.6f, -21.35f), L"Building");
	
	pMeshData = CResMgr::GetInst()->FindRes<CMeshData>(L"Hotel_Roof.mdat");
	pObj = pMeshData->Instantiate();
	pObj->SetName(L"Hotel_Roof");
	pObj->Transform()->SetRelativeScale(1.0f, 1.0f, 1.0f);
	pObj->Transform()->SetRelativeScale(0.01f, 0.01f, 0.01f);
	//SpawnGameObject(pObj, Vec3(8532.f, 365.f, -4365.f), L"Roof");
	//SpawnGameObject(pObj, Vec3(4974.f, -70.f, 6979.f), L"Roof");
	SpawnGameObject(pObj, Vec3(-101.72f, -0.910f, -18.075f), L"Roof");
#pragma endregion

#pragma region SandyBeach
	pMeshData = CResMgr::GetInst()->FindRes<CMeshData>(L"SandyBeach_Base.mdat");
	pObj = pMeshData->Instantiate();
	pObj->SetName(L"SandyBeach_Base");
	pObj->Transform()->SetRelativeScale(1.0f, 1.0f, 1.0f);
	pObj->Transform()->SetRelativeScale(0.01f, 0.01f, 0.01f);
	//SpawnGameObject(pObj, Vec3(10799.f, -669.f, -10157.f), L"Base");
	//SpawnGameObject(pObj, Vec3(7241.f, -1104.f, 1187.f), L"Base");
	SpawnGameObject(pObj, Vec3(-79.400f, -11.500f, -75.300f), L"Base");
	
	pMeshData = CResMgr::GetInst()->FindRes<CMeshData>(L"SandyBeach_Building.mdat");
	pObj = pMeshData->Instantiate();
	pObj->SetName(L"SandyBeach_Building");
	rot = pObj->Transform()->GetRelativeRot();
	rot.x = -1.5708f;
	pObj->Transform()->SetRelativeRot(rot);
	pObj->Transform()->SetRelativeScale(1.0f, 1.0f, 1.0f);
	pObj->Transform()->SetRelativeScale(0.01f, 0.01f, 0.01f);
	//SpawnGameObject(pObj, Vec3(18869.f, 166.f, -1388.f), L"Building");
	//SpawnGameObject(pObj, Vec3(15311.f, -269.f, 9956.f), L"Building");
	SpawnGameObject(pObj, Vec3(0.911f, -0.700f, 12.500f), L"Building");
#pragma endregion

#pragma region School

	pMeshData = CResMgr::GetInst()->FindRes<CMeshData>(L"School_Base.mdat");
	pObj = pMeshData->Instantiate();
	pObj->SetName(L"School_Base");
	pObj->Transform()->SetRelativeScale(1.0f, 1.0f, 1.0f);
	pObj->Transform()->SetRelativeScale(0.01f, 0.01f, 0.01f);
	//SpawnGameObject(pObj, Vec3(10055.f, 105.f, 4190.f), L"Base");
	//SpawnGameObject(pObj, Vec3(6497.f, -330.f, 15534.f), L"Base");
	SpawnGameObject(pObj, Vec3(-86.656f, -4.100f, 67.850f), L"Base");
	
	pMeshData = CResMgr::GetInst()->FindRes<CMeshData>(L"School_Building.mdat");
	pObj = pMeshData->Instantiate();
	pObj->SetName(L"School_Building");
	pObj->Transform()->SetRelativeScale(1.0f, 1.0f, 1.0f);
	pObj->Transform()->SetRelativeScale(0.01f, 0.01f, 0.01f);
	//SpawnGameObject(pObj, Vec3(11240.f, 385.f, 545.f), L"Building");
	//SpawnGameObject(pObj, Vec3(7882.f, -50.f, 11889.f), L"Building");
	SpawnGameObject(pObj, Vec3(-74.800f, -1.380f, 31.400f), L"Building");
	
	pMeshData = CResMgr::GetInst()->FindRes<CMeshData>(L"School_Roof.mdat");
	pObj = pMeshData->Instantiate();
	pObj->SetName(L"School_Roof");
	rot = pObj->Transform()->GetRelativeRot();
	rot.y = -1.5708f;
	pObj->Transform()->SetRelativeRot(rot);
	pObj->Transform()->SetRelativeScale(1.0f, 1.0f, 1.0f);
	pObj->Transform()->SetRelativeScale(0.01f, 0.01f, 0.01f);
	//SpawnGameObject(pObj, Vec3(9995.f, 395.f, 5030.f), L"Roof");
	//SpawnGameObject(pObj, Vec3(6437.f, -40.f, 16374.f), L"Roof");
	SpawnGameObject(pObj, Vec3(-87.150f, -1.280f, 76.150f), L"Roof");
	
#pragma endregion

#pragma region Uptown
	pMeshData = CResMgr::GetInst()->FindRes<CMeshData>(L"Uptown_Base.mdat");
	pObj = pMeshData->Instantiate();
	pObj->SetName(L"Uptown_Base");
	pObj->Transform()->SetRelativeScale(1.0f, 1.0f, 1.0f);
	pObj->Transform()->SetRelativeScale(0.01f, 0.01f, 0.01f);
	//SpawnGameObject(pObj, Vec3(21120.f, -144.f, -7965.f), L"Base");
	//SpawnGameObject(pObj, Vec3(17562.f, -579.f, 3379.f), L"Base");
	SpawnGameObject(pObj, Vec3(23.820f, -6.250f, -53.350f), L"Base");
	
	pMeshData = CResMgr::GetInst()->FindRes<CMeshData>(L"Uptown_Building.mdat");
	pObj = pMeshData->Instantiate();
	pObj->SetName(L"Uptown_Building");
	pObj->Transform()->SetRelativeScale(1.0f, 1.0f, 1.0f);
	pObj->Transform()->SetRelativeScale(0.01f, 0.01f, 0.01f);
	//SpawnGameObject(pObj, Vec3(19628.f, 68.f, -9652.f), L"Building");
	//SpawnGameObject(pObj, Vec3(16070.f, -367.f, 1692.f), L"Building");
	SpawnGameObject(pObj, Vec3(9.f, -5.600f, -70.290f), L"Building");
#pragma endregion

	CCollisionMgr::GetInst()->LayerCheck(L"Monster", L"Monster");
}

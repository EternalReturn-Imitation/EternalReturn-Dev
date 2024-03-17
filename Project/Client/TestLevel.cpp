#include "pch.h"
#include "TestLevel.h"

#include <Engine\CLevelMgr.h>
#include <Engine\CLevel.h>
#include <Engine\CLayer.h>
#include <Engine\CGameObject.h>
#include <Engine\components.h>

#include <Engine\CResMgr.h>
#include <Engine\CPathFindMgr.h>
#include <Engine\CCollisionMgr.h>

#include <Engine\CCollider2D.h>
#include <Engine\CCollider3D.h>
#include <Engine\CFindPath.h>

#include <Script\CEditCamControlScript.h>
#include <Script\CFollowMainCamScript.h>
#include <Script\ER_CamControllerScript.h>
#include <Script\ER_DataScript_ItemBox.h>
#include <Script\ER_DataScript_LandMeshBase.h>

#include <Engine\CSetColorShader.h>

#include <Script\ER_CharacterMgr.h>

#include "CEditorObjMgr.h"

#include "CLevelSaveLoad.h"



void CreateTestLevel()
{
	CLevel* pCurLevel = CLevelMgr::GetInst()->GetCurLevel();
	pCurLevel->ChangeState(LEVEL_STATE::STOP);

	// Layer 설정
	pCurLevel->GetLayer(0)->SetName(L"Default");
	pCurLevel->GetLayer(1)->SetName(L"SkyBox");
	pCurLevel->GetLayer(2)->SetName(L"Base");
	pCurLevel->GetLayer(3)->SetName(L"Building");
	pCurLevel->GetLayer(4)->SetName(L"Roof");
	pCurLevel->GetLayer(5)->SetName(L"ItemBox");
	pCurLevel->GetLayer(11)->SetName(L"Monster");
	pCurLevel->GetLayer(12)->SetName(L"Character");
	pCurLevel->GetLayer(13)->SetName(L"Player");
	pCurLevel->GetLayer(30)->SetName(L"NONERender");
	pCurLevel->GetLayer(31)->SetName(L"UI");


	// Main Camera Object
	CGameObject* pMainCam = new CGameObject;
	pMainCam->SetName(L"MainCamera");

	pMainCam->AddComponent(new CTransform);
	pMainCam->AddComponent(new ER_CamControllerScript);
	pMainCam->AddComponent(new CCamera);

	pMainCam->Transform()->SetRelativeRot(Vec3(Deg2Rad(54.f), Deg2Rad(-45.f), 0.f));

	pMainCam->Camera()->SetFar(500.f);
	pMainCam->Camera()->SetFOV(Deg2Rad(30));
	pMainCam->Camera()->SetProjType(PROJ_TYPE::PERSPECTIVE);
	pMainCam->Camera()->SetCameraIndex(0);		
	pMainCam->Camera()->SetLayerMaskAll(true);	
	pMainCam->Camera()->SetLayerMask(31, false);

	SpawnGameObject(pMainCam, Vec3(0.f, 20.f, 0.f), 0);


	// UI Camera
	CGameObject* pUICam = new CGameObject;
	pUICam->SetName(L"UICamera");
	
	pUICam->AddComponent(new CTransform);
	pUICam->AddComponent(new CCamera);
	
	pUICam->Transform()->SetRelativeRot(Vec3(0.f,0.f,0.f));
	
	pUICam->Camera()->SetFar(10.f);
	pUICam->Camera()->SetProjType(PROJ_TYPE::ORTHOGRAPHIC);
	pUICam->Camera()->SetCameraIndex(1);
	
	Vec2 WinResolution = CEngine::GetInst()->GetWindowResolution();
	pUICam->Camera()->SetOrthoWidth(WinResolution.x);
	pUICam->Camera()->SetOrthoHeight(WinResolution.y);
	
	pUICam->Camera()->SetLayerMaskAll(false);
	pUICam->Camera()->SetLayerMask(31, true);
	
	// SpawnGameObject(pUICam, Vec3(-(WinResolution.x / 2.f), -(WinResolution.y / 2.f), 0.f), 0);
	SpawnGameObject(pUICam, Vec3(0.f, 0.f, 0.f), 0);

	// 메인 조명
	CGameObject* pLightObj = new CGameObject;
	pLightObj->SetName(L"Directional Light");

	pLightObj->AddComponent(new CTransform);
	pLightObj->AddComponent(new CLight3D);
	pLightObj->AddComponent(new CFollowMainCamScript);
	pLightObj->GetScript<CFollowMainCamScript>()->SetTarget(Vec3(-14, 0.f, 0.f));
	pLightObj->Transform()->SetRelativeScale(Vec3(1.f, 1.f, 1.f));
	pLightObj->Transform()->SetRelativeRot(Vec3(Deg2Rad(55.f), 0.f, Deg2Rad(5.f)));

	pLightObj->Light3D()->SetLightType(LIGHT_TYPE::DIRECTIONAL);
	pLightObj->Light3D()->SetRadius(500.f);
	pLightObj->Light3D()->GetLightRenderCam()->SetFar(200.f);
	pLightObj->Light3D()->GetLightRenderCam()->SetOrthoWidth(30.f);
	pLightObj->Light3D()->GetLightRenderCam()->SetOrthoHeight(30.f);
	pLightObj->Light3D()->SetLightColor(Vec3(1.f, 1.f, 1.f));
	pLightObj->Light3D()->SetLightAmbient(Vec3(0.f, 0.f, 0.f));

	SpawnGameObject(pLightObj, Vec3(0.f, 0.f, 0.f), 0);


	// UITestObj
	CGameObject* UITestObj = new CGameObject;
	UITestObj->SetName(L"UITestObj");
	
	UITestObj->AddComponent(new CTransform);
	UITestObj->AddComponent(new CMeshRender);
	UITestObj->AddComponent(new CUI_Button);
	
	UITestObj->Transform()->SetRelativeScale(Vec3(100.f, 100.f, 1.f));
	
	UITestObj->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	UITestObj->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std2DMtrl"), 0);
	UITestObj->MeshRender()->GetMaterial(0)->SetTexParam(TEX_PARAM::TEX_0, CResMgr::GetInst()->GetInst()->FindRes<CTexture>(L"Char_Portrait_Rio.png"));
	
	SpawnGameObject(UITestObj, Vec3(-300.f, -300.f, 0.f), L"UI");


	// SkyBox
	// CGameObject* pSkyBox = new CGameObject;
	// pSkyBox->SetName(L"SkyBox");
	// 
	// pSkyBox->AddComponent(new CTransform);
	// pSkyBox->AddComponent(new CSkyBox);
	// 
	// pSkyBox->Transform()->SetRelativeScale(Vec3(100.f, 100.f, 100));
	// pSkyBox->SkyBox()->SetSkyBoxType(SKYBOX_TYPE::SPHERE);
	// pSkyBox->SkyBox()->SetSkyBoxTexture(CResMgr::GetInst()->FindRes<CTexture>(L"Sky01.png"));
	// 
	// SpawnGameObject(pSkyBox, Vec3(0.f, 0.f, 0.f), L"SkyBox");

	

	// 파티클
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

	bool bCube = false;
	if (bCube)
	{
		CGameObject* CubeObj = new CGameObject;
		CubeObj->SetName(L"CubeObj");
		CubeObj->AddComponent(new CTransform);
		CubeObj->AddComponent(new CMeshRender);
		CubeObj->AddComponent(new CFindPath);
		CubeObj->Transform()->SetRelativeScale(Vec3(1.f, 2.f, 1.f));
		CubeObj->Transform()->SetRelativeRot(Vec3(XM_PI / 2.f, 0.f, 0.f));

		CubeObj->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"CubeMesh"));
		CubeObj->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std3D_DeferredMtrl"), 0);

		SpawnGameObject(CubeObj, Vec3(000.f, 0.f, 000.f), L"Monster");
	}
	else
	{
		CGameObject* CharObj = ER_CharacterMgr::GetInst()->GetCharacter(L"Rio");
		CharObj->AddComponent(new CCollider3D);
		CharObj->Collider3D()->SetCollider3DType(COLLIDER3D_TYPE::CUBE);
		CharObj->Collider3D()->SetOffsetScale(Vec3(2.0f, 2.0f, 2.0f));
		//SpawnGameObject(CharObj, Vec3(7.f, 0.f, 3.f), L"Monster");
		SpawnGameObject(CharObj, Vec3(-69.32188f, 0.0f, 37.60328f), L"Player");
	}

	CGameObject* MapCollider = new CGameObject;
	MapCollider->SetName(L"MapCollider");
	MapCollider->AddComponent(new CTransform);
	MapCollider->AddComponent(new CCollider2D);
	MapCollider->Collider2D()->SetOffsetRot(Vec3(XMConvertToRadians(90.f), 0.f, 0.f));
	MapCollider->Collider2D()->SetOffsetScale(Vec2(256.f, 240.f));
	MapCollider->Collider2D()->SetOffsetPos(Vec3(0.f, 0.f, 0.f));
	CPathFindMgr::GetInst()->SetMapCollider(MapCollider);
	SpawnGameObject(MapCollider, Vec3(-37.98901f, 0.f, 0.62141f), L"NONERender");

	CCollisionMgr::GetInst()->LayerCheck(L"Monster", L"Monster");
	CCollisionMgr::GetInst()->LayerCheck(L"Player", L"Base");
	CCollisionMgr::GetInst()->LayerCheck(L"ItemBox", L"NONERender");

	//CGameObject* pObject = new CGameObject;
	//pObject->SetName(L"TestCollider2D01");
	//pObject->AddComponent(new CTransform);
	//pObject->AddComponent(new CCollider3D);
	//pObject->Collider3D()->SetOffsetScale(Vec3(1.f, 1.f, 1.f));
	//pObject->Collider3D()->SetOffsetPos(Vec3(0.f, 0.f, 0.f));
	//pObject->Collider3D()->SetCollider3DType(COLLIDER3D_TYPE::CUBE);
	//pObject->begin();
	//SpawnGameObject(pObject, Vec3(10.f, 1.f, 10.f), L"Nonerender");

	LoadingBackGround();
	LoadingItemBoxes();

	CCollisionMgr::GetInst()->RayLayerCheck(L"NONERender");
	CCollisionMgr::GetInst()->RayLayerCheck(L"ItemBox");
}

void LoadingBackGround() {
	Ptr<CMeshData> pMeshData = nullptr;
	Vec3 rot = {};
#pragma region Archery
	CGameObject* LandMesh = new CGameObject;
	pMeshData = CResMgr::GetInst()->FindRes<CMeshData>(L"Archery_Roof.mdat");
	LandMesh = pMeshData->Instantiate();
	LandMesh->SetName(L"Archery_Roof");
	LandMesh->Transform()->SetRelativeScale(0.01f, 0.01f, 0.01f);
	SpawnGameObject(LandMesh, Vec3(-133.18f, 4.660f, 43.730f), L"Roof");

	//지붕 예외처리하기 위해 할당해줌.
	CGameObject* roof = LandMesh;

	pMeshData = CResMgr::GetInst()->FindRes<CMeshData>(L"Archery_Building.mdat");
	LandMesh = pMeshData->Instantiate();
	LandMesh->SetName(L"Archery_Building");
	rot = LandMesh->Transform()->GetRelativeRot();
	rot.x = -1.5708f;
	LandMesh->Transform()->SetRelativeRot(rot);
	LandMesh->Transform()->SetRelativeScale(0.01f, 0.01f, 0.01f);
	SpawnGameObject(LandMesh, Vec3(0.2f, -1.f, 0.5f), L"Building");

	pMeshData = CResMgr::GetInst()->FindRes<CMeshData>(L"Archery_Base.mdat");
	LandMesh = pMeshData->Instantiate();
	LandMesh->SetName(L"Archery_Base");
	LandMesh->Transform()->SetRelativeScale(0.01f, 0.01f, 0.01f);
	SpawnGameObject(LandMesh, Vec3(-187.f, -5.2f, 26.f), L"Base");

	LandMesh = new CGameObject;
	LandMesh->SetName(L"Archery_Base_Collider3D");
	LandMesh->AddComponent(new CTransform);
	LandMesh->AddComponent(new CCollider3D);
	LandMesh->Collider3D()->SetOffsetScale(Vec3(10.f, 1.f, 20.f));
	LandMesh->Collider3D()->SetOffsetPos(Vec3(0.f, 0.f, 0.f));
	LandMesh->Collider3D()->SetCollider3DType(COLLIDER3D_TYPE::CUBE);
	LandMesh->AddComponent(new ER_DataScript_LandMeshBase);
	ER_DataScript_LandMeshBase* script = LandMesh->GetScript<ER_DataScript_LandMeshBase>();
	script->SetRoof(roof);
	SpawnGameObject(LandMesh, Vec3(-132.f, 0.76422f, 53.28091f), L"Base");
#pragma endregion

#pragma region Forest
	pMeshData = CResMgr::GetInst()->FindRes<CMeshData>(L"Forest_Base.mdat");
	LandMesh = pMeshData->Instantiate();
	LandMesh->SetName(L"Forest_Base");
	LandMesh->Transform()->SetRelativeScale(0.01f, 0.01f, 0.01f);
	SpawnGameObject(LandMesh, Vec3(-53.680f, -8.500f, -20.202f), L"Base");

	pMeshData = CResMgr::GetInst()->FindRes<CMeshData>(L"Forest_Building.mdat");
	LandMesh = pMeshData->Instantiate();
	LandMesh->SetName(L"Forest_Building");
	LandMesh->Transform()->SetRelativeScale(0.01025f, 0.01110f, 0.01110f);
	SpawnGameObject(LandMesh, Vec3(-12.71692f, -0.42700f, -21.42004f), L"Building");
#pragma endregion

#pragma region Hotel
	pMeshData = CResMgr::GetInst()->FindRes<CMeshData>(L"Hotel_Base.mdat");
	LandMesh = pMeshData->Instantiate();
	LandMesh->SetName(L"Hotel_Base");
	LandMesh->Transform()->SetRelativeScale(0.01f, 0.01f, 0.01f);
	SpawnGameObject(LandMesh, Vec3(-96.4f, -2.32f, 176.4f), L"Base");

	pMeshData = CResMgr::GetInst()->FindRes<CMeshData>(L"Hotel_Building.mdat");
	LandMesh = pMeshData->Instantiate();
	LandMesh->SetName(L"Hotel_Building");
	LandMesh->Transform()->SetRelativeScale(0.01f, 0.01f, 0.01f);
	SpawnGameObject(LandMesh, Vec3(-141.f, -3.6f, -21.35f), L"Building");

	pMeshData = CResMgr::GetInst()->FindRes<CMeshData>(L"Hotel_Roof.mdat");
	LandMesh = pMeshData->Instantiate();
	LandMesh->SetName(L"Hotel_Roof");
	LandMesh->Transform()->SetRelativeScale(0.01f, 0.01f, 0.01f);
	SpawnGameObject(LandMesh, Vec3(-101.72f, -0.910f, -18.075f), L"Roof");

	//루프 안보이게 하기 위해 할당해줌.
	roof = LandMesh;

	LandMesh = new CGameObject;
	LandMesh->SetName(L"Hotel_Base_Collider3D");
	LandMesh->AddComponent(new CTransform);
	LandMesh->AddComponent(new CCollider3D);
	LandMesh->Collider3D()->SetOffsetScale(Vec3(15.f, 1.f, 30.f));
	LandMesh->Collider3D()->SetOffsetPos(Vec3(0.f, 0.f, 0.f));
	LandMesh->Collider3D()->SetCollider3DType(COLLIDER3D_TYPE::CUBE);
	LandMesh->AddComponent(new ER_DataScript_LandMeshBase);
	script = LandMesh->GetScript<ER_DataScript_LandMeshBase>();
	script->SetRoof(roof);
	SpawnGameObject(LandMesh, Vec3(-109.07837f, 0.f, -3.79784f), L"Base");
#pragma endregion

#pragma region SandyBeach
	pMeshData = CResMgr::GetInst()->FindRes<CMeshData>(L"SandyBeach_Base.mdat");
	LandMesh = pMeshData->Instantiate();
	LandMesh->SetName(L"SandyBeach_Base");
	LandMesh->Transform()->SetRelativeScale(0.01f, 0.01f, 0.01f);
	SpawnGameObject(LandMesh, Vec3(-79.400f, -11.500f, -75.300f), L"Base");

	pMeshData = CResMgr::GetInst()->FindRes<CMeshData>(L"SandyBeach_Building.mdat");
	LandMesh = pMeshData->Instantiate();
	LandMesh->SetName(L"SandyBeach_Building");
	rot = LandMesh->Transform()->GetRelativeRot();
	rot.x = -1.5708f;
	LandMesh->Transform()->SetRelativeRot(rot);
	LandMesh->Transform()->SetRelativeScale(0.01f, 0.01f, 0.01f);
	SpawnGameObject(LandMesh, Vec3(0.911f, -0.700f, 12.500f), L"Building");
#pragma endregion

#pragma region School

	pMeshData = CResMgr::GetInst()->FindRes<CMeshData>(L"School_Base.mdat");
	LandMesh = pMeshData->Instantiate();
	LandMesh->SetName(L"School_Base");
	LandMesh->Transform()->SetRelativeScale(0.01f, 0.01f, 0.01f);
	SpawnGameObject(LandMesh, Vec3(-86.656f, -4.100f, 67.850f), L"Base");

	pMeshData = CResMgr::GetInst()->FindRes<CMeshData>(L"School_Building.mdat");
	LandMesh = pMeshData->Instantiate();
	LandMesh->SetName(L"School_Building");
	LandMesh->Transform()->SetRelativeScale(0.01f, 0.01f, 0.01f);
	SpawnGameObject(LandMesh, Vec3(-74.800f, -1.380f, 31.400f), L"Building");

	pMeshData = CResMgr::GetInst()->FindRes<CMeshData>(L"School_Roof.mdat");
	LandMesh = pMeshData->Instantiate();
	LandMesh->SetName(L"School_Roof");
	rot = LandMesh->Transform()->GetRelativeRot();
	rot.y = -1.5708f;
	LandMesh->Transform()->SetRelativeRot(rot);
	LandMesh->Transform()->SetRelativeScale(0.01f, 0.01f, 0.01f);
	SpawnGameObject(LandMesh, Vec3(-87.150f, -1.280f, 76.150f), L"Roof");

	roof = LandMesh;

	// TO_DO : 복합 콜라이더의 오버랩 카운트를 동기화시켜서 한번에 관리할 수 있도록 프로세스 구성

	LandMesh = new CGameObject;
	LandMesh->SetName(L"School_Base_Collider3D01");
	LandMesh->AddComponent(new CTransform);
	LandMesh->AddComponent(new CCollider3D);
	LandMesh->Collider3D()->SetOffsetScale(Vec3(28.f, 1.f, 24.f));
	LandMesh->Collider3D()->SetOffsetPos(Vec3(0.f, 0.f, 0.f));
	LandMesh->Collider3D()->SetCollider3DType(COLLIDER3D_TYPE::CUBE);
	LandMesh->AddComponent(new ER_DataScript_LandMeshBase);
	script = LandMesh->GetScript<ER_DataScript_LandMeshBase>();
	script->SetRoof(roof);
	SpawnGameObject(LandMesh, Vec3(-101.36192f, 0.f, 88.06251f), L"Base");

	LandMesh = new CGameObject;
	LandMesh->SetName(L"School_Base_Collider3D02");
	LandMesh->AddComponent(new CTransform);
	LandMesh->AddComponent(new CCollider3D);
	LandMesh->Collider3D()->SetOffsetScale(Vec3(17.f, 1.f, 55.f));
	LandMesh->Collider3D()->SetOffsetPos(Vec3(0.f, 0.f, 0.f));
	LandMesh->Collider3D()->SetCollider3DType(COLLIDER3D_TYPE::CUBE);
	LandMesh->AddComponent(new ER_DataScript_LandMeshBase);
	script = LandMesh->GetScript<ER_DataScript_LandMeshBase>();
	script->SetRoof(roof);
	SpawnGameObject(LandMesh, Vec3(-69.12391f, 0.f, 68.64030f), L"Base");

	LandMesh = new CGameObject;
	LandMesh->SetName(L"School_Base_Collider3D03");
	LandMesh->AddComponent(new CTransform);
	LandMesh->AddComponent(new CCollider3D);
	LandMesh->Collider3D()->SetOffsetScale(Vec3(6.f, 1.f, 7.f));
	LandMesh->Collider3D()->SetOffsetPos(Vec3(0.f, 0.f, 0.f));
	LandMesh->Collider3D()->SetCollider3DType(COLLIDER3D_TYPE::CUBE);
	LandMesh->AddComponent(new ER_DataScript_LandMeshBase);
	script = LandMesh->GetScript<ER_DataScript_LandMeshBase>();
	script->SetRoof(roof);
	SpawnGameObject(LandMesh, Vec3(-59.57447f, 0.f, 65.01f), L"Base");

#pragma endregion

#pragma region Uptown
	pMeshData = CResMgr::GetInst()->FindRes<CMeshData>(L"Uptown_Base.mdat");
	LandMesh = pMeshData->Instantiate();
	LandMesh->SetName(L"Uptown_Base");
	LandMesh->Transform()->SetRelativeScale(0.01f, 0.01f, 0.01f);
	SpawnGameObject(LandMesh, Vec3(23.820f, -6.250f, -53.350f), L"Base");

	pMeshData = CResMgr::GetInst()->FindRes<CMeshData>(L"Uptown_Building.mdat");
	LandMesh = pMeshData->Instantiate();
	LandMesh->SetName(L"Uptown_Building");
	LandMesh->Transform()->SetRelativeScale(0.01f, 0.01f, 0.01f);
	SpawnGameObject(LandMesh, Vec3(9.f, -5.600f, -70.290f), L"Building");
#pragma endregion
}

void LoadingItemBoxes() {
	Ptr<CMeshData> pMeshData = nullptr;
	Vec3 rot = {};
	CGameObject* pItemBox;

#pragma region Archery
	pMeshData = CResMgr::GetInst()->FindRes<CMeshData>(L"Archery_Biotoilet01.mdat");
	pItemBox = pMeshData->Instantiate();
	pItemBox->SetName(L"Archery_Biotoilet01");
	rot = pItemBox->Transform()->GetRelativeRot();
	rot.x = XMConvertToRadians(90.000f);
	rot.y = XMConvertToRadians(-68.000f);
	rot.z = XMConvertToRadians(-180.000f);
	pItemBox->Transform()->SetRelativeRot(rot);
	pItemBox->Transform()->SetRelativeScale(0.01f, 0.01f, 0.01f);
	pItemBox->AddComponent(new CCollider3D);
	pItemBox->Collider3D()->SetOffsetScale(Vec3(150.f, 150.f, 300.f));
	pItemBox->Collider3D()->SetOffsetPos(Vec3(0.f, 0.f, 100.f));
	pItemBox->Collider3D()->SetCollider3DType(COLLIDER3D_TYPE::CUBE);
	pItemBox->AddComponent(new ER_DataScript_ItemBox);
	SpawnGameObject(pItemBox, Vec3(-134.624f, -1.012f, 91.280f), L"ItemBox");

	//pMeshData = CResMgr::GetInst()->FindRes<CMeshData>(L"Archery_Dumpster01.mdat");
	//pItemBox = pMeshData->Instantiate();
	//pItemBox->SetName(L"Archery_Dumpster01");
	//rot = pItemBox->Transform()->GetRelativeRot();
	//rot.x = XMConvertToRadians(90.000f);
	//rot.z = XMConvertToRadians(-180.000f);
	//pItemBox->Transform()->SetRelativeRot(rot);
	//pItemBox->Transform()->SetRelativeScale(0.01f, 0.01f, 0.01f);
	//pItemBox->AddComponent(new CCollider3D);
	//pItemBox->Collider3D()->SetOffsetScale(Vec3(100.f, 200.f, 100.f));
	//pItemBox->Collider3D()->SetOffsetPos(Vec3(0.f, 0.f, 100.f));
	//pItemBox->Collider3D()->SetCollider3DType(COLLIDER3D_TYPE::CUBE);
	//pItemBox->AddComponent(new ER_DataScript_ItemBox);
	//pItemBox->MeshRender()->GetDynamicMaterial(0);
	//SpawnGameObject(pItemBox, Vec3(-122.704f, -0.380f, 60.545f), L"ItemBox");

	pMeshData = CResMgr::GetInst()->FindRes<CMeshData>(L"Archery_Locker01.mdat");
	pItemBox = pMeshData->Instantiate();
	pItemBox->SetName(L"Archery_Locker01");
	rot = pItemBox->Transform()->GetRelativeRot();
	rot.x = XMConvertToRadians(-90.000f);
	pItemBox->Transform()->SetRelativeRot(rot);
	pItemBox->Transform()->SetRelativeScale(0.01f, 0.01f, 0.01f);
	pItemBox->AddComponent(new CCollider3D);
	pItemBox->Collider3D()->SetOffsetScale(Vec3(100.f, 200.f, 300.f));
	pItemBox->Collider3D()->SetOffsetPos(Vec3(0.f, 100.f, 100.f));
	pItemBox->Collider3D()->SetCollider3DType(COLLIDER3D_TYPE::CUBE);
	pItemBox->AddComponent(new ER_DataScript_ItemBox);
	SpawnGameObject(pItemBox, Vec3(-133.184f, 0.028f, 46.186f), L"ItemBox");

	pMeshData = CResMgr::GetInst()->FindRes<CMeshData>(L"Archery_SteelBox01.mdat");
	pItemBox = pMeshData->Instantiate();
	pItemBox->SetName(L"Archery_SteelBox01");
	rot = pItemBox->Transform()->GetRelativeRot();
	rot.y = XMConvertToRadians(-36.000f);
	pItemBox->Transform()->SetRelativeRot(rot);
	pItemBox->Transform()->SetRelativeScale(0.01f, 0.01f, 0.01f);
	pItemBox->AddComponent(new CCollider3D);
	pItemBox->Collider3D()->SetOffsetScale(Vec3(100.f, 200.f, 100.f));
	pItemBox->Collider3D()->SetOffsetPos(Vec3(0.f, 100.f, 0.f));
	pItemBox->Collider3D()->SetCollider3DType(COLLIDER3D_TYPE::CUBE);
	pItemBox->AddComponent(new ER_DataScript_ItemBox);
	pItemBox->MeshRender()->GetDynamicMaterial(0);
	SpawnGameObject(pItemBox, Vec3(-121.580f, -0.397f, 35.689f), L"ItemBox");
#pragma endregion

#pragma region Forest
	pMeshData = CResMgr::GetInst()->FindRes<CMeshData>(L"Forest_PalletLoaded01.mdat");
	pItemBox = pMeshData->Instantiate();
	pItemBox->SetName(L"Forest_PalletLoaded01");
	pItemBox->Transform()->SetRelativeScale(0.01f, 0.01f, 0.01f);
	pItemBox->AddComponent(new CCollider3D);
	pItemBox->Collider3D()->SetOffsetScale(Vec3(120.f, 120.f, 120.f));
	pItemBox->Collider3D()->SetOffsetPos(Vec3(0.f, 50.f, 0.f));
	pItemBox->Collider3D()->SetCollider3DType(COLLIDER3D_TYPE::CUBE);
	pItemBox->AddComponent(new ER_DataScript_ItemBox);
	SpawnGameObject(pItemBox, Vec3(19.80982f, -0.11900f, 17.09492f), L"ItemBox");

	pMeshData = CResMgr::GetInst()->FindRes<CMeshData>(L"Forest_SteelBox01.mdat");
	pItemBox = pMeshData->Instantiate();
	pItemBox->SetName(L"Forest_SteelBox01");
	pItemBox->Transform()->SetRelativeScale(0.01f, 0.01f, 0.01f);
	pItemBox->AddComponent(new CCollider3D);
	pItemBox->Collider3D()->SetOffsetScale(Vec3(100.f, 120.f, 100.f));
	pItemBox->Collider3D()->SetOffsetPos(Vec3(0.f, 50.f, 0.f));
	pItemBox->Collider3D()->SetCollider3DType(COLLIDER3D_TYPE::CUBE);
	pItemBox->AddComponent(new ER_DataScript_ItemBox);
	pItemBox->MeshRender()->GetDynamicMaterial(0);
	SpawnGameObject(pItemBox, Vec3(-27.24952f, -0.299f, 19.63642f), L"ItemBox");

	pMeshData = CResMgr::GetInst()->FindRes<CMeshData>(L"Forest_TreeStump01.mdat");
	pItemBox = pMeshData->Instantiate();
	pItemBox->SetName(L"Forest_TreeStump01");
	rot = pItemBox->Transform()->GetRelativeRot();
	rot.y = XMConvertToRadians(21.000f);
	pItemBox->Transform()->SetRelativeRot(Vec3(0.f, Deg2Rad(-10), 0.f));
	pItemBox->Transform()->SetRelativeScale(0.01f, 0.01f, 0.01f);
	pItemBox->AddComponent(new CCollider3D);
	pItemBox->Collider3D()->SetOffsetScale(Vec3(400.f, 100.f, 100.f));
	pItemBox->Collider3D()->SetOffsetPos(Vec3(0.f, 0.f, 0.f));
	pItemBox->Collider3D()->SetCollider3DType(COLLIDER3D_TYPE::CUBE);
	pItemBox->AddComponent(new ER_DataScript_ItemBox);
	pItemBox->MeshRender()->GetDynamicMaterial(0);
	SpawnGameObject(pItemBox, Vec3(14.36587f, 0.59560f, -3.72484f), L"ItemBox");

	pMeshData = CResMgr::GetInst()->FindRes<CMeshData>(L"Forest_TreeStump02.mdat");
	pItemBox = pMeshData->Instantiate();
	pItemBox->SetName(L"Forest_TreeStump02");
	pItemBox->Transform()->SetRelativeScale(0.01f, 0.01f, 0.01f);
	pItemBox->Transform()->SetRelativeRot(Deg2Rad(-11), Deg2Rad(88), Deg2Rad(-11));
	pItemBox->AddComponent(new CCollider3D);
	pItemBox->Collider3D()->SetOffsetScale(Vec3(400.f, 100.f, 100.f));
	pItemBox->Collider3D()->SetOffsetPos(Vec3(0.f, 0.f, 0.f));
	pItemBox->Collider3D()->SetCollider3DType(COLLIDER3D_TYPE::CUBE);
	pItemBox->MeshRender()->GetDynamicMaterial(0);
	pItemBox->AddComponent(new ER_DataScript_ItemBox);
	SpawnGameObject(pItemBox, Vec3(-32.45346f, -0.05804f, -2.84341f), L"ItemBox");
#pragma endregion

#pragma region Hotel
	pMeshData = CResMgr::GetInst()->FindRes<CMeshData>(L"Hotel_ATM01.mdat");
	pItemBox = pMeshData->Instantiate();
	pItemBox->SetName(L"Hotel_ATM01");
	rot = pItemBox->Transform()->GetRelativeRot();
	rot.x = XMConvertToRadians(-180.000f);
	rot.z = XMConvertToRadians(-180.000f);
	pItemBox->Transform()->SetRelativeRot(rot);
	pItemBox->Transform()->SetRelativeScale(0.01f, 0.01f, 0.01f);
	pItemBox->AddComponent(new CCollider3D);
	pItemBox->Collider3D()->SetOffsetScale(Vec3(100.f, 200.f, 100.f));
	pItemBox->Collider3D()->SetOffsetPos(Vec3(0.f, 100.f, 0.f));
	pItemBox->Collider3D()->SetCollider3DType(COLLIDER3D_TYPE::CUBE);
	pItemBox->AddComponent(new ER_DataScript_ItemBox);
	SpawnGameObject(pItemBox, Vec3(-112.772f, -0.318f, -16.547f), L"ItemBox");

	pMeshData = CResMgr::GetInst()->FindRes<CMeshData>(L"Hotel_BarbequeGrill01.mdat");
	pItemBox = pMeshData->Instantiate();
	pItemBox->SetName(L"Hotel_BarbequeGrill01");
	rot = pItemBox->Transform()->GetRelativeRot();
	rot.y = XMConvertToRadians(-90.000f);
	pItemBox->Transform()->SetRelativeRot(rot);
	pItemBox->Transform()->SetRelativeScale(0.01f, 0.01f, 0.01f);
	pItemBox->AddComponent(new CCollider3D);
	pItemBox->Collider3D()->SetOffsetScale(Vec3(120.f, 150.f, 150.f));
	pItemBox->Collider3D()->SetOffsetPos(Vec3(0.f, 70.f, 0.f));
	pItemBox->Collider3D()->SetCollider3DType(COLLIDER3D_TYPE::CUBE);
	pItemBox->AddComponent(new ER_DataScript_ItemBox);
	pItemBox->MeshRender()->GetDynamicMaterial(0);
	SpawnGameObject(pItemBox, Vec3(-103.886f, -0.374f, -32.562f), L"ItemBox");

	pMeshData = CResMgr::GetInst()->FindRes<CMeshData>(L"Hotel_Dumpster01.mdat");
	pItemBox = pMeshData->Instantiate();
	pItemBox->SetName(L"Hotel_Dumpster01");
	rot = pItemBox->Transform()->GetRelativeRot();
	rot.x = XMConvertToRadians(-90.000f);
	pItemBox->Transform()->SetRelativeRot(rot);
	pItemBox->Transform()->SetRelativeScale(0.01f, 0.01f, 0.01f);
	pItemBox->AddComponent(new CCollider3D);
	pItemBox->Collider3D()->SetOffsetScale(Vec3(100.f, 220.f, 170.f));
	pItemBox->Collider3D()->SetOffsetPos(Vec3(0.f, 0.f, 90.f));
	pItemBox->Collider3D()->SetCollider3DType(COLLIDER3D_TYPE::CUBE);
	pItemBox->AddComponent(new ER_DataScript_ItemBox);
	pItemBox->MeshRender()->GetDynamicMaterial(0);
	SpawnGameObject(pItemBox, Vec3(-63.052f, -0.332f, -19.634f), L"ItemBox");

	pMeshData = CResMgr::GetInst()->FindRes<CMeshData>(L"Hotel_VendingMachine01.mdat");
	pItemBox = pMeshData->Instantiate();
	pItemBox->SetName(L"Hotel_VendingMachine01");
	pItemBox->Transform()->SetRelativeScale(0.01f, 0.01f, 0.01f);
	pItemBox->AddComponent(new CCollider3D);
	pItemBox->Collider3D()->SetOffsetScale(Vec3(120.f, 220.f, 120.f));
	pItemBox->Collider3D()->SetOffsetPos(Vec3(0.f, 140.f, 0.f));
	pItemBox->Collider3D()->SetCollider3DType(COLLIDER3D_TYPE::CUBE);
	pItemBox->AddComponent(new ER_DataScript_ItemBox);
	SpawnGameObject(pItemBox, Vec3(-105.717f, -0.322f, 8.205f), L"ItemBox");
#pragma endregion

#pragma region SandyBeach
	pMeshData = CResMgr::GetInst()->FindRes<CMeshData>(L"SandyBeach_Boat01.mdat");
	pItemBox = pMeshData->Instantiate();
	pItemBox->SetName(L"SandyBeach_Boat01");
	rot = pItemBox->Transform()->GetRelativeRot();
	rot.x = XMConvertToRadians(90.000f);
	rot.y = XMConvertToRadians(45.000f);
	rot.z = XMConvertToRadians(-180.000f);
	pItemBox->Transform()->SetRelativeRot(rot);
	pItemBox->Transform()->SetRelativeScale(0.01f, 0.01f, 0.01f);
	pItemBox->AddComponent(new CCollider3D);
	pItemBox->Collider3D()->SetOffsetScale(Vec3(200.f, 700.f, 120.f));
	pItemBox->Collider3D()->SetOffsetPos(Vec3(0.f, 0.f, 50.f));
	pItemBox->Collider3D()->SetCollider3DType(COLLIDER3D_TYPE::CUBE);
	pItemBox->AddComponent(new ER_DataScript_ItemBox);
	pItemBox->MeshRender()->GetDynamicMaterial(0);
	SpawnGameObject(pItemBox, Vec3(-32.593f, -3.816f, -68.192f), L"ItemBox");

	pMeshData = CResMgr::GetInst()->FindRes<CMeshData>(L"SandyBeach_Boat02.mdat");
	pItemBox = pMeshData->Instantiate();
	pItemBox->SetName(L"SandyBeach_Boat02");
	rot = pItemBox->Transform()->GetRelativeRot();
	rot.x = XMConvertToRadians(-90.000f);
	rot.y = XMConvertToRadians(22.000f);
	pItemBox->Transform()->SetRelativeRot(rot);
	pItemBox->Transform()->SetRelativeScale(0.01f, 0.01f, 0.01f);
	pItemBox->AddComponent(new CCollider3D);
	pItemBox->Collider3D()->SetOffsetScale(Vec3(200.f, 700.f, 120.f));
	pItemBox->Collider3D()->SetOffsetPos(Vec3(0.f, 0.f, 50.f));
	pItemBox->Collider3D()->SetCollider3DType(COLLIDER3D_TYPE::CUBE);
	pItemBox->AddComponent(new ER_DataScript_ItemBox);
	pItemBox->MeshRender()->GetDynamicMaterial(0);
	SpawnGameObject(pItemBox, Vec3(-64.518f, -4.001f, -72.063f), L"ItemBox");

	pMeshData = CResMgr::GetInst()->FindRes<CMeshData>(L"SandyBeach_Dumpster01.mdat");
	pItemBox = pMeshData->Instantiate();
	pItemBox->SetName(L"SandyBeach_Dumpster01");
	rot = pItemBox->Transform()->GetRelativeRot();
	rot.x = XMConvertToRadians(-91.000f);
	rot.y = XMConvertToRadians(-66.000f);
	pItemBox->Transform()->SetRelativeRot(rot);
	pItemBox->Transform()->SetRelativeScale(0.01f, 0.01f, 0.01f);
	pItemBox->AddComponent(new CCollider3D);
	pItemBox->Collider3D()->SetOffsetScale(Vec3(100.f, 220.f, 170.f));
	pItemBox->Collider3D()->SetOffsetPos(Vec3(0.f, 0.f, 90.f));
	pItemBox->Collider3D()->SetCollider3DType(COLLIDER3D_TYPE::CUBE);
	pItemBox->AddComponent(new ER_DataScript_ItemBox);
	pItemBox->MeshRender()->GetDynamicMaterial(0);
	SpawnGameObject(pItemBox, Vec3(-42.661f, -2.764f, -48.906f), L"ItemBox");

	pMeshData = CResMgr::GetInst()->FindRes<CMeshData>(L"SandyBeach_Dumpster02.mdat");
	pItemBox = pMeshData->Instantiate();
	pItemBox->SetName(L"SandyBeach_Dumpster02");
	rot = pItemBox->Transform()->GetRelativeRot();
	rot.x = XMConvertToRadians(90.000f);
	rot.y = XMConvertToRadians(-90.000f);
	rot.z = XMConvertToRadians(-180.000f);
	pItemBox->Transform()->SetRelativeRot(rot);
	pItemBox->Transform()->SetRelativeScale(0.01f, 0.01f, 0.01f);
	pItemBox->AddComponent(new CCollider3D);
	pItemBox->Collider3D()->SetOffsetScale(Vec3(100.f, 220.f, 170.f));
	pItemBox->Collider3D()->SetOffsetPos(Vec3(0.f, 0.f, 90.f));
	pItemBox->Collider3D()->SetCollider3DType(COLLIDER3D_TYPE::CUBE);
	pItemBox->AddComponent(new ER_DataScript_ItemBox);
	pItemBox->MeshRender()->GetDynamicMaterial(0);
	SpawnGameObject(pItemBox, Vec3(-64.094f, -2.371f, -42.020f), L"ItemBox");
#pragma endregion

#pragma region School
	pMeshData = CResMgr::GetInst()->FindRes<CMeshData>(L"School_Dumpster01.mdat");
	pItemBox = pMeshData->Instantiate();
	pItemBox->SetName(L"School_Dumpster01");
	rot = pItemBox->Transform()->GetRelativeRot();
	rot.x = XMConvertToRadians(-90.000f);
	pItemBox->Transform()->SetRelativeRot(rot);
	pItemBox->Transform()->SetRelativeScale(0.01f, 0.01f, 0.01f);
	pItemBox->AddComponent(new CCollider3D);
	pItemBox->Collider3D()->SetOffsetScale(Vec3(100.f, 220.f, 170.f));
	pItemBox->Collider3D()->SetOffsetPos(Vec3(0.f, 0.f, 90.f));
	pItemBox->Collider3D()->SetCollider3DType(COLLIDER3D_TYPE::CUBE);
	pItemBox->AddComponent(new ER_DataScript_ItemBox);
	pItemBox->MeshRender()->GetDynamicMaterial(0);
	SpawnGameObject(pItemBox, Vec3(-105.100f, -1.160f, 53.730f), L"ItemBox");

	pMeshData = CResMgr::GetInst()->FindRes<CMeshData>(L"School_Locker01.mdat");
	pItemBox = pMeshData->Instantiate();
	pItemBox->SetName(L"School_Locker01");
	rot = pItemBox->Transform()->GetRelativeRot();
	rot.x = XMConvertToRadians(-90.000f);
	rot.y = XMConvertToRadians(90.000f);
	pItemBox->Transform()->SetRelativeRot(rot);
	pItemBox->Transform()->SetRelativeScale(0.01f, 0.01f, 0.01f);
	pItemBox->AddComponent(new CCollider3D);
	pItemBox->Collider3D()->SetOffsetScale(Vec3(50.f, 200.f, 250.f));
	pItemBox->Collider3D()->SetOffsetPos(Vec3(30.f, 90.f, 120.f));
	pItemBox->Collider3D()->SetCollider3DType(COLLIDER3D_TYPE::CUBE);
	pItemBox->AddComponent(new ER_DataScript_ItemBox);
	SpawnGameObject(pItemBox, Vec3(-66.118f, -0.291f, 68.203f), L"ItemBox");

	pMeshData = CResMgr::GetInst()->FindRes<CMeshData>(L"School_WreckCar01.mdat");
	pItemBox = pMeshData->Instantiate();
	pItemBox->SetName(L"School_WreckCar01");
	rot = pItemBox->Transform()->GetRelativeRot();
	rot.x = XMConvertToRadians(-180.000f);
	rot.y = XMConvertToRadians(-90.000f);
	rot.z = XMConvertToRadians(-180.000f);
	pItemBox->Transform()->SetRelativeRot(rot);
	pItemBox->Transform()->SetRelativeScale(0.01f, 0.01f, 0.01f);
	pItemBox->AddComponent(new CCollider3D);
	pItemBox->Collider3D()->SetOffsetScale(Vec3(200.f, 100.f, 550.f));
	pItemBox->Collider3D()->SetOffsetPos(Vec3(0.f, 70.f, 0.f));
	pItemBox->Collider3D()->SetCollider3DType(COLLIDER3D_TYPE::CUBE);
	pItemBox->AddComponent(new ER_DataScript_ItemBox);
	pItemBox->MeshRender()->GetDynamicMaterial(0);
	SpawnGameObject(pItemBox, Vec3(-66.021f, -0.499f, 101.998f), L"ItemBox");

	pMeshData = CResMgr::GetInst()->FindRes<CMeshData>(L"School_WreckCar02.mdat");
	pItemBox = pMeshData->Instantiate();
	pItemBox->SetName(L"School_WreckCar02");
	pItemBox->Transform()->SetRelativeScale(0.01f, 0.01f, 0.01f);
	pItemBox->AddComponent(new CCollider3D);
	pItemBox->Collider3D()->SetOffsetScale(Vec3(200.f, 100.f, 550.f));
	pItemBox->Collider3D()->SetOffsetPos(Vec3(0.f, 70.f, 0.f));
	pItemBox->Collider3D()->SetCollider3DType(COLLIDER3D_TYPE::CUBE);
	pItemBox->AddComponent(new ER_DataScript_ItemBox);
	pItemBox->MeshRender()->GetDynamicMaterial(0);
	SpawnGameObject(pItemBox, Vec3(-84.585f, -1.220f, 47.429f), L"ItemBox");
#pragma endregion

#pragma region Uptown
	pMeshData = CResMgr::GetInst()->FindRes<CMeshData>(L"Uptown_BarbequeGrill01.mdat");
	pItemBox = pMeshData->Instantiate();
	pItemBox->SetName(L"Uptown_BarbequeGrill01");
	pItemBox->Transform()->SetRelativeScale(0.01f, 0.01f, 0.01f);
	pItemBox->AddComponent(new CCollider3D);
	pItemBox->Collider3D()->SetOffsetScale(Vec3(120.f, 150.f, 150.f));
	pItemBox->Collider3D()->SetOffsetPos(Vec3(0.f, 70.f, 0.f));
	pItemBox->Collider3D()->SetCollider3DType(COLLIDER3D_TYPE::CUBE);
	pItemBox->AddComponent(new ER_DataScript_ItemBox);
	pItemBox->MeshRender()->GetDynamicMaterial(0);
	SpawnGameObject(pItemBox, Vec3(-13.929f, -0.333f, -50.756f), L"ItemBox");

	pMeshData = CResMgr::GetInst()->FindRes<CMeshData>(L"Uptown_Sedan01.mdat");
	pItemBox = pMeshData->Instantiate();
	pItemBox->SetName(L"Uptown_Sedan01");
	rot = pItemBox->Transform()->GetRelativeRot();
	rot.x = XMConvertToRadians(-180.000f);
	rot.y = XMConvertToRadians(9.000f);
	rot.z = XMConvertToRadians(-180.000f);
	pItemBox->Transform()->SetRelativeRot(rot);
	pItemBox->Transform()->SetRelativeScale(0.01f, 0.01f, 0.01f);
	pItemBox->AddComponent(new CCollider3D);
	pItemBox->Collider3D()->SetOffsetScale(Vec3(200.f, 100.f, 550.f));
	pItemBox->Collider3D()->SetOffsetPos(Vec3(0.f, 70.f, 0.f));
	pItemBox->Collider3D()->SetCollider3DType(COLLIDER3D_TYPE::CUBE);
	pItemBox->AddComponent(new ER_DataScript_ItemBox);
	SpawnGameObject(pItemBox, Vec3(-5.619f, -0.523f, -65.105f), L"ItemBox");

	pMeshData = CResMgr::GetInst()->FindRes<CMeshData>(L"Uptown_SteelBox01.mdat");
	pItemBox = pMeshData->Instantiate();
	pItemBox->SetName(L"Uptown_SteelBox01");
	pItemBox->Transform()->SetRelativeScale(0.01f, 0.01f, 0.01f);
	pItemBox->AddComponent(new CCollider3D);
	pItemBox->Collider3D()->SetOffsetScale(Vec3(100.f, 200.f, 100.f));
	pItemBox->Collider3D()->SetOffsetPos(Vec3(0.f, 100.f, 0.f));
	pItemBox->Collider3D()->SetCollider3DType(COLLIDER3D_TYPE::CUBE);
	pItemBox->AddComponent(new ER_DataScript_ItemBox);
	pItemBox->MeshRender()->GetDynamicMaterial(0);
	SpawnGameObject(pItemBox, Vec3(31.074f, -0.497f, -41.496f), L"ItemBox");

	pMeshData = CResMgr::GetInst()->FindRes<CMeshData>(L"Uptown_WreckCar01.mdat");
	pItemBox = pMeshData->Instantiate();
	pItemBox->SetName(L"Uptown_WreckCar01");
	rot = pItemBox->Transform()->GetRelativeRot();
	rot.x = XMConvertToRadians(-180.000f);
	rot.y = XMConvertToRadians(-90.000f);
	rot.z = XMConvertToRadians(-180.000f);
	pItemBox->Transform()->SetRelativeRot(rot);
	pItemBox->Transform()->SetRelativeScale(0.01f, 0.01f, 0.01f);
	pItemBox->AddComponent(new CCollider3D);
	pItemBox->Collider3D()->SetOffsetScale(Vec3(200.f, 100.f, 550.f));
	pItemBox->Collider3D()->SetOffsetPos(Vec3(0.f, 70.f, 0.f));
	pItemBox->Collider3D()->SetCollider3DType(COLLIDER3D_TYPE::CUBE);
	pItemBox->AddComponent(new ER_DataScript_ItemBox);
	pItemBox->MeshRender()->GetDynamicMaterial(0);
	SpawnGameObject(pItemBox, Vec3(27.374f, -0.969f, -76.997f), L"ItemBox");
#pragma endregion
}
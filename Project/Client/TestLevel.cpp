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


void CreateTestLevel()
{
	CLevel* pCurLevel = CLevelMgr::GetInst()->GetCurLevel();
	pCurLevel->ChangeState(LEVEL_STATE::STOP);

	// Layer 이름설정
	pCurLevel->GetLayer(0)->SetName(L"Default");
	pCurLevel->GetLayer(1)->SetName(L"SkyBox");
	pCurLevel->GetLayer(2)->SetName(L"House");
	pCurLevel->GetLayer(3)->SetName(L"Monster");
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

	SpawnGameObject(pLightObj, Vec3(-2000, 2000.f, -2000.f), 0);

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

	
	
	//// ============
	//// FBX Loading
	//// ============	
	//{
	//	Ptr<CMeshData> pMeshData = nullptr;
	//	CGameObject* pObj = nullptr;
	//
	//	//pMeshData = CResMgr::GetInst()->LoadFBX(L"fbx\\house.fbx");
	//	//pMeshData = CResMgr::GetInst()->FindRes<CMeshData>(L"meshdata\\house.mdat");
	//	//pObj = pMeshData->Instantiate();
	//	//pObj->SetName(L"House");
	//
	//	//pMeshData = CResMgr::GetInst()->LoadFBX(L"fbx\\monster.fbx");
	//	//pMeshData = CResMgr::GetInst()->Load<CMeshData>(L"meshdata\\monster.mdat", L"meshdata\\monster.mdat");
	//	pMeshData = CResMgr::GetInst()->FindRes<CMeshData>(L"meshdata\\monster.mdat");
	//	pObj = pMeshData->Instantiate();
	//	pObj->SetName(L"Monster");
	//	pObj->Transform()->SetRelativeScale(Vec3(10.f, 10.f, 10.f));
	//
	//	SpawnGameObject(pObj, Vec3(0.f, 0.f, 100.f), L"Default");
	//}

	// ============
	// FBX Loading
	// ============	
	{
		// 인스턴싱 테스트		
		Ptr<CMeshData> pMeshData = nullptr;
		CGameObject* pObj = nullptr;
		// 
		// pMeshData = CResMgr::GetInst()->LoadFBX(L"fbx\\House.fbx");
		// for (int i = 0; i < 10; ++i)
		// {
		// 	pObj = pMeshData->Instantiate();
		// 	pObj->SetName(L"House");
		// 	SpawnGameObject(pObj, Vec3((i + 1) * 300.f, 200.f, 500.f), 2);
		// }

		pMeshData = CResMgr::GetInst()->FindRes<CMeshData>(L"meshdata\\Hyunwoo_Craft.mdat");
		//for (int i = 0; i < 10; ++i)
		//{
			pObj = pMeshData->Instantiate();
			pObj->SetName(L"HW");
			SpawnGameObject(pObj, Vec3(50.f, 200.f, 100.f), 0);
			pObj->Transform()->SetRelativeScale(Vec3(100.f , 100.f, 100.f));
		//}
	}

	// LandScape Object
	// CGameObject* pLandScape = new CGameObject;
	// pLandScape->SetName(L"LandScape");
	// 
	// pLandScape->AddComponent(new CTransform);
	// pLandScape->AddComponent(new CLandScape);
	// 
	// pLandScape->Transform()->SetRelativeScale(Vec3(500.f, 3000.f, 500.f));
	// 
	// pLandScape->LandScape()->SetFace(64, 64);
	// pLandScape->LandScape()->SetFrustumCheck(false);
	// //pLandScape->LandScape()->SetHeightMap(CResMgr::GetInst()->FindRes<CTexture>(L"HeightMap_01.jpg"));
	// 
	// SpawnGameObject(pLandScape, Vec3(0.f, 0.f, 0.f), 0);

	// 충돌 시킬 레이어 짝 지정
	//CCollisionMgr::GetInst()->LayerCheck(L"Player", L"Monster");
}

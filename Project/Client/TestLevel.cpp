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
#include <Script\CPlanetScript.h>
#include <Script\CCloudScript.h>

#include "CLevelSaveLoad.h"


#include <Engine/CSetColorShader.h>


void CreateTestLevel()
{
	CLevel* pCurLevel = CLevelMgr::GetInst()->GetCurLevel();
	pCurLevel->ChangeState(LEVEL_STATE::STOP);
	pCurLevel->SetName(L"TestLevel");

	// Layer 이름설정
	pCurLevel->GetLayer(0)->SetName(L"Default");
	pCurLevel->GetLayer(1)->SetName(L"SkyBox");
	pCurLevel->GetLayer(2)->SetName(L"Player");
	pCurLevel->GetLayer(3)->SetName(L"Particle");
	pCurLevel->GetLayer(31)->SetName(L"ViewPort UI");


	// Main Camera Object 생성
	CGameObject* pMainCam = new CGameObject;
	pMainCam->SetName(L"MainCamera");

	pMainCam->AddComponent(new CTransform);
	pMainCam->AddComponent(new CCamera);
	pMainCam->AddComponent(new CCameraMoveScript);

	pMainCam->Transform()->SetRelativeRot(Vec3(XM_PI / 6.f, -XM_PI / 6.f, 0.f));

	pMainCam->Camera()->SetProjType(PROJ_TYPE::PERSPECTIVE);
	pMainCam->Camera()->SetCameraIndex(0);		// MainCamera 로 설정
	pMainCam->Camera()->SetLayerMaskAll(true);	// 모든 레이어 체크
	pMainCam->Camera()->SetLayerMask(31, false);// UI Layer 는 렌더링하지 않는다.

	SpawnGameObject(pMainCam, Vec3(430.f, 490.f, -780.f), 0);

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
	 
	 
	// SkyBox 추가
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
	pLightObj->SetName(L"SunLight");
	
	pLightObj->AddComponent(new CTransform);
	pLightObj->AddComponent(new CLight3D);

	pLightObj->Light3D()->SetLightType(LIGHT_TYPE::DIRECTIONAL);
	pLightObj->Light3D()->SetRadius(4000.f);
	pLightObj->Light3D()->SetLightColor(Vec3(1.f, 1.f, 1.f));	
	pLightObj->Light3D()->SetLightAmbient(Vec3(0.1f, 0.1f, 0.1f));
	
	SpawnGameObject(pLightObj, Vec3(0.f, 0.f, 0.f), 0);
	


	CGameObject* TestObj = new CGameObject;
	TestObj->SetName(L"TestObj");
	TestObj->AddComponent(new CTransform);
	
	TestObj->Transform()->SetRelativeScale(Vec3(100.f, 100.f, 100.f));

	TestObj->AddComponent(new CParticleSystem);
	TestObj->ParticleSystem()->SetSpawnRate(5);
	TestObj->ParticleSystem()->SetSpawnInitialColor(Vec3(1.f, 0.f, 1.f));

	SpawnGameObject(TestObj, Vec3(0.f, 0.f, 500.f), L"Player");

	/*
	float dist = 100.f;
	float radius = 0.f;

	// 행성 오브젝트
	// 태양
	CGameObject* Sun = new CGameObject;
	Sun->SetName(L"Sun");
	Sun->AddComponent(new CTransform);
	Sun->AddComponent(new CMeshRender);
	Sun->AddComponent(new CPlanetScript);
	Sun->AddComponent(new CLight3D);

	Sun->Light3D()->SetLightType(LIGHT_TYPE::POINT);
	Sun->Light3D()->SetRadius(110.f);
	Sun->Light3D()->SetLightColor(Vec3(10.f, 10.f, 10.f));
	Sun->Light3D()->SetLightAmbient(Vec3(1.f, 1.f, 1.f));

	Sun->Transform()->SetRelativeScale(Vec3(220.f, 220.f, 220.f));
	Sun->Transform()->SetRelativeRot(Vec3(0.f, 0.f, 0.f));

	Sun->GetScript<CPlanetScript>()->SetPlanet(radius, 0.f, 0.99f, Vec3(0.f, 1.f, 0.f), Vec3(0.f, 10.f, 0.7f));

	Sun->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"SphereMesh"));
	Sun->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"SunMtrl.mtrl"));
	

	SpawnGameObject(Sun, Vec3(0.f, 0.f, 0.f), L"Planet");

	CGameObject* SunEffect = new CGameObject;
	SunEffect->SetName(L"SunEffect");
	SunEffect->AddComponent(new CTransform);
	SunEffect->AddComponent(new CMeshRender);
	SunEffect->Transform()->SetRelativeScale(Vec3(0.95f, 0.95f, 0.95f));
	SunEffect->Transform()->SetRelativePos(Vec3(0.f, 0.f, 0.f));

	SunEffect->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"SphereMesh"));
	SunEffect->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"DistortionMtrl"));

	Sun->AddChild(SunEffect);
	Sun->AddChild(pLightObj);

	// 수성
	CGameObject* Mercury = new CGameObject;
	Mercury->SetName(L"Mercury");
	Mercury->AddComponent(new CTransform);
	Mercury->AddComponent(new CMeshRender);
	Mercury->AddComponent(new CPlanetScript);
	Mercury->Transform()->SetRelativeScale(Vec3(42.f, 42.f, 42.f));
	Mercury->Transform()->SetRelativeRot(Vec3(0.f, 0.f, 0.f));

	radius = dist * 2.2f;
	Mercury->GetScript<CPlanetScript>()->SetPlanet(radius, 0.478f, 0.03f, Vec3(0.f, 1.f, 0.07f) , Vec3(0.f, 10.f, 0.01f));

	Mercury->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"SphereMesh"));
	Mercury->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"MercuryMtrl.mtrl"));

	SpawnGameObject(Mercury, Vec3(radius, 0.f, 0.f), L"Planet");

	// 금성
	CGameObject* Venus = new CGameObject;
	Venus->SetName(L"Venus");
	Venus->AddComponent(new CTransform);
	Venus->AddComponent(new CMeshRender);
	Venus->AddComponent(new CPlanetScript);
	Venus->Transform()->SetRelativeScale(Vec3(56.f, 56.f, 56.f));
	Venus->Transform()->SetRelativeRot(Vec3(0.f, 0.f, 0.f));

	radius = dist * 2.9f;
	Venus->GetScript<CPlanetScript>()->SetPlanet(radius, 0.358f, 0.001f, Vec3(0.f, 1.f, 0.03f), Vec3(0.f, 10.f, 0.2f));

	Venus->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"SphereMesh"));
	Venus->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"VenusMtrl.mtrl"));

	SpawnGameObject(Venus, Vec3(radius, 0.f, 0.f), L"Planet");

	CGameObject* VenusAtmoSphere = new CGameObject;
	VenusAtmoSphere->SetName(L"VenusAtmoSphere");
	VenusAtmoSphere->AddComponent(new CTransform);
	VenusAtmoSphere->AddComponent(new CMeshRender);
	VenusAtmoSphere->AddComponent(new CCloudScript);
	VenusAtmoSphere->Transform()->SetRelativeScale(Vec3(1.05f, 1.05f, 1.05f));
	VenusAtmoSphere->Transform()->SetRelativeRot(Vec3(0.f, 1.f, 0.2f));

	VenusAtmoSphere->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"SphereMesh"));
	VenusAtmoSphere->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"VenusAtmoSphereMtrl.mtrl"));

	Venus->AddChild(VenusAtmoSphere);

	// 지구
	CGameObject* Earth = new CGameObject;
	Earth->SetName(L"Earth");
	Earth->AddComponent(new CTransform);
	Earth->AddComponent(new CMeshRender);
	Earth->AddComponent(new CPlanetScript);
	Earth->Transform()->SetRelativeScale(Vec3(70.f, 70.f, 70.f));
	Earth->Transform()->SetRelativeRot(Vec3(0.f, 0.f, 0.f));
	
	radius = dist * 3.8f;
	Earth->GetScript<CPlanetScript>()->SetPlanet(radius, 0.297f, 0.46f, Vec3(0.f, 1.f, 0.f), Vec3(0.f, 10.f, 2.3f));

	Earth->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"SphereMesh"));
	Earth->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"EarthMtrl.mtrl"));
	
	SpawnGameObject(Earth, Vec3(radius, 0.f, 0.f), L"Planet");

	CGameObject* Moon = new CGameObject;
	Moon->SetName(L"Moon");
	Moon->AddComponent(new CTransform);
	Moon->AddComponent(new CMeshRender);
	Moon->AddComponent(new CPlanetScript);
	Moon->Transform()->SetRelativePos(Vec3(1.f, 1.f, 0.f));
	Moon->Transform()->SetRelativeScale(Vec3(0.3f, 0.3f, 0.3f));
	Moon->Transform()->SetRelativeRot(Vec3(0.f, 0.f, 0.f));

	radius = dist * 1.f;
	Moon->GetScript<CPlanetScript>()->SetPlanet(radius, 0.01f, 0.0046f, Vec3(0.f, 1.f, 0.f), Vec3(0.f, 1.f, 0.f));
	Moon->GetScript<CPlanetScript>()->SetParentPlanet(Earth);

	Moon->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"SphereMesh"));
	Moon->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"MoonMtrl.mtrl"));

	Earth->AddChild(Moon);

	CGameObject* pCloud = new CGameObject;
	pCloud->SetName(L"EarthCloud");
	pCloud->AddComponent(new CTransform);
	pCloud->AddComponent(new CMeshRender);
	pCloud->AddComponent(new CCloudScript);
	pCloud->Transform()->SetRelativeScale(Vec3(1.1f, 1.1f, 1.1f));
	pCloud->Transform()->SetRelativeRot(Vec3(0.f, 1.f, -0.2f));
	
	pCloud->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"SphereMesh"));
	pCloud->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"EarthCloudMtrl.mtrl"));
	
	Earth->AddChild(pCloud);

	// 화성
	CGameObject* Mars = new CGameObject;
	Mars->SetName(L"Mars");
	Mars->AddComponent(new CTransform);
	Mars->AddComponent(new CMeshRender);
	Mars->AddComponent(new CPlanetScript);
	Mars->Transform()->SetRelativeScale(Vec3(50.f, 50.f, 50.f));
	Mars->Transform()->SetRelativeRot(Vec3(0.f, 0.f, 0.f));

	radius = dist * 4.6f;
	Mars->GetScript<CPlanetScript>()->SetPlanet(radius, 0.241f, 0.2f, Vec3(0.f, 1.f, 0.18f), Vec3(0.f, 10.f, 2.5f));

	Mars->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"SphereMesh"));
	Mars->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"MarsMtrl.mtrl"));

	SpawnGameObject(Mars, Vec3(radius, 0.f, 0.f), L"Planet");

	// 목성
	CGameObject* Jupiter = new CGameObject;
	Jupiter->SetName(L"Jupiter");
	Jupiter->AddComponent(new CTransform);
	Jupiter->AddComponent(new CMeshRender);
	Jupiter->AddComponent(new CPlanetScript);
	Jupiter->Transform()->SetRelativeScale(Vec3(200.f, 200.f, 200.f));
	Jupiter->Transform()->SetRelativeRot(Vec3(0.f, 0.f, 0.f));

	radius = dist * 7.2f;
	Jupiter->GetScript<CPlanetScript>()->SetPlanet(radius, 0.13f, 12.6f, Vec3(0.f, 1.f, 0.13f), Vec3(0.0f, 1.0f, 0.03f));

	Jupiter->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"SphereMesh"));
	Jupiter->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"JupiterMtrl.mtrl"));

	SpawnGameObject(Jupiter, Vec3(radius, 0.f, 0.f), L"Planet");

	// 토성
	CGameObject* Saturn = new CGameObject;
	Saturn->SetName(L"Saturn");
	Saturn->AddComponent(new CTransform);
	Saturn->AddComponent(new CMeshRender);
	Saturn->AddComponent(new CPlanetScript);
	Saturn->Transform()->SetRelativeScale(Vec3(130.f, 130.f, 130.f));
	Saturn->Transform()->SetRelativeRot(Vec3(0.f, 0.f, 0.f));

	radius = dist * 10.5f;
	Saturn->GetScript<CPlanetScript>()->SetPlanet(radius, 0.096f, 9.87f, Vec3(0.f, 1.f, 0.24f), Vec3(0.f, 10.f, 2.6f));
	
	Saturn->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"SphereMesh"));
	Saturn->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"SaturnMtrl.mtrl"));

	SpawnGameObject(Saturn, Vec3(radius, 0.f, 0.f), L"Planet");

	CGameObject* SaturnRing = new CGameObject;
	SaturnRing->SetName(L"SaturnRing");
	SaturnRing->AddComponent(new CTransform);
	SaturnRing->AddComponent(new CMeshRender);
	SaturnRing->Transform()->SetRelativeScale(Vec3(2.f, 2.f, 2.f));
	SaturnRing->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RingMesh"));
	SaturnRing->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"SaturnRingMtrl.mtrl"));

	Saturn->AddChild(SaturnRing);

	// 천왕성
	CGameObject* Uranus = new CGameObject;
	Uranus->SetName(L"Uranus");
	Uranus->AddComponent(new CTransform);
	Uranus->AddComponent(new CMeshRender);
	Uranus->AddComponent(new CPlanetScript);
	Uranus->Transform()->SetRelativeScale(Vec3(120.f, 120.f, 120.f));
	Uranus->Transform()->SetRelativeRot(Vec3(0.f, 0.f, 0.f));

	radius = dist * 13.2f;
	Uranus->GetScript<CPlanetScript>()->SetPlanet(radius, 0.068f, 2.59f, Vec3(0.f, 1.f, 0.07f), Vec3(0.f, 10.f, 0.8f));

	Uranus->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"SphereMesh"));
	Uranus->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"UranusMtrl.mtrl"));

	SpawnGameObject(Uranus, Vec3(radius, 0.f, 0.f), L"Planet");

	// 해왕성
	CGameObject* Neptune = new CGameObject;
	Neptune->SetName(L"Neptune");
	Neptune->AddComponent(new CTransform);
	Neptune->AddComponent(new CMeshRender);
	Neptune->AddComponent(new CPlanetScript);
	Neptune->Transform()->SetRelativeScale(Vec3(120.f, 120.f, 120.f));
	Neptune->Transform()->SetRelativeRot(Vec3(0.f, 0.f, 0.f));

	radius = dist * 15.1f;
	Neptune->GetScript<CPlanetScript>()->SetPlanet(radius, 0.054f, 2.68f, Vec3(0.f, 1.f, 0.17f), Vec3(0.f, 10.f, 0.28f));

	Neptune->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"SphereMesh"));
	Neptune->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"NeptuneMtrl.mtrl"));

	SpawnGameObject(Neptune, Vec3(radius, 0.f, 0.f), L"Planet");
	*/

}

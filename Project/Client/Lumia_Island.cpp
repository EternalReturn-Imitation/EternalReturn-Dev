#include "pch.h"
#include "LevelMgr.h"

#pragma region HEADER

// [Engine Manager]
#include <Engine\CResMgr.h>
#include <Engine\CPathFindMgr.h>
#include <Engine\CCollisionMgr.h>
#include <Engine\CLevelMgr.h>
#include <Engine\CRenderMgr.h>
#include <Engine\CFontMgr.h>

// [Level&Object]
#include <Engine\CLevel.h>
#include <Engine\CLayer.h>
#include <Engine\CGameObject.h>

// [Component]
#include <Engine\components.h>
#include <Engine\CCollider2D.h>
#include <Engine\CCollider3D.h>
#include <Engine\CFindPath.h>
#include <Engine\CText.h>
#include <Engine\CUIComponent.h>

// [Graphic]
#include <Engine\CSetColorShader.h>


// [Script&GameSystem]
#include <Script\CEditCamControlScript.h>
#include <Script\CFollowMainCamScript.h>
#include <Script\ER_CamControllerScript.h>
#include <Script\ER_DataScript_ItemBox.h>
#include <Script\ER_DataScript_LandMeshBase.h>

#include <Script\ER_CharacterMgr.h>
#include <Script\ER_UIMgr.h>
#include <Script\ER_GameSystem.h>

// [Editor]
#include "CEditorObjMgr.h"
#include "CLevelSaveLoad.h"
#include "LevelMgr.h"

#pragma endregion

#include <Script\ER_ProjectileScript.h>

void CreateLumiaIsland()
{
	CLevel* pCurLevel = CLevelMgr::GetInst()->GetCurLevel();
	pCurLevel->ChangeState(LEVEL_STATE::STOP);

	// Level Setting
	SetLayer(pCurLevel);
	SetCamera();
	SetLight();
	SetMapCollider();

	CreateTestPlayer();
	CreateTestEnemy();

	LumiaIsland();
	
	ER_GameSystem::GetInst()->GameStart();

//	TestObject();
}

void CreateTestPlayer()
{
	ER_CharacterMgr::GetInst()->SpawnCharacter_Player(L"Hyunwoo", Vec3(-69.3f, 0.0f, 37.6f));
}
void CreateTestEnemy()
{
	ER_CharacterMgr::GetInst()->SpawnCharacter_Enemy(L"Jackie", Vec3(-78.5f, 0.0f, 34.3f));
	ER_CharacterMgr::GetInst()->SpawnCharacter_Enemy(L"Aya", Vec3(-80.4f, 1.00345f, 44.8f));
	ER_CharacterMgr::GetInst()->SpawnCharacter_Enemy(L"Rio", Vec3(-76.9f, 0.0f, 37.6f));
	ER_CharacterMgr::GetInst()->SpawnCharacter_Enemy(L"Yuki", Vec3(-71.9f, 0.0f, 37.6f));

}

void TestObject()
{
	// CGameObject* testParticle = new CGameObject();
	// CGameObject* testParticle = xnew<CGameObject>();
	// CGameObject* testParticle = new(CGameObject);
	// 
	// //CGameObject* testParticle111 = onew(CGameObject);
	// //odelete(testParticle111);
	// 
	// testParticle->SetName(L"particleTest");
	// AddComponents(testParticle, _TRANSFORM | _PARTICLESYSTEM);
	// CParticleSystem* Particle = testParticle->ParticleSystem();
}

void SetLayer(CLevel* _Level)
{
	// Layer Set
	_Level->GetLayer(0)->SetName(L"System");
	_Level->GetLayer(1)->SetName(L"SkyBox");
	_Level->GetLayer(2)->SetName(L"Base");
	_Level->GetLayer(3)->SetName(L"Building");
	_Level->GetLayer(4)->SetName(L"Roof");
	
	_Level->GetLayer(5)->SetName(L"ItemBox");
	_Level->GetLayer(11)->SetName(L"Monster");
	_Level->GetLayer(12)->SetName(L"Character");
	_Level->GetLayer(13)->SetName(L"Player");
	
	_Level->GetLayer(14)->SetName(L"Projectile");

	_Level->GetLayer(28)->SetName(L"InGameUI");
	_Level->GetLayer(29)->SetName(L"MapCollider");
	_Level->GetLayer(30)->SetName(L"Camera");
	_Level->GetLayer(31)->SetName(L"UI");

	// Collision Set
	CCollisionMgr::GetInst()->LayerCheck(L"Monster", L"Monster");
	CCollisionMgr::GetInst()->LayerCheck(L"Player", L"Base");
	CCollisionMgr::GetInst()->LayerCheck(L"ItemBox", L"MapCollider");
	CCollisionMgr::GetInst()->LayerCheck(L"ItemBox", L"Character");
	CCollisionMgr::GetInst()->LayerCheck(L"ItemBox", L"Player");

	CCollisionMgr::GetInst()->LayerCheck(L"Player", L"Projectile");
	CCollisionMgr::GetInst()->LayerCheck(L"Character", L"Projectile");

	// Ray LayerCheck
	CCollisionMgr::GetInst()->RayLayerCheck(L"ItemBox");
	CCollisionMgr::GetInst()->RayLayerCheck(L"Character");
	CCollisionMgr::GetInst()->RayLayerCheck(L"Monster");

}
void SetCamera()
{
	// [ Main ]
	CGameObject* MainCamera = new CGameObject;
	MainCamera->SetName(L"MainCamera");
	AddComponents(MainCamera, _TRANSFORM | _CAMERA);
	MainCamera->AddComponent(new ER_CamControllerScript);

	// Transform
	MainCamera->Transform()->SetRelativeRot(Vec3(Deg2Rad(54.f), Deg2Rad(-45.f), 0.f));
	
	// CamSet
	MainCamera->Camera()->SetFar(500.f);
	MainCamera->Camera()->SetFOV(Deg2Rad(30));
	MainCamera->Camera()->SetProjType(PROJ_TYPE::PERSPECTIVE);
	MainCamera->Camera()->SetCameraIndex(0);
	MainCamera->Camera()->SetLayerMaskAll(true);
	MainCamera->Camera()->SetLayerMask(31, false);

	SpawnGameObject(MainCamera, Vec3(0.f, 20.f, 0.f), L"Camera");

	// [ UI ]

	CGameObject* UICamera = new CGameObject;
	UICamera->SetName(L"UICamera");
	AddComponents(UICamera, _TRANSFORM | _CAMERA);

	UICamera->Camera()->SetFar(10.f);
	UICamera->Camera()->SetProjType(PROJ_TYPE::ORTHOGRAPHIC);
	UICamera->Camera()->SetCameraIndex(1);

	Vec2 WinResolution = CEngine::GetInst()->GetWindowResolution();
	
	UICamera->Camera()->SetOrthoWidth(WinResolution.x);
	UICamera->Camera()->SetOrthoHeight(WinResolution.y);

	UICamera->Camera()->SetLayerMaskAll(false);
	UICamera->Camera()->SetLayerMask(31, true);

	SpawnGameObject(UICamera, Vec3(0.f, 0.f, -5.f), L"Camera");
}
void SetLight()
{
	CGameObject* pLightObj = new CGameObject;
	pLightObj->SetName(L"MainLight");

	AddComponents(pLightObj, _TRANSFORM | _LIGHT3D);
	pLightObj->AddComponent(new CFollowMainCamScript);
	pLightObj->GetScript<CFollowMainCamScript>()->SetTarget(Vec3(-14.f, 0.f, 0.f));
	pLightObj->Transform()->SetRelativeScale(Vec3(1.f, 1.f, 1.f));
	pLightObj->Transform()->SetRelativeRot(Vec3(Deg2Rad(55.f), 0.f, Deg2Rad(5.f)));

	// Light Set
	pLightObj->Light3D()->SetLightType(LIGHT_TYPE::DIRECTIONAL);
	pLightObj->Light3D()->SetRadius(500.f);
	pLightObj->Light3D()->GetLightRenderCam()->SetFar(200.f);
	pLightObj->Light3D()->GetLightRenderCam()->SetOrthoWidth(30.f);
	pLightObj->Light3D()->GetLightRenderCam()->SetOrthoHeight(30.f);
	pLightObj->Light3D()->SetLightColor(Vec3(1.f, 1.f, 1.f));
	pLightObj->Light3D()->SetLightAmbient(Vec3(0.f, 0.f, 0.f));

	pLightObj->Light3D()->GetLightRenderCam()->SetLayerMaskAll(true);
	pLightObj->Light3D()->GetLightRenderCam()->SetLayerMask(28, false);	// InGameUI
	pLightObj->Light3D()->GetLightRenderCam()->SetLayerMask(31, false);	// UI

	SpawnGameObject(pLightObj, Vec3(0.f, 0.f, 0.f), L"System");
}
void SetMapCollider()
{
	CGameObject* MapCollider = new CGameObject;
	MapCollider->SetName(L"MapCollider");
	AddComponents(MapCollider, _TRANSFORM | _COLLIDER2D);
	
	MapCollider->Collider2D()->SetOffsetRot(Vec3(XMConvertToRadians(90.f), 0.f, 0.f));
	MapCollider->Collider2D()->SetOffsetScale(Vec2(256.f, 240.f));
	MapCollider->Collider2D()->SetOffsetPos(Vec3(0.f, 0.f, 0.f));
	
	CPathFindMgr::GetInst()->SetMapCollider(MapCollider);
	SpawnGameObject(MapCollider, Vec3(-37.98901f, 0.f, 0.62141f), L"MapCollider");
}


void LumiaIsland()
{
	Create_Archery();
	Create_Forest();
	Create_Hotel();
	Create_SandyBeach();
	Create_School();
	Create_Uptown();
}

#define REVERSE90RAD	-1.5708f
#define LANDSCALE		0.01f

void Create_Archery()
{
	CGameObject* Base = CResMgr::GetInst()->FindRes<CMeshData>(L"Archery_Base.mdat")->Instantiate();
	CGameObject* Building = CResMgr::GetInst()->FindRes<CMeshData>(L"Archery_Building.mdat")->Instantiate();
	CGameObject* Roof = CResMgr::GetInst()->FindRes<CMeshData>(L"Archery_Roof.mdat")->Instantiate();

	Base->SetName(L"Archery_Base");
	Building->SetName(L"Archery_Building");
	Roof->SetName(L"Archery_Roof");

	Base->LoadAllPrefabFromObjName();
	Building->LoadAllPrefabFromObjName();
	Roof->LoadAllPrefabFromObjName();

	SpawnGameObject(Base, L"Base");
	SpawnGameObject(Building, L"Building");
	SpawnGameObject(Roof, L"Roof");


	// [ Collider ]
	CGameObject* RoofEnable = new CGameObject;
	RoofEnable->SetName(L"Archery_Base_Collider3D");
	AddComponents(RoofEnable, _TRANSFORM | _COLLIDER3D);

	RoofEnable->LoadAllPrefabFromObjName();

	RoofEnable->AddComponent(new ER_DataScript_LandMeshBase);
	RoofEnable->GetScript<ER_DataScript_LandMeshBase>()->SetRoof(Roof);

	SpawnGameObject(RoofEnable, L"Base");

	// ====================================

	CGameObject* ItemBox[3] = {};

	ItemBox[0] = CResMgr::GetInst()->FindRes<CMeshData>(L"Archery_Biotoilet01.mdat")->Instantiate();
	ItemBox[1] = CResMgr::GetInst()->FindRes<CMeshData>(L"Archery_Locker01.mdat")->Instantiate();
	ItemBox[2] = CResMgr::GetInst()->FindRes<CMeshData>(L"Archery_SteelBox01.mdat")->Instantiate();
	
	ItemBox[0]->SetName(L"Archery_Biotoilet01");
	ItemBox[1]->SetName(L"Archery_Locker01");
	ItemBox[2]->SetName(L"Archery_SteelBox01");
	
	for (auto& box : ItemBox)
	{
		AddComponents(box, _COLLIDER3D);
		box->Collider3D()->SetCollider3DType(COLLIDER3D_TYPE::CUBE);
		box->AddComponent(new ER_DataScript_ItemBox);
		box->MeshRender()->GetDynamicMaterial(0);
		box->LoadAllPrefabFromObjName();
		SpawnGameObject(box, L"ItemBox");
	}
}
void Create_Forest()
{
	CGameObject* Base = CResMgr::GetInst()->FindRes<CMeshData>(L"Forest_Base.mdat")->Instantiate();
	CGameObject* Building = CResMgr::GetInst()->FindRes<CMeshData>(L"Forest_Building.mdat")->Instantiate();

	Base->SetName(L"Forest_Base");
	Building->SetName(L"Forest_Building");

	Base->LoadAllPrefabFromObjName();
	Building->LoadAllPrefabFromObjName();

	SpawnGameObject(Base, L"Base");
	SpawnGameObject(Building, L"Building");

	// =========================================================

	CGameObject* ItemBox[4] = {};

	ItemBox[0] = CResMgr::GetInst()->FindRes<CMeshData>(L"Forest_PalletLoaded01.mdat")->Instantiate();
	ItemBox[1] = CResMgr::GetInst()->FindRes<CMeshData>(L"Forest_SteelBox01.mdat")->Instantiate();
	ItemBox[2] = CResMgr::GetInst()->FindRes<CMeshData>(L"Forest_TreeStump01.mdat")->Instantiate();
	ItemBox[3] = CResMgr::GetInst()->FindRes<CMeshData>(L"Forest_TreeStump02.mdat")->Instantiate();
	
	ItemBox[0]->SetName(L"Forest_PalletLoaded01");
	ItemBox[1]->SetName(L"Forest_SteelBox01");
	ItemBox[2]->SetName(L"Forest_TreeStump01");
	ItemBox[3]->SetName(L"Forest_TreeStump02");

	for (auto& box : ItemBox)
	{
		AddComponents(box, _COLLIDER3D);
		box->Collider3D()->SetCollider3DType(COLLIDER3D_TYPE::CUBE);
		box->AddComponent(new ER_DataScript_ItemBox);

		box->MeshRender()->GetDynamicMaterial(0);
		box->LoadAllPrefabFromObjName();
		SpawnGameObject(box, L"ItemBox");
	}
}
void Create_Hotel()
{
	CGameObject* Base = CResMgr::GetInst()->FindRes<CMeshData>(L"Hotel_Base.mdat")->Instantiate();
	CGameObject* Building = CResMgr::GetInst()->FindRes<CMeshData>(L"Hotel_Building.mdat")->Instantiate();
	CGameObject* Roof = CResMgr::GetInst()->FindRes<CMeshData>(L"Hotel_Roof.mdat")->Instantiate();
	
	Base->SetName(L"Hotel_Base");
	Building->SetName(L"Hotel_Building");
	Roof->SetName(L"Hotel_Roof");
	
	Base->LoadAllPrefabFromObjName();
	Building->LoadAllPrefabFromObjName();
	Roof->LoadAllPrefabFromObjName();

	SpawnGameObject(Base, L"Base");
	SpawnGameObject(Building, L"Building");
	SpawnGameObject(Roof, L"Roof");

	// [ Collider ]
	CGameObject* RoofEnable = new CGameObject;
	RoofEnable->SetName(L"Hotel_Base_Collider3D");
	AddComponents(RoofEnable, _TRANSFORM | _COLLIDER3D);
	RoofEnable->LoadAllPrefabFromObjName();

	RoofEnable->AddComponent(new ER_DataScript_LandMeshBase);
	RoofEnable->GetScript<ER_DataScript_LandMeshBase>()->SetRoof(Roof);
	
	SpawnGameObject(RoofEnable, L"Base");

	// =====================================================================

	CGameObject* ItemBox[4] = {};

	ItemBox[0] = CResMgr::GetInst()->FindRes<CMeshData>(L"Hotel_ATM01.mdat")->Instantiate();
	ItemBox[1] = CResMgr::GetInst()->FindRes<CMeshData>(L"Hotel_BarbequeGrill01.mdat")->Instantiate();
	ItemBox[2] = CResMgr::GetInst()->FindRes<CMeshData>(L"Hotel_Dumpster01.mdat")->Instantiate();
	ItemBox[3] = CResMgr::GetInst()->FindRes<CMeshData>(L"Hotel_VendingMachine01.mdat")->Instantiate();

	ItemBox[0]->SetName(L"Hotel_ATM01");
	ItemBox[1]->SetName(L"Hotel_BarbequeGrill01");
	ItemBox[2]->SetName(L"Hotel_Dumpster01");
	ItemBox[3]->SetName(L"Hotel_VendingMachine01");

	for (auto& box : ItemBox)
	{
		AddComponents(box, _COLLIDER3D);
		box->Collider3D()->SetCollider3DType(COLLIDER3D_TYPE::CUBE);
		box->AddComponent(new ER_DataScript_ItemBox);
		box->MeshRender()->GetDynamicMaterial(0);
		box->LoadAllPrefabFromObjName();
		SpawnGameObject(box, L"ItemBox");
	}
}
void Create_SandyBeach()
{
	CGameObject* Base = CResMgr::GetInst()->FindRes<CMeshData>(L"SandyBeach_Base.mdat")->Instantiate();
	CGameObject* Building = CResMgr::GetInst()->FindRes<CMeshData>(L"SandyBeach_Building.mdat")->Instantiate();
	
	Base->SetName(L"SandyBeach_Base");
	Building->SetName(L"SandyBeach_Building");

	Base->LoadAllPrefabFromObjName();
	Building->LoadAllPrefabFromObjName();

	SpawnGameObject(Base, L"Base");
	SpawnGameObject(Building, L"Building");

	// ==================================================================================

	CGameObject* ItemBox[4] = {};

	ItemBox[0] = CResMgr::GetInst()->FindRes<CMeshData>(L"SandyBeach_Boat01.mdat")->Instantiate();
	ItemBox[1] = CResMgr::GetInst()->FindRes<CMeshData>(L"SandyBeach_Boat02.mdat")->Instantiate();
	ItemBox[2] = CResMgr::GetInst()->FindRes<CMeshData>(L"SandyBeach_Dumpster01.mdat")->Instantiate();
	ItemBox[3] = CResMgr::GetInst()->FindRes<CMeshData>(L"SandyBeach_Dumpster02.mdat")->Instantiate();

	ItemBox[0]->SetName(L"SandyBeach_Boat01");
	ItemBox[1]->SetName(L"SandyBeach_Boat02");
	ItemBox[2]->SetName(L"SandyBeach_Dumpster01");
	ItemBox[3]->SetName(L"SandyBeach_Dumpster02");

	for (auto& box : ItemBox)
	{
		AddComponents(box, _COLLIDER3D);
		box->Collider3D()->SetCollider3DType(COLLIDER3D_TYPE::CUBE);
		box->AddComponent(new ER_DataScript_ItemBox);
		box->MeshRender()->GetDynamicMaterial(0);
		box->LoadAllPrefabFromObjName();
		SpawnGameObject(box, L"ItemBox");
	}
}
void Create_School()
{
	CGameObject* Base = CResMgr::GetInst()->FindRes<CMeshData>(L"School_Base.mdat")->Instantiate();
	CGameObject* Building = CResMgr::GetInst()->FindRes<CMeshData>(L"School_Building.mdat")->Instantiate();
	CGameObject* Roof = CResMgr::GetInst()->FindRes<CMeshData>(L"School_Roof.mdat")->Instantiate();
	
	Base->SetName(L"School_Base");
	Building->SetName(L"School_Building");
	Roof->SetName(L"School_Roof");
	
	Base->LoadAllPrefabFromObjName();
	Building->LoadAllPrefabFromObjName();
	Roof->LoadAllPrefabFromObjName();

	SpawnGameObject(Base, L"Base");
	SpawnGameObject(Building, L"Building");
	SpawnGameObject(Roof, L"Roof");

	// Collider Set

	CGameObject* RoofEnableCollidr[3] = {};

	RoofEnableCollidr[0] = new CGameObject;
	RoofEnableCollidr[1] = new CGameObject;
	RoofEnableCollidr[2] = new CGameObject;

	RoofEnableCollidr[0]->SetName(L"School_Base_Collider3D01");
	RoofEnableCollidr[1]->SetName(L"School_Base_Collider3D02");
	RoofEnableCollidr[2]->SetName(L"School_Base_Collider3D03");

	for (auto &collider : RoofEnableCollidr)
	{
		AddComponents(collider, _TRANSFORM | _COLLIDER3D);
		collider->AddComponent(new ER_DataScript_LandMeshBase);
		collider->GetScript<ER_DataScript_LandMeshBase>()->SetRoof(Roof);
		collider->Collider3D()->SetCollider3DType(COLLIDER3D_TYPE::CUBE);
		collider->LoadAllPrefabFromObjName();
	}
	
	SpawnGameObject(RoofEnableCollidr[0], L"Base");
	SpawnGameObject(RoofEnableCollidr[1], L"Base");
	SpawnGameObject(RoofEnableCollidr[2], L"Base");

	// =================================================================================

	CGameObject* ItemBox[4] = {};

	ItemBox[0] = CResMgr::GetInst()->FindRes<CMeshData>(L"School_Dumpster01.mdat")->Instantiate();
	ItemBox[1] = CResMgr::GetInst()->FindRes<CMeshData>(L"School_Locker01.mdat")->Instantiate();
	ItemBox[2] = CResMgr::GetInst()->FindRes<CMeshData>(L"School_WreckCar01.mdat")->Instantiate();
	ItemBox[3] = CResMgr::GetInst()->FindRes<CMeshData>(L"School_WreckCar02.mdat")->Instantiate();

	ItemBox[0]->SetName(L"School_Dumpster01");
	ItemBox[1]->SetName(L"School_Locker01");
	ItemBox[2]->SetName(L"School_WreckCar01");
	ItemBox[3]->SetName(L"School_WreckCar02");

	for (auto& box : ItemBox)
	{
		AddComponents(box, _COLLIDER3D);
		box->Collider3D()->SetCollider3DType(COLLIDER3D_TYPE::CUBE);
		box->AddComponent(new ER_DataScript_ItemBox);
		box->MeshRender()->GetDynamicMaterial(0);
		box->LoadAllPrefabFromObjName();
		SpawnGameObject(box, L"ItemBox");
	}
}
void Create_Uptown()
{
	CGameObject* Base = CResMgr::GetInst()->FindRes<CMeshData>(L"Uptown_Base.mdat")->Instantiate();
	CGameObject* Building = CResMgr::GetInst()->FindRes<CMeshData>(L"Uptown_Building.mdat")->Instantiate();
	
	Base->SetName(L"Uptown_Base");
	Building->SetName(L"Uptown_Building");

	Base->LoadAllPrefabFromObjName();
	Building->LoadAllPrefabFromObjName();
	
	SpawnGameObject(Base, L"Base");
	SpawnGameObject(Building, L"Building");

	// ====================================================================================

	CGameObject* ItemBox[4] = {};

	ItemBox[0] = CResMgr::GetInst()->FindRes<CMeshData>(L"Uptown_BarbequeGrill01.mdat")->Instantiate();
	ItemBox[1] = CResMgr::GetInst()->FindRes<CMeshData>(L"Uptown_Sedan01.mdat")->Instantiate();
	ItemBox[2] = CResMgr::GetInst()->FindRes<CMeshData>(L"Uptown_SteelBox01.mdat")->Instantiate();
	ItemBox[3] = CResMgr::GetInst()->FindRes<CMeshData>(L"Uptown_WreckCar01.mdat")->Instantiate();

	ItemBox[0]->SetName(L"Uptown_BarbequeGrill01");
	ItemBox[1]->SetName(L"Uptown_Sedan01");
	ItemBox[2]->SetName(L"Uptown_SteelBox01");
	ItemBox[3]->SetName(L"Uptown_WreckCar01");

	for (auto& box : ItemBox)
	{
		AddComponents(box, _COLLIDER3D);
		box->Collider3D()->SetCollider3DType(COLLIDER3D_TYPE::CUBE);
		box->AddComponent(new ER_DataScript_ItemBox);
		box->MeshRender()->GetDynamicMaterial(0);
		box->LoadAllPrefabFromObjName();
		SpawnGameObject(box, L"ItemBox");
	}
}

// func
void SetRot_x(CGameObject* _obj, float _x)
{
	CTransform* transform = _obj->Transform();
	Vec3 rot = transform->GetRelativeRot();
	rot.x = _x;
	transform->SetRelativeRot(rot);
}
void SetRot_y(CGameObject* _obj, float _y)
{
	CTransform* transform = _obj->Transform();
	Vec3 rot = transform->GetRelativeRot();
	rot.y = _y;
	transform->SetRelativeRot(rot);
}
void SetRot_z(CGameObject* _obj, float _z)
{
	CTransform* transform = _obj->Transform();
	Vec3 rot = transform->GetRelativeRot();
	rot.z = _z;
	transform->SetRelativeRot(rot);
}

void XMConv_Rot_X(CGameObject* _obj, float _x)
{
	Vec3 vRot = _obj->Transform()->GetRelativeRot();
	vRot.x = XMConvertToRadians(_x);
	_obj->Transform()->SetRelativeRot(vRot);
}
void XMConv_Rot_Y(CGameObject* _obj, float _y)
{
	Vec3 vRot = _obj->Transform()->GetRelativeRot();
	vRot.y = XMConvertToRadians(_y);
	_obj->Transform()->SetRelativeRot(vRot);
}
void XMConv_Rot_Z(CGameObject* _obj, float _z)
{
	Vec3 vRot = _obj->Transform()->GetRelativeRot();
	vRot.z = XMConvertToRadians(_z);
	_obj->Transform()->SetRelativeRot(vRot);
}
void XMConv_Rot_XYZ(CGameObject* _obj, float _x, float _y, float _z)
{
	Vec3 vRot = _obj->Transform()->GetRelativeRot();
	vRot.x = XMConvertToRadians(_x);
	vRot.y = XMConvertToRadians(_y);
	vRot.z = XMConvertToRadians(_z);
	_obj->Transform()->SetRelativeRot(vRot);
}
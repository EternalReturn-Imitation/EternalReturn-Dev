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

// [Graphic]
#include <Engine\CSetColorShader.h>


// [Script&GameSystem]
#include <Script\CEditCamControlScript.h>
#include <Script\CFollowMainCamScript.h>
#include <Script\ER_CamControllerScript.h>
#include <Script\ER_DataScript_ItemBox.h>
#include <Script\ER_DataScript_LandMeshBase.h>

#include <Script\ER_CharacterMgr.h>

// [Editor]
#include "CEditorObjMgr.h"
#include "CLevelSaveLoad.h"
#include "LevelMgr.h"

#pragma endregion

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

	// TestObject();
}

void TestObject()
{
	// Text Obj
	CGameObject* testTextObj = new CGameObject;
	AddComponents(testTextObj, _TRANSFORM | _MESHRENDER | _TEXT);
	testTextObj->SetName(L"TextUI");
	
	// 텍스트 출력 필수요소 : _TRANSFORM | _MESHRENDER | _TEXT
	// Std2DUIMtrl 사용
	testTextObj->Transform()->SetRelativeScale(Vec3(100.f, 100.f, 0.f));
	testTextObj->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	testTextObj->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std2DUIMtrl"), 0);
	
	// 텍스쳐없어도되지만 텍스쳐지정 안하면 마젠타색상출력돼서 쉐이더코드처리필요
	testTextObj->MeshRender()->GetMaterial(0)->SetTexParam(TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"Ico_ItemGradebg_01.png"));
	

	// 폰트 넥슨Lv2고딕 과, FW1_CENTER | FW1_VCENTER Flags는 기본값으로설정해놓음.
	// 예시 : 폰트 패밀리이름(파일이름아님), OffsetPos, FontSize, RGBA값, 폰트출력 Flag.
	testTextObj->Text()->TextInit(L"넥슨Lv2고딕", Vec2(0.f, 0.f), 50.f, FONT_RGBA(255, 255, 255, 255), FW1_CENTER | FW1_VCENTER);
	testTextObj->Text()->InputString(L"테스트");
	
	SpawnGameObject(testTextObj, Vec3(0.f, 0.f, 0.f), L"UI");
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

	_Level->GetLayer(28)->SetName(L"InGameUI");
	_Level->GetLayer(29)->SetName(L"MapCollider");
	_Level->GetLayer(30)->SetName(L"Camera");
	_Level->GetLayer(31)->SetName(L"UI");

	// Collision Set
	CCollisionMgr::GetInst()->LayerCheck(L"Monster", L"Monster");
	CCollisionMgr::GetInst()->LayerCheck(L"Player", L"Base");
	CCollisionMgr::GetInst()->LayerCheck(L"ItemBox", L"MapCollider");

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

	SpawnGameObject(UICamera, Vec3(0.f, 0.f, 0.f), L"Camera");
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
void CreateTestPlayer()
{
	ER_CharacterMgr::GetInst()->SpawnCharacter_Player(L"Yuki", Vec3(-69.32188f, 0.0f, 37.60328f));
}
void CreateTestEnemy()
{
	ER_CharacterMgr::GetInst()->SpawnCharacter_Enemy(L"Rio", Vec3(-74.98286f, 0.0f, 37.60328f));
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

	Base->Transform()->SetRelativeScale(LANDSCALE, LANDSCALE, LANDSCALE);
	Building->Transform()->SetRelativeScale(LANDSCALE, LANDSCALE, LANDSCALE);
	Roof->Transform()->SetRelativeScale(LANDSCALE, LANDSCALE, LANDSCALE);

	SetRot_x(Building, REVERSE90RAD);
	
	SpawnGameObject(Base, Vec3(-187.f, -3.99f, 26.f), L"Base");
	SpawnGameObject(Building, Vec3(0.2f, -0.1f, 0.5f), L"Building");
	SpawnGameObject(Roof, Vec3(-133.18f, 5.660f, 43.730f), L"Roof");


	// [ Collider ]
	CGameObject* RoofEnable = new CGameObject;
	RoofEnable->SetName(L"Archery_Base_Collider3D");
	AddComponents(RoofEnable, _TRANSFORM | _COLLIDER3D);

	RoofEnable->Collider3D()->SetOffsetScale(Vec3(10.f, 1.f, 20.f));
	RoofEnable->Collider3D()->SetOffsetPos(Vec3(0.f, 1.f, 0.f));
	RoofEnable->Collider3D()->SetCollider3DType(COLLIDER3D_TYPE::CUBE);

	RoofEnable->AddComponent(new ER_DataScript_LandMeshBase);
	RoofEnable->GetScript<ER_DataScript_LandMeshBase>()->SetRoof(Roof);

	SpawnGameObject(RoofEnable, Vec3(-132.f, 1.76422f, 53.28091f), L"Base");

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
		
		box->Transform()->SetRelativeScale(0.01f, 0.01f, 0.01f);
		
		box->MeshRender()->GetDynamicMaterial(0);
	}
	
	XMConv_Rot_XYZ(ItemBox[0], 90.f, -68.f, -180.f);
	XMConv_Rot_X(ItemBox[1], -90.f);
	XMConv_Rot_Y(ItemBox[2], -36.f);
	
	ItemBox[0]->Collider3D()->SetOffsetScale(Vec3(150.f, 150.f, 300.f));
	ItemBox[1]->Collider3D()->SetOffsetScale(Vec3(100.f, 200.f, 300.f));
	ItemBox[2]->Collider3D()->SetOffsetScale(Vec3(100.f, 200.f, 100.f));

	ItemBox[0]->Collider3D()->SetOffsetPos(Vec3(0.f, 1.f, 100.f));
	ItemBox[1]->Collider3D()->SetOffsetPos(Vec3(0.f, 101.f, 100.f));
	ItemBox[2]->Collider3D()->SetOffsetPos(Vec3(0.f, 101.f, 0.f));

	SpawnGameObject(ItemBox[0], Vec3(-134.89401f, -0.228f, 91.17091f), L"ItemBox");
	SpawnGameObject(ItemBox[1], Vec3(-132.93658f, 1.4689f, 46.70636f), L"ItemBox");
	SpawnGameObject(ItemBox[2], Vec3(-121.580f, 0.82966f, 35.689f), L"ItemBox");
}
void Create_Forest()
{
	CGameObject* Base = CResMgr::GetInst()->FindRes<CMeshData>(L"Forest_Base.mdat")->Instantiate();
	CGameObject* Building = CResMgr::GetInst()->FindRes<CMeshData>(L"Forest_Building.mdat")->Instantiate();

	Base->SetName(L"Forest_Base");
	Building->SetName(L"Forest_Building");
	
	Base->Transform()->SetRelativeScale(LANDSCALE, LANDSCALE, LANDSCALE);
	Building->Transform()->SetRelativeScale(0.01007f, 0.01067f, 0.01067f);

	SpawnGameObject(Base, Vec3(-53.680f, -7.98f, -20.202f), L"Base");
	SpawnGameObject(Building, Vec3(-11.92211f, 0.227f, -20.54554f), L"Building");

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

		box->Transform()->SetRelativeScale(0.01f, 0.01f, 0.01f);
		box->MeshRender()->GetDynamicMaterial(0);
	}

	XMConv_Rot_XYZ(ItemBox[3], -180.f, 65.f, -180.f);

	ItemBox[0]->Collider3D()->SetOffsetScale(Vec3(120.f, 120.f, 120.f));
	ItemBox[1]->Collider3D()->SetOffsetScale(Vec3(100.f, 120.f, 100.f));
	ItemBox[2]->Collider3D()->SetOffsetScale(Vec3(400.f, 100.f, 100.f));
	ItemBox[3]->Collider3D()->SetOffsetScale(Vec3(400.f, 100.f, 100.f));
	
	ItemBox[0]->Collider3D()->SetOffsetPos(Vec3(0.f, 51.f, 0.f));
	ItemBox[1]->Collider3D()->SetOffsetPos(Vec3(0.f, 51.f, 0.f));
	ItemBox[2]->Collider3D()->SetOffsetPos(Vec3(0.f, 1.f, 0.f));
	ItemBox[3]->Collider3D()->SetOffsetPos(Vec3(0.f, 1.f, 0.f));

	ItemBox[2]->Transform()->SetRelativeRot(Vec3(0.f, Deg2Rad(-10), 0.f));
	ItemBox[3]->Transform()->SetRelativeRot(Deg2Rad(-11), Deg2Rad(88), Deg2Rad(-11));
	
	SpawnGameObject(ItemBox[0], Vec3(19.80982f, 0.11900f, 16.42808f), L"ItemBox");
	SpawnGameObject(ItemBox[1], Vec3(-26.02692f, 0.27824f, 18.76737f), L"ItemBox");
	SpawnGameObject(ItemBox[2], Vec3(14.36587f, 1.59560f, -3.72484f), L"ItemBox");
	SpawnGameObject(ItemBox[3], Vec3(-26.97585f, 0.05435f, -3.30284f), L"ItemBox");
}
void Create_Hotel()
{
	CGameObject* Base = CResMgr::GetInst()->FindRes<CMeshData>(L"Hotel_Base.mdat")->Instantiate();
	CGameObject* Building = CResMgr::GetInst()->FindRes<CMeshData>(L"Hotel_Building.mdat")->Instantiate();
	CGameObject* Roof = CResMgr::GetInst()->FindRes<CMeshData>(L"Hotel_Roof.mdat")->Instantiate();
	
	Base->SetName(L"Hotel_Base");
	Building->SetName(L"Hotel_Building");
	Roof->SetName(L"Hotel_Roof");
	
	Base->Transform()->SetRelativeScale(LANDSCALE, LANDSCALE, LANDSCALE);
	Building->Transform()->SetRelativeScale(LANDSCALE, LANDSCALE, LANDSCALE);
	Roof->Transform()->SetRelativeScale(LANDSCALE, LANDSCALE, LANDSCALE);
	
	SpawnGameObject(Base, Vec3(-96.4f, -1.76f, 176.4f), L"Base");
	SpawnGameObject(Building, Vec3(-141.f, -2.6f, -21.35f), L"Building");
	SpawnGameObject(Roof, Vec3(-101.72f, 0.910f, -18.075f), L"Roof");

	// [ Collider ]
	CGameObject* RoofEnable = new CGameObject;
	RoofEnable->SetName(L"Hotel_Base_Collider3D");
	AddComponents(RoofEnable, _TRANSFORM | _COLLIDER3D);

	RoofEnable->Collider3D()->SetOffsetScale(Vec3(15.f, 1.f, 30.f));
	RoofEnable->Collider3D()->SetOffsetPos(Vec3(0.f, 1.f, 0.f));
	RoofEnable->Collider3D()->SetCollider3DType(COLLIDER3D_TYPE::CUBE);
	
	RoofEnable->AddComponent(new ER_DataScript_LandMeshBase);
	RoofEnable->GetScript<ER_DataScript_LandMeshBase>()->SetRoof(Roof);
	
	SpawnGameObject(RoofEnable, Vec3(-109.07837f, 1.f, -3.79784f), L"Base");

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

		box->Transform()->SetRelativeScale(0.01f, 0.01f, 0.01f);
		box->MeshRender()->GetDynamicMaterial(0);
	}
	
	XMConv_Rot_X(ItemBox[0], -180.f);
	XMConv_Rot_Z(ItemBox[0], -180.f);

	XMConv_Rot_Y(ItemBox[1], -90.f);

	XMConv_Rot_X(ItemBox[2], -90.f);
	
	ItemBox[0]->Collider3D()->SetOffsetScale(Vec3(100.f, 200.f, 100.f));
	ItemBox[1]->Collider3D()->SetOffsetScale(Vec3(120.f, 150.f, 150.f));
	ItemBox[2]->Collider3D()->SetOffsetScale(Vec3(100.f, 220.f, 170.f));
	ItemBox[3]->Collider3D()->SetOffsetScale(Vec3(120.f, 220.f, 120.f));
	
	ItemBox[0]->Collider3D()->SetOffsetPos(Vec3(0.f, 101.f, 0.f));
	ItemBox[1]->Collider3D()->SetOffsetPos(Vec3(0.f, 71.f, 0.f));
	ItemBox[2]->Collider3D()->SetOffsetPos(Vec3(0.f, 0.f, 90.f));
	ItemBox[3]->Collider3D()->SetOffsetPos(Vec3(0.f, 140.f, 0.f));
	
	SpawnGameObject(ItemBox[0], Vec3(-112.772f, 1.14831f, -16.68324f), L"ItemBox");
	SpawnGameObject(ItemBox[1], Vec3(-103.886f, 0.374f, -32.562f), L"ItemBox");
	SpawnGameObject(ItemBox[2], Vec3(-63.052f, 0.332f, -19.634f), L"ItemBox");
	SpawnGameObject(ItemBox[3], Vec3(-105.717f, 0.322f, 8.205f), L"ItemBox");
}
void Create_SandyBeach()
{
	CGameObject* Base = CResMgr::GetInst()->FindRes<CMeshData>(L"SandyBeach_Base.mdat")->Instantiate();
	CGameObject* Building = CResMgr::GetInst()->FindRes<CMeshData>(L"SandyBeach_Building.mdat")->Instantiate();
	
	Base->SetName(L"SandyBeach_Base");
	Building->SetName(L"SandyBeach_Building");

	Base->Transform()->SetRelativeScale(LANDSCALE, LANDSCALE, LANDSCALE);
	Building->Transform()->SetRelativeScale(LANDSCALE, LANDSCALE, LANDSCALE);

	SetRot_x(Building, REVERSE90RAD);

	SpawnGameObject(Base, Vec3(-79.400f, -10.500f, -75.300f), L"Base");
	SpawnGameObject(Building, Vec3(0.911f, -0.7f, 12.500f), L"Building");

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

		box->Transform()->SetRelativeScale(0.01f, 0.01f, 0.01f);
		box->MeshRender()->GetDynamicMaterial(0);
	}

	XMConv_Rot_XYZ(ItemBox[0], 90.f, 45.f, -180.f);

	XMConv_Rot_X(ItemBox[1], -90.f);
	XMConv_Rot_Y(ItemBox[1], 22.f);

	XMConv_Rot_X(ItemBox[2], -91.f);
	XMConv_Rot_Y(ItemBox[2], -66.f);

	XMConv_Rot_XYZ(ItemBox[3], 90.f, -90.f, -180.f);

	ItemBox[0]->Collider3D()->SetOffsetScale(Vec3(200.f, 700.f, 120.f));
	ItemBox[1]->Collider3D()->SetOffsetScale(Vec3(100.f, 220.f, 170.f));
	ItemBox[2]->Collider3D()->SetOffsetScale(Vec3(100.f, 220.f, 170.f));
	ItemBox[3]->Collider3D()->SetOffsetScale(Vec3(200.f, 700.f, 120.f));

	ItemBox[0]->Collider3D()->SetOffsetPos(Vec3(0.f, 0.f, 50.f));
	ItemBox[1]->Collider3D()->SetOffsetPos(Vec3(0.f, 0.f, 50.f));
	ItemBox[2]->Collider3D()->SetOffsetPos(Vec3(0.f, 0.f, 90.f));
	ItemBox[3]->Collider3D()->SetOffsetPos(Vec3(0.f, 0.f, 90.f));

	SpawnGameObject(ItemBox[0], Vec3(-32.593f, -2.816f, -68.192f), L"ItemBox");
	SpawnGameObject(ItemBox[1], Vec3(-64.518f, -3.001f, -72.063f), L"ItemBox");
	SpawnGameObject(ItemBox[2], Vec3(-42.661f, -1.764f, -48.906f), L"ItemBox");
	SpawnGameObject(ItemBox[3], Vec3(-64.094f, -1.371f, -42.020f), L"ItemBox");
}
void Create_School()
{
	CGameObject* Base = CResMgr::GetInst()->FindRes<CMeshData>(L"School_Base.mdat")->Instantiate();
	CGameObject* Building = CResMgr::GetInst()->FindRes<CMeshData>(L"School_Building.mdat")->Instantiate();
	CGameObject* Roof = CResMgr::GetInst()->FindRes<CMeshData>(L"School_Roof.mdat")->Instantiate();
	
	Base->SetName(L"School_Base");
	Building->SetName(L"School_Building");
	Roof->SetName(L"School_Roof");
	
	Base->Transform()->SetRelativeScale(LANDSCALE, LANDSCALE, LANDSCALE);
	Building->Transform()->SetRelativeScale(LANDSCALE, LANDSCALE, LANDSCALE);
	Roof->Transform()->SetRelativeScale(LANDSCALE, LANDSCALE, LANDSCALE);
	
	SetRot_y(Roof, REVERSE90RAD);

	SpawnGameObject(Base, Vec3(-86.656f, -3.100f, 67.850f), L"Base");
	SpawnGameObject(Building, Vec3(-74.800f, -0.380f, 31.400f), L"Building");
	SpawnGameObject(Roof, Vec3(-87.150f, -0.280f, 76.150f), L"Roof");

	// Collider Set

	CGameObject* RoofEnableCollidr[3] = {};

	for (auto &collider : RoofEnableCollidr)
	{
		collider = new CGameObject;
		AddComponents(collider, _TRANSFORM | _COLLIDER3D);
		collider->Collider3D()->SetOffsetPos(Vec3(0.f, 1.f, 0.f));
		collider->AddComponent(new ER_DataScript_LandMeshBase);
		collider->GetScript<ER_DataScript_LandMeshBase>()->SetRoof(Roof);
		collider->Collider3D()->SetCollider3DType(COLLIDER3D_TYPE::CUBE);
	}

	RoofEnableCollidr[0]->SetName(L"School_Base_Collider3D01");
	RoofEnableCollidr[1]->SetName(L"School_Base_Collider3D02");
	RoofEnableCollidr[2]->SetName(L"School_Base_Collider3D03");
	
	RoofEnableCollidr[0]->Collider3D()->SetOffsetScale(Vec3(28.f, 1.f, 24.f));
	RoofEnableCollidr[1]->Collider3D()->SetOffsetScale(Vec3(17.f, 1.f, 55.f));
	RoofEnableCollidr[2]->Collider3D()->SetOffsetScale(Vec3(6.f, 1.f, 7.f));
	

	SpawnGameObject(RoofEnableCollidr[0], Vec3(-101.36192f, 1.f, 88.06251f), L"Base");
	SpawnGameObject(RoofEnableCollidr[1], Vec3(-69.12391f, 1.f, 68.64030f), L"Base");
	SpawnGameObject(RoofEnableCollidr[2], Vec3(-59.57447f, 1.f, 65.01f), L"Base");

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

	for (auto &box : ItemBox)
	{
		AddComponents(box, _COLLIDER3D);
		box->Collider3D()->SetCollider3DType(COLLIDER3D_TYPE::CUBE);
		box->AddComponent(new ER_DataScript_ItemBox);

		box->Transform()->SetRelativeScale(0.01f, 0.01f, 0.01f);
		box->MeshRender()->GetDynamicMaterial(0);
	}
	XMConv_Rot_X(ItemBox[0], -90.f);

	XMConv_Rot_Y(ItemBox[1], -90.f);
	XMConv_Rot_Z(ItemBox[1], 90.f);

	XMConv_Rot_XYZ(ItemBox[2], -180.f, -90.f, -180.f);
	
	ItemBox[0]->Collider3D()->SetOffsetScale(Vec3(100.f, 220.f, 170.f));
	ItemBox[1]->Collider3D()->SetOffsetScale(Vec3(50.f, 200.f, 250.f));
	ItemBox[2]->Collider3D()->SetOffsetScale(Vec3(200.f, 100.f, 550.f));
	ItemBox[3]->Collider3D()->SetOffsetScale(Vec3(200.f, 100.f, 550.f));

	ItemBox[0]->Collider3D()->SetOffsetPos(Vec3(0.f, 0.f, 90.f));
	ItemBox[1]->Collider3D()->SetOffsetPos(Vec3(30.f, 90.f, 120.f));
	ItemBox[2]->Collider3D()->SetOffsetPos(Vec3(0.f, 70.f, 0.f));
	ItemBox[3]->Collider3D()->SetOffsetPos(Vec3(0.f, 70.f, 0.f));
	
	SpawnGameObject(ItemBox[0], Vec3(-105.100f, -0.160f, 53.730f), L"ItemBox");
	SpawnGameObject(ItemBox[1], Vec3(-65.92036f, 3.28625f, 67.42262f), L"ItemBox");
	SpawnGameObject(ItemBox[2], Vec3(-65.73459f, 0.45918f, 101.998f), L"ItemBox");
	SpawnGameObject(ItemBox[3], Vec3(-84.585f, -0.16467f, 47.429f), L"ItemBox");
}
void Create_Uptown()
{
	CGameObject* Base = CResMgr::GetInst()->FindRes<CMeshData>(L"Uptown_Base.mdat")->Instantiate();
	CGameObject* Building = CResMgr::GetInst()->FindRes<CMeshData>(L"Uptown_Building.mdat")->Instantiate();
	
	Base->SetName(L"Uptown_Base");
	Building->SetName(L"Uptown_Building");

	Base->Transform()->SetRelativeScale(LANDSCALE, LANDSCALE, LANDSCALE);
	Building->Transform()->SetRelativeScale(LANDSCALE, LANDSCALE, LANDSCALE);
	
	SpawnGameObject(Base, Vec3(23.820f, -5.33f, -53.350f), L"Base");
	SpawnGameObject(Building, Vec3(9.f, -4.600f, -70.290f), L"Building");

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

		box->Transform()->SetRelativeScale(0.01f, 0.01f, 0.01f);
		box->MeshRender()->GetDynamicMaterial(0);
	}

	XMConv_Rot_XYZ(ItemBox[1], -180.f, 9.f, -180.f);
	XMConv_Rot_XYZ(ItemBox[3], -180.f, -90.f, -180.f);

	ItemBox[0]->Collider3D()->SetOffsetScale(Vec3(120.f, 150.f, 150.f));
	ItemBox[1]->Collider3D()->SetOffsetScale(Vec3(200.f, 100.f, 550.f));
	ItemBox[2]->Collider3D()->SetOffsetScale(Vec3(100.f, 200.f, 100.f));
	ItemBox[3]->Collider3D()->SetOffsetScale(Vec3(200.f, 100.f, 550.f));

	ItemBox[0]->Collider3D()->SetOffsetPos(Vec3(0.f, 70.f, 0.f));
	ItemBox[1]->Collider3D()->SetOffsetPos(Vec3(0.f, 70.f, 0.f));
	ItemBox[2]->Collider3D()->SetOffsetPos(Vec3(0.f, 100.f, 0.f));
	ItemBox[3]->Collider3D()->SetOffsetPos(Vec3(0.f, 70.f, 0.f));

	SpawnGameObject(ItemBox[0], Vec3(-13.929f, -0.46114f, -50.756f), L"ItemBox");
	SpawnGameObject(ItemBox[1], Vec3(-6.39721f, -0.40347f, -65.22822f), L"ItemBox");
	SpawnGameObject(ItemBox[2], Vec3(31.074f, -0.57047f, -41.496f), L"ItemBox");
	SpawnGameObject(ItemBox[3], Vec3(27.374f, -0.05370f, -76.997f), L"ItemBox");
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
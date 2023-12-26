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

#include "CLevelSaveLoad.h"


#include <Engine/CSetColorShader.h>


void CreateTestLevel()
{
	CLevel* pCurLevel = CLevelMgr::GetInst()->GetCurLevel();
	pCurLevel->ChangeState(LEVEL_STATE::STOP);

	// Layer 이름설정
	pCurLevel->GetLayer(0)->SetName(L"Default");
	pCurLevel->GetLayer(1)->SetName(L"Tile");
	pCurLevel->GetLayer(2)->SetName(L"Player");
	pCurLevel->GetLayer(3)->SetName(L"Monster");
	pCurLevel->GetLayer(4)->SetName(L"PlayerProjectile");
	pCurLevel->GetLayer(5)->SetName(L"MonsterProjectile");
	pCurLevel->GetLayer(31)->SetName(L"ViewPort UI");


	// Main Camera Object 생성
	CGameObject* pMainCam = new CGameObject;
	pMainCam->SetName(L"MainCamera");

	pMainCam->AddComponent(new CTransform);
	pMainCam->AddComponent(new CCamera);

	pMainCam->Camera()->SetProjType(PROJ_TYPE::ORTHOGRAPHIC);
	pMainCam->Camera()->SetCameraIndex(0);		// MainCamera 로 설정
	pMainCam->Camera()->SetLayerMaskAll(true);	// 모든 레이어 체크
	pMainCam->Camera()->SetLayerMask(31, false);// UI Layer 는 렌더링하지 않는다.

	SpawnGameObject(pMainCam, Vec3(0.f, 0.f, 0.f), 0);

	// UI cameara
	CGameObject* pUICam = new CGameObject;
	pUICam->SetName(L"UICamera");

	pUICam->AddComponent(new CTransform);
	pUICam->AddComponent(new CCamera);

	pUICam->Camera()->SetProjType(PROJ_TYPE::ORTHOGRAPHIC);
	pUICam->Camera()->SetCameraIndex(1);		// Sub 카메라로 지정
	pUICam->Camera()->SetLayerMask(31, true);	// 31번 레이어만 체크

	SpawnGameObject(pUICam, Vec3(0.f, 0.f, 0.f), 0);


	// SkyBox 추가
	CGameObject* pSkyBox = new CGameObject;
	pSkyBox->SetName(L"SkyBox");

	pSkyBox->AddComponent(new CTransform);
	pSkyBox->AddComponent(new CSkyBox);

	pSkyBox->Transform()->SetRelativeScale(Vec3(100.f, 100.f, 100));
	pSkyBox->SkyBox()->SetSkyBoxType(SKYBOX_TYPE::CUBE);
	pSkyBox->SkyBox()->SetSkyBoxTexture(CResMgr::GetInst()->FindRes<CTexture>(L"texture\\skybox\\SkyWater.dds"));

	SpawnGameObject(pSkyBox, Vec3(0.f, 0.f, 0.f), 0);

	// 광원 추가
	CGameObject* pLightObj = new CGameObject;
	pLightObj->SetName(L"Directional Light");

	pLightObj->AddComponent(new CTransform);
	pLightObj->AddComponent(new CLight3D);

	pLightObj->Transform()->SetRelativeRot(Vec3(XM_PI / 4.f, XM_PI / 4.f, 0.f));
	pLightObj->Light3D()->SetLightType(LIGHT_TYPE::DIRECTIONAL);
	pLightObj->Light3D()->SetLightColor(Vec3(0.4f, 0.4f, 0.4f));	
	pLightObj->Light3D()->SetLightAmbient(Vec3(0.15f, 0.15f, 0.15f));
	
	SpawnGameObject(pLightObj, Vec3(-500.f, -250.f, 0.f), 0);


	// 오브젝트 생성
	CGameObject* pParent = new CGameObject;
	pParent->SetName(L"Player");
	pParent->AddComponent(new CTransform);
	pParent->AddComponent(new CMeshRender);
	pParent->AddComponent(new CPlayerScript);
	pParent->AddComponent(new CBehaviorTree);


	CBehaviorTree* pBT = pParent->BehaviorTree();
	Root_Node* MainRoot = pBT->SetRootNode((Root_Node*)CBehaviorTreeMgr::CreateBTNode(BT_ROOT, 0));
	MainRoot->SetNodeName(L"RootNode");

	BB* MainBB = MainRoot->GetBlackBoard();
	MainBB->AddBBData("PlayerObj", pParent);
	int Testint = 158;
	MainBB->AddBBData("Ine", Testint);
	float Testfloat = 0.158f;
	MainBB->AddBBData("Inefloat", Testfloat);
	string Teststring = "Ine";
	MainBB->AddBBData("Inestring", Teststring);
	wstring Testwstring = L"Ine158";
	MainBB->AddBBData("IneWstring", Testwstring);

	string TestIne;
	MainBB->FindBBData("Inestring", TestIne);

	BTNode* N_1 = MainRoot->AddChild(CBehaviorTreeMgr::CreateBTNode(BT_COMPOSITE, Composite_Node::CompositeNodeFlag_SEQUENCE));
	BTNode* N_1_1 = N_1->AddChild(CBehaviorTreeMgr::CreateBTNode(BT_COMPOSITE, Composite_Node::CompositeNodeFlag_SEQUENCE));
	BTNode* N_1_2 = N_1->AddChild(CBehaviorTreeMgr::CreateBTNode(BT_COMPOSITE, Composite_Node::CompositeNodeFlag_SELECTOR));

	BTNode* N_1_1_1 = N_1_1->AddChild(CBehaviorTreeMgr::CreateBTNode(BT_COMPOSITE, Composite_Node::CompositeNodeFlag_SEQUENCE));
	BTNode* N_1_1_2 = N_1_1->AddChild(CBehaviorTreeMgr::CreateBTNode(BT_COMPOSITE, Composite_Node::CompositeNodeFlag_SELECTOR));
	BTNode* N_1_1_3 = N_1_1->AddChild(CBehaviorTreeMgr::CreateBTNode(BT_TASK, Task_Node::TaskNodeFlag_WAIT));

	BTNode* N_1_2_1 = N_1_2->AddChild(CBehaviorTreeMgr::CreateBTNode(BT_TASK, Task_Node::TaskNodeFlag_WAIT));
	N_1_2_1->SetNodeName(L"N_1_2_1WAIT");
	BTNode* N_1_2_2 = N_1_2->AddChild(CBehaviorTreeMgr::CreateBTNode(BT_TASK, Task_Node::TaskNodeFlag_PLAY_SOUND));
	N_1_2_2->SetNodeName(L"N_1_2_2PLAY_SOUND");
	BTNode* N_1_2_3 = N_1_2->AddChild(CBehaviorTreeMgr::CreateBTNode(BT_COMPOSITE, Composite_Node::CompositeNodeFlag_SEQUENCE));
	N_1_2_3->SetNodeName(L"N_1_2_3SEQ");
	BTNode* N_1_2_4 = N_1_2->AddChild(CBehaviorTreeMgr::CreateBTNode(BT_COMPOSITE, Composite_Node::CompositeNodeFlag_SELECTOR));
	N_1_2_4->SetNodeName(L"N_1_2_4SEL");
	BTNode* N_1_2_5 = N_1_2->AddChild(CBehaviorTreeMgr::CreateBTNode(BT_COMPOSITE, Composite_Node::CompositeNodeFlag_SEQUENCE));
	N_1_2_5->SetNodeName(L"N_1_2_5SEQ");

	BTNode* N_1_1_2_1 = N_1_1_2->AddChild(CBehaviorTreeMgr::CreateBTNode(BT_TASK, Task_Node::TaskNodeFlag_WAIT));
	BTNode* N_1_2_3_1 = N_1_2_3->AddChild(CBehaviorTreeMgr::CreateBTNode(BT_TASK, Task_Node::TaskNodeFlag_WAIT));
	SpawnGameObject(pParent, Vec3(0.f, 0.f, 0.f), L"Player");
	

	CGameObject* pObject = new CGameObject;
	pObject->SetName(L"Sphere");
	pObject->AddComponent(new CTransform);
	pObject->AddComponent(new CMeshRender);
	pObject->Transform()->SetRelativeScale(Vec3(1000.f, 1000.f, 1000.f));
	pObject->Transform()->SetRelativeRot(Vec3(0.f, 0.f, 0.f));

	pObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"SphereMesh"));
	pObject->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std3D_DeferredMtrl"));
	pObject->MeshRender()->GetMaterial()->SetTexParam(TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"texture\\tile\\TILE_01.tga"));
	pObject->MeshRender()->GetMaterial()->SetTexParam(TEX_1, CResMgr::GetInst()->FindRes<CTexture>(L"texture\\tile\\TILE_01_N.tga"));
	SpawnGameObject(pObject, Vec3(0.f, 0.f, 0.f), L"Default");

	pObject = new CGameObject;
	pObject->SetName(L"Plane");
	pObject->AddComponent(new CTransform);
	pObject->AddComponent(new CMeshRender);

	pObject->Transform()->SetRelativeScale(Vec3(2000.f, 2000.f, 2000.f));
	pObject->Transform()->SetRelativeRot(Vec3(XM_PI / 2.f, 0.f, 0.f));

	pObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	pObject->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std3D_DeferredMtrl"));
	pObject->MeshRender()->GetMaterial()->SetTexParam(TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"texture\\tile\\TILE_01.tga"));
	pObject->MeshRender()->GetMaterial()->SetTexParam(TEX_1, CResMgr::GetInst()->FindRes<CTexture>(L"texture\\tile\\TILE_01_N.tga"));

	SpawnGameObject(pObject, Vec3(0.f, -1000.f, 0.f), L"Default");

	pObject = new CGameObject;
	pObject->SetName(L"Decal");
	pObject->AddComponent(new CTransform);
	pObject->AddComponent(new CDecal);

	pObject->Transform()->SetRelativeScale(Vec3(200.f, 200.f, 200.f));
	pObject->Transform()->SetRelativeRot(Vec3(0.f, 0.f, 0.f));

	pObject->Decal()->SetDeferredDecal(true);
	pObject->Decal()->ActivateEmissive(true);

	SpawnGameObject(pObject, Vec3(0.f, 0.f, 500.f), L"Default");


	
	// 충돌 시킬 레이어 짝 지정
	CCollisionMgr::GetInst()->LayerCheck(L"Player", L"Monster");	
}

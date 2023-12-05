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
	//return;

	// 컴퓨트 쉐이더 테스트
	Ptr<CTexture> pTestTexture = 
		CResMgr::GetInst()->CreateTexture(L"ComputeTestTex"
			, 200, 200, DXGI_FORMAT_R8G8B8A8_UNORM
			, D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS
			, D3D11_USAGE_DEFAULT );

	Ptr<CSetColorShader> pCS = (CSetColorShader*)CResMgr::GetInst()->FindRes<CComputeShader>(L"SetColorCS").Get();
	pCS->SetTargetTexture(pTestTexture);
	pCS->SetColor(Vec3(1.f, 0.f, 1.f));
	pCS->Execute();


	//Ptr<CSound> pSound = CResMgr::GetInst()->FindRes<CSound>(L"sound\\BGM_Stage1.wav");
	//pSound->Play(1, 0.5f, false);

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


	// 광원 추가
	CGameObject* pLightObj = new CGameObject;
	pLightObj->SetName(L"Point Light");

	pLightObj->AddComponent(new CTransform);
	pLightObj->AddComponent(new CLight2D);

	pLightObj->Transform()->SetRelativePos(Vec3(0.f, 0.f, 0.f));
	pLightObj->Transform()->SetRelativeRot(Vec3(0.f, 0.f, XM_PI / 2.f));

	pLightObj->Light2D()->SetLightType(LIGHT_TYPE::POINT);
	pLightObj->Light2D()->SetLightDiffuse(Vec3(1.f, 1.f, 1.f));
	pLightObj->Light2D()->SetRadius(500.f);

	SpawnGameObject(pLightObj, Vec3(0.f, 0.f, 0.f), 0);


	// 오브젝트 생성
	CGameObject* pParent = new CGameObject;
	pParent->SetName(L"Player");
	pParent->AddComponent(new CTransform);
	pParent->AddComponent(new CMeshRender);
	pParent->AddComponent(new CCollider2D);
	pParent->AddComponent(new CAnimator2D);
	pParent->AddComponent(new CPlayerScript);

	pParent->Transform()->SetRelativeScale(Vec3(200.f, 200.f, 1.f));

	pParent->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	pParent->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std2DAnimLightMtrl"));

	pParent->Collider2D()->SetAbsolute(true);
	pParent->Collider2D()->SetOffsetScale(Vec2(150.f, 150.f));

	Ptr<CTexture> pAnimAtlas = CResMgr::GetInst()->FindRes<CTexture>(L"texture\\link.png");
	pParent->Animator2D()->CreateAnimation(L"WalkDown", pAnimAtlas, Vec2(0.f, 520.f), Vec2(120.f, 130.f), Vec2(300.f, 300.f), 10, 16);
	pParent->Animator2D()->CreateAnimation(L"WalkLeft", pAnimAtlas, Vec2(0.f, 650.f), Vec2(120.f, 130.f), Vec2(300.f, 300.f), 10, 16);
	pParent->Animator2D()->Play(L"WalkLeft", true);

	SpawnGameObject(pParent, Vec3(0.f, 0.f, 500.f), L"Player");


	CGameObject* pChild = new CGameObject;

	pChild->SetName(L"Child");
	pChild->AddComponent(new CTransform);
	pChild->AddComponent(new CMeshRender);

	pChild->Transform()->SetAbsolute(true);
	pChild->Transform()->SetRelativePos(200.f, 0.f, 0.f);
	pChild->Transform()->SetRelativeScale(100.f, 100.f, 1.f);

	pChild->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	pChild->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std2DLightMtrl"));
	pChild->MeshRender()->GetMaterial()->SetTexParam(TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"texture\\Fighter.bmp"));

	pParent->AddChild(pChild);

	// Monster
	CGameObject* pMonster = new CGameObject;
	pMonster->SetName(L"Monster");

	pMonster->AddComponent(new CTransform);
	pMonster->AddComponent(new CMeshRender);
	pMonster->AddComponent(new CCollider2D);
	pMonster->AddComponent(new CMonsterScript);

	pMonster->Transform()->SetRelativePos(Vec3(0.f, 250.f, 100.f));
	pMonster->Transform()->SetRelativeScale(Vec3(200.f, 200.f, 1.f));

	pMonster->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	pMonster->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"TestShaderMtrl"));	
	int i = 2;
	pMonster->MeshRender()->GetMaterial()->SetScalarParam(INT_0, &i);
	//pMonster->MeshRender()->GetMaterial()->SetTexParam(TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"texture\\smokeparticle.png"));


	pMonster->Collider2D()->SetAbsolute(true);
	pMonster->Collider2D()->SetOffsetScale(Vec2(100.f, 100.f));

	SpawnGameObject(pMonster, Vec3(0.f, 250.f, 100.f), L"Monster");


	// TileMap Object
	CGameObject* pTileMap = new CGameObject;
	pTileMap->SetName(L"TileMap");

	pTileMap->AddComponent(new CTransform);
	pTileMap->AddComponent(new CTileMap);

	pTileMap->Transform()->SetRelativePos(Vec3(0.f, 0.f, 600.f));
	pTileMap->Transform()->SetRelativeScale(Vec3(500.f, 500.f, 1.f));

	pTileMap->TileMap()->GetMaterial()->SetTexParam(TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"texture\\TILE.bmp"));
	pTileMap->TileMap()->SetSliceSize(Vec2(0.125f, 0.166f));
	pTileMap->TileMap()->SetTileCount(8, 8);

	SpawnGameObject(pTileMap, Vec3(0.f, 0.f, 600.f), L"Tile");

	// Particle Object
	CGameObject* pParticleObj = new CGameObject;
	pParticleObj->SetName(L"ParticleObject");
	pParticleObj->AddComponent(new CTransform);
	pParticleObj->AddComponent(new CParticleSystem);

	pParticleObj->ParticleSystem();

	SpawnGameObject(pParticleObj, Vec3(0.f, 0.f, 0.f), 0);


	/*CGameObject* pPostProcess = new CGameObject;
	pPostProcess->SetName(L"PostProcess");
	pPostProcess->AddComponent(new CTransform);
	pPostProcess->AddComponent(new CMeshRender);
	pPostProcess->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	pPostProcess->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"GrayMtrl"));
	SpawnGameObject(pPostProcess, Vec3(0.f, 0.f, 0.f), 0);*/

	/*CGameObject* pDistortion = new CGameObject;
	pDistortion->SetName(L"Distortion");
	pDistortion->AddComponent(new CTransform);
	pDistortion->AddComponent(new CMeshRender);
	pDistortion->Transform()->SetRelativeScale(200.f, 200.f, 1.f);
	pDistortion->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	pDistortion->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"DistortionMtrl"));
	SpawnGameObject(pDistortion, Vec3(0.f, 0.f, 500.f), 0);*/

	
	
	// 충돌 시킬 레이어 짝 지정
	CCollisionMgr::GetInst()->LayerCheck(L"Player", L"Monster");	
}

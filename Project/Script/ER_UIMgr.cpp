#include "pch.h"
#include "ER_UIMgr.h"

//Spawn Object를 위한 헤더
#include <Engine\CLevelMgr.h>
#include <Engine\CTransform.h>
#include <Engine\CMeshRender.h>
#include <Engine\CUI_Button.h>
#include "CUIScript_Button.h"
#include "ER_ItemMgr.h"
#include "ER_DataScript_Item.h"
#include "ER_GameSystem.h"
#include "ER_DataScript_Character.h"
#include <Engine\CRenderMgr.h>
#include <Engine\CDevice.h>
#include <Engine\CKeyMgr.h>

//테스트용
#include <Engine\CPathFindMgr.h>
#include <Engine\CCollisionMgr.h>

ER_UIMgr::ER_UIMgr()
{
}

ER_UIMgr::~ER_UIMgr()
{
}

void ER_UIMgr::init()
{
}

void ER_UIMgr::tick()
{
	if (KEY_TAP(KEY::F)) {
		CCamera* mainCam = CRenderMgr::GetInst()->GetMainCam();
		tRay ray = mainCam->GetRay();
		IntersectResult result = CCollisionMgr::GetInst()->IsCollidingBtwRayRect(ray, CPathFindMgr::GetInst()->GetMapCollider());
		Vec3 posResult = WorldPosToUIPos(result.vCrossPoint);
	}
}

void ER_UIMgr::GameStart()
{
	CreateBaseUI();

	CreateHPBar();

	CreateSkill();

	CreateInventorySlot();

	CreateInventoryItem();

	CreateEquipSlot();

	CreateEquipItem();

	CreateDropInventory();
}

void ER_UIMgr::CreateBaseUI()
{
#pragma region BaseUI
	//CGameObject* UITestObj = new CGameObject;
	//UITestObj->SetName(L"UI_SubStatus_Background");
	//
	//UITestObj->AddComponent(new CTransform);
	//UITestObj->AddComponent(new CMeshRender);
	//UITestObj->AddComponent(new CUI_Button);
	//UITestObj->AddComponent(new CUIScript_Button);
	//
	//UITestObj->Transform()->SetRelativeScale(Vec3(100.f, 100.f, 1.f));
	//
	//UITestObj->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	//UITestObj->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std2DMtrl"), 0);
	//UITestObj->MeshRender()->GetMaterial(0)->SetTexParam(TEX_PARAM::TEX_0, CResMgr::GetInst()->GetInst()->FindRes<CTexture>(L"SubStatus_Background.png"));
	//UITestObj->MeshRender()->GetDynamicMaterial(0);
	//
	//SpawnGameObject(UITestObj, Vec3(-1200.f, -300.f, 0.f), L"UI");

	CGameObject* UITestObj = new CGameObject;
	UITestObj->SetName(L"UI_MainStatus_Background");

	UITestObj->AddComponent(new CTransform);
	UITestObj->AddComponent(new CMeshRender);
	UITestObj->AddComponent(new CUI_Button);
	UITestObj->AddComponent(new CUIScript_Button);

	UITestObj->Transform()->SetRelativeScale(Vec3(156.097560f, 100.f, 1.f));

	UITestObj->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	UITestObj->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std2DMtrl"), 0);
	UITestObj->MeshRender()->GetMaterial(0)->SetTexParam(TEX_PARAM::TEX_0, CResMgr::GetInst()->GetInst()->FindRes<CTexture>(L"MainStatus_Background.png"));
	UITestObj->MeshRender()->GetDynamicMaterial(0);

	SpawnGameObject(UITestObj, Vec3(-520.f, -334.f, 0.f), L"UI");


	//UITestObj = new CGameObject;
	//UITestObj->SetName(L"UI_Player's Space Background");
	//
	//UITestObj->AddComponent(new CTransform);
	//UITestObj->AddComponent(new CMeshRender);
	//UITestObj->AddComponent(new CUI_Button);
	//UITestObj->AddComponent(new CUIScript_Button);
	//
	//UITestObj->Transform()->SetRelativeScale(Vec3(100.f, 130.158730f, 1.f));
	//
	//UITestObj->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	//UITestObj->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std2DMtrl"), 0);
	//UITestObj->MeshRender()->GetMaterial(0)->SetTexParam(TEX_PARAM::TEX_0, CResMgr::GetInst()->GetInst()->FindRes<CTexture>(L"Char_Portrait_Background.png"));
	//UITestObj->MeshRender()->GetDynamicMaterial(0);
	//
	//SpawnGameObject(UITestObj, Vec3(-369.f, -319.f, 0.f), L"UI");


	m_pPCFace = new CGameObject;
	m_pPCFace->SetName(L"UI_Player's Face");

	m_pPCFace->AddComponent(new CTransform);
	m_pPCFace->AddComponent(new CMeshRender);
	m_pPCFace->AddComponent(new CUI_Button);
	m_pPCFace->AddComponent(new CUIScript_Button);

	m_pPCFace->Transform()->SetRelativeScale(Vec3(100.f, 130.158730f, 1.f));
	m_pPCFace->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	m_pPCFace->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std2DMtrl"), 0);
	m_pPCFace->MeshRender()->GetMaterial(0)->SetTexParam(TEX_PARAM::TEX_0, ER_GameSystem::GetInst()->GetPlayerCharacter()->GetScript<ER_DataScript_Character>()->GetPortraitTex().Get());
	m_pPCFace->MeshRender()->GetDynamicMaterial(0);

	SpawnGameObject(m_pPCFace, Vec3(-379.f, -318.f, 0.f), L"UI");


	UITestObj = new CGameObject;
	UITestObj->SetName(L"UI_Equip_BackGround");

	UITestObj->AddComponent(new CTransform);
	UITestObj->AddComponent(new CMeshRender);
	UITestObj->AddComponent(new CUI_Button);
	UITestObj->AddComponent(new CUIScript_Button);

	UITestObj->Transform()->SetRelativeScale(Vec3(158.536585f, 100.f, 1.f));

	UITestObj->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	UITestObj->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std2DMtrl"), 0);
	UITestObj->MeshRender()->GetMaterial(0)->SetTexParam(TEX_PARAM::TEX_0, CResMgr::GetInst()->GetInst()->FindRes<CTexture>(L"Equip_BackGround_Copy.png"));
	UITestObj->MeshRender()->GetDynamicMaterial(0);

	SpawnGameObject(UITestObj, Vec3(-238.f, -334.f, 0.f), L"UI");

	UITestObj = new CGameObject;
	UITestObj->SetName(L"UI_Skill_BackGround");

	UITestObj->AddComponent(new CTransform);
	UITestObj->AddComponent(new CMeshRender);
	UITestObj->AddComponent(new CUI_Button);
	UITestObj->AddComponent(new CUIScript_Button);

	UITestObj->Transform()->SetRelativeScale(Vec3(314.634146f, 100.f, 1.f));

	UITestObj->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	UITestObj->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std2DMtrl"), 0);
	UITestObj->MeshRender()->GetMaterial(0)->SetTexParam(TEX_PARAM::TEX_0, CResMgr::GetInst()->GetInst()->FindRes<CTexture>(L"Skill_BackGround.png"));
	UITestObj->MeshRender()->GetDynamicMaterial(0);

	SpawnGameObject(UITestObj, Vec3(0.f, -334.f, 0.f), L"UI");

	UITestObj = new CGameObject;
	UITestObj->SetName(L"UI_Inventory_BackGround");

	UITestObj->AddComponent(new CTransform);
	UITestObj->AddComponent(new CMeshRender);
	UITestObj->AddComponent(new CUI_Button);
	UITestObj->AddComponent(new CUIScript_Button);

	UITestObj->Transform()->SetRelativeScale(Vec3(331.32530f, 100.0f, 1.f));

	UITestObj->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	UITestObj->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std2DMtrl"), 0);
	UITestObj->MeshRender()->GetMaterial(0)->SetTexParam(TEX_PARAM::TEX_0, CResMgr::GetInst()->GetInst()->FindRes<CTexture>(L"Inventory_BackGround.png"));
	UITestObj->MeshRender()->GetDynamicMaterial(0);

	SpawnGameObject(UITestObj, Vec3(325.f, -334.f, 0.f), L"UI");


	UITestObj = new CGameObject;
	UITestObj->SetName(L"UI_TimeBg");

	UITestObj->AddComponent(new CTransform);
	UITestObj->AddComponent(new CMeshRender);
	UITestObj->AddComponent(new CUI_Button);
	UITestObj->AddComponent(new CUIScript_Button);

	UITestObj->Transform()->SetRelativeScale(Vec3(204.34782f, 50.f, 1.f));

	UITestObj->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	UITestObj->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std2DMtrl"), 0);
	UITestObj->MeshRender()->GetMaterial(0)->SetTexParam(TEX_PARAM::TEX_0, CResMgr::GetInst()->GetInst()->FindRes<CTexture>(L"UI_TimeBg.png"));
	UITestObj->MeshRender()->GetDynamicMaterial(0);

	SpawnGameObject(UITestObj, Vec3(0.f, 336.f, 0.f), L"UI");
#pragma endregion
}

void ER_UIMgr::CreateHPBar()
{
	m_pHPBar = new CGameObject;
	m_pHPBar->SetName(L"UI_HPBar");

	m_pHPBar->AddComponent(new CTransform);
	m_pHPBar->AddComponent(new CMeshRender);
	m_pHPBar->AddComponent(new CUI_Button);
	m_pHPBar->AddComponent(new CUIScript_Button);

	m_pHPBar->Transform()->SetRelativeScale(Vec3(300.f, 12.f, 1.f));

	m_pHPBar->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	m_pHPBar->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std2DMtrl"), 0);
	m_pHPBar->MeshRender()->GetMaterial(0)->SetTexParam(TEX_PARAM::TEX_0, CResMgr::GetInst()->GetInst()->FindRes<CTexture>(L"HPBar_UI.png"));
	m_pHPBar->MeshRender()->GetDynamicMaterial(0);

	SpawnGameObject(m_pHPBar, Vec3(1.f, -356.5f, -1.f), L"UI");

	m_pStemnarBar = new CGameObject;
	m_pStemnarBar->SetName(L"UI_SteminarBar");

	m_pStemnarBar->AddComponent(new CTransform);
	m_pStemnarBar->AddComponent(new CMeshRender);
	m_pStemnarBar->AddComponent(new CUI_Button);
	m_pStemnarBar->AddComponent(new CUIScript_Button);

	m_pStemnarBar->Transform()->SetRelativeScale(Vec3(300.f, 12.f, 1.f));

	m_pStemnarBar->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	m_pStemnarBar->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std2DMtrl"), 0);
	m_pStemnarBar->MeshRender()->GetMaterial(0)->SetTexParam(TEX_PARAM::TEX_0, CResMgr::GetInst()->GetInst()->FindRes<CTexture>(L"SPBar_UI.png"));
	m_pStemnarBar->MeshRender()->GetDynamicMaterial(0);

	SpawnGameObject(m_pStemnarBar, Vec3(1.f, -372.f, -1.f), L"UI");
}

void ER_UIMgr::CreateSkill()
{
	CGameObject* skill = new CGameObject;
	skill->SetName(L"UI_SKill_Q");

	skill->AddComponent(new CTransform);
	skill->AddComponent(new CMeshRender);
	skill->AddComponent(new CUI_Button);
	skill->AddComponent(new CUIScript_Button);

	skill->Transform()->SetRelativeScale(Vec3(46.f, 46.f, 1.f));

	skill->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	skill->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std2DMtrl"), 0);
	skill->MeshRender()->GetMaterial(0)->SetTexParam(TEX_PARAM::TEX_0, ER_GameSystem::GetInst()->GetPlayerCharacter()->GetScript<ER_DataScript_Character>()->GetSkill(0)->TexSkill.Get());
	skill->MeshRender()->GetDynamicMaterial(0);

	SpawnGameObject(skill, Vec3(-126.5f, -314.5f, -1.f), L"UI");

	skill = new CGameObject;
	skill->SetName(L"UI_SKill_W");

	skill->AddComponent(new CTransform);
	skill->AddComponent(new CMeshRender);
	skill->AddComponent(new CUI_Button);
	skill->AddComponent(new CUIScript_Button);

	skill->Transform()->SetRelativeScale(Vec3(46.f, 46.f, 1.f));

	skill->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	skill->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std2DMtrl"), 0);
	skill->MeshRender()->GetMaterial(0)->SetTexParam(TEX_PARAM::TEX_0, ER_GameSystem::GetInst()->GetPlayerCharacter()->GetScript<ER_DataScript_Character>()->GetSkill(1)->TexSkill.Get());
	skill->MeshRender()->GetDynamicMaterial(0);

	SpawnGameObject(skill, Vec3(-74.f, -314.5f, -1.f), L"UI");

	skill = new CGameObject;
	skill->SetName(L"UI_SKill_E");

	skill->AddComponent(new CTransform);
	skill->AddComponent(new CMeshRender);
	skill->AddComponent(new CUI_Button);
	skill->AddComponent(new CUIScript_Button);

	skill->Transform()->SetRelativeScale(Vec3(46.f, 46.f, 1.f));

	skill->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	skill->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std2DMtrl"), 0);
	skill->MeshRender()->GetMaterial(0)->SetTexParam(TEX_PARAM::TEX_0, ER_GameSystem::GetInst()->GetPlayerCharacter()->GetScript<ER_DataScript_Character>()->GetSkill(2)->TexSkill.Get());
	skill->MeshRender()->GetDynamicMaterial(0);

	SpawnGameObject(skill, Vec3(-21.5f, -314.5f, -1.f), L"UI");

	skill = new CGameObject;
	skill->SetName(L"UI_SKill_R");

	skill->AddComponent(new CTransform);
	skill->AddComponent(new CMeshRender);
	skill->AddComponent(new CUI_Button);
	skill->AddComponent(new CUIScript_Button);

	skill->Transform()->SetRelativeScale(Vec3(46.f, 46.f, 1.f));

	skill->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	skill->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std2DMtrl"), 0);
	skill->MeshRender()->GetMaterial(0)->SetTexParam(TEX_PARAM::TEX_0, ER_GameSystem::GetInst()->GetPlayerCharacter()->GetScript<ER_DataScript_Character>()->GetSkill(3)->TexSkill.Get());
	skill->MeshRender()->GetDynamicMaterial(0);

	SpawnGameObject(skill, Vec3(31.f, -314.5f, -1.f), L"UI");
}

void ER_UIMgr::CreateInventorySlot()
{
#pragma region InventorySlot
	CGameObject* UITestObj = new CGameObject;
	UITestObj->SetName(L"UI_InvenSlot00");

	UITestObj->AddComponent(new CTransform);
	UITestObj->AddComponent(new CMeshRender);
	UITestObj->AddComponent(new CUI_Button);
	UITestObj->AddComponent(new CUIScript_Button);

	UITestObj->Transform()->SetRelativeScale(Vec3(54.731707f, 34.f, 1.f));

	UITestObj->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	UITestObj->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std2DMtrl"), 0);
	UITestObj->MeshRender()->GetMaterial(0)->SetTexParam(TEX_PARAM::TEX_0, CResMgr::GetInst()->GetInst()->FindRes<CTexture>(L"Ico_ItemGradebg_04.png"));
	UITestObj->MeshRender()->GetDynamicMaterial(0);

	m_aInventoryList[0][0].first = UITestObj;

	SpawnGameObject(UITestObj, Vec3(195.f, -325.f, -1.f), L"UI");


	UITestObj = new CGameObject;
	UITestObj->SetName(L"UI_InvenSlot01");

	UITestObj->AddComponent(new CTransform);
	UITestObj->AddComponent(new CMeshRender);
	UITestObj->AddComponent(new CUI_Button);
	UITestObj->AddComponent(new CUIScript_Button);

	UITestObj->Transform()->SetRelativeScale(Vec3(54.731707f, 34.f, 1.f));

	UITestObj->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	UITestObj->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std2DMtrl"), 0);
	UITestObj->MeshRender()->GetMaterial(0)->SetTexParam(TEX_PARAM::TEX_0, CResMgr::GetInst()->GetInst()->FindRes<CTexture>(L"Ico_ItemGradebg_04.png"));
	UITestObj->MeshRender()->GetDynamicMaterial(0);

	m_aInventoryList[0][1].first = UITestObj;

	SpawnGameObject(UITestObj, Vec3(252.f, -325.f, -1.f), L"UI");

	UITestObj = new CGameObject;
	UITestObj->SetName(L"UI_InvenSlot02");

	UITestObj->AddComponent(new CTransform);
	UITestObj->AddComponent(new CMeshRender);
	UITestObj->AddComponent(new CUI_Button);
	UITestObj->AddComponent(new CUIScript_Button);

	UITestObj->Transform()->SetRelativeScale(Vec3(54.731707f, 34.f, 1.f));

	UITestObj->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	UITestObj->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std2DMtrl"), 0);
	UITestObj->MeshRender()->GetMaterial(0)->SetTexParam(TEX_PARAM::TEX_0, CResMgr::GetInst()->GetInst()->FindRes<CTexture>(L"Ico_ItemGradebg_04.png"));
	UITestObj->MeshRender()->GetDynamicMaterial(0);

	m_aInventoryList[0][2].first = UITestObj;

	SpawnGameObject(UITestObj, Vec3(309.f, -325.f, -1.f), L"UI");

	UITestObj = new CGameObject;
	UITestObj->SetName(L"UI_InvenSlot03");

	UITestObj->AddComponent(new CTransform);
	UITestObj->AddComponent(new CMeshRender);
	UITestObj->AddComponent(new CUI_Button);
	UITestObj->AddComponent(new CUIScript_Button);

	UITestObj->Transform()->SetRelativeScale(Vec3(54.731707f, 34.f, 1.f));

	UITestObj->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	UITestObj->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std2DMtrl"), 0);
	UITestObj->MeshRender()->GetMaterial(0)->SetTexParam(TEX_PARAM::TEX_0, CResMgr::GetInst()->GetInst()->FindRes<CTexture>(L"Ico_ItemGradebg_04.png"));
	UITestObj->MeshRender()->GetDynamicMaterial(0);

	m_aInventoryList[0][3].first = UITestObj;

	SpawnGameObject(UITestObj, Vec3(366.f, -325.f, -1.f), L"UI");

	UITestObj = new CGameObject;
	UITestObj->SetName(L"UI_InvenSlot04");

	UITestObj->AddComponent(new CTransform);
	UITestObj->AddComponent(new CMeshRender);
	UITestObj->AddComponent(new CUI_Button);
	UITestObj->AddComponent(new CUIScript_Button);

	UITestObj->Transform()->SetRelativeScale(Vec3(54.731707f, 34.f, 1.f));

	UITestObj->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	UITestObj->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std2DMtrl"), 0);
	UITestObj->MeshRender()->GetMaterial(0)->SetTexParam(TEX_PARAM::TEX_0, CResMgr::GetInst()->GetInst()->FindRes<CTexture>(L"Ico_ItemGradebg_04.png"));
	UITestObj->MeshRender()->GetDynamicMaterial(0);

	m_aInventoryList[0][4].first = UITestObj;

	SpawnGameObject(UITestObj, Vec3(423.f, -325.f, -1.f), L"UI");

	UITestObj = new CGameObject;
	UITestObj->SetName(L"UI_InvenSlot10");

	UITestObj->AddComponent(new CTransform);
	UITestObj->AddComponent(new CMeshRender);
	UITestObj->AddComponent(new CUI_Button);
	UITestObj->AddComponent(new CUIScript_Button);

	UITestObj->Transform()->SetRelativeScale(Vec3(54.731707f, 34.f, 1.f));

	UITestObj->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	UITestObj->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std2DMtrl"), 0);
	UITestObj->MeshRender()->GetMaterial(0)->SetTexParam(TEX_PARAM::TEX_0, CResMgr::GetInst()->GetInst()->FindRes<CTexture>(L"Ico_ItemGradebg_04.png"));
	UITestObj->MeshRender()->GetDynamicMaterial(0);

	m_aInventoryList[1][0].first = UITestObj;

	SpawnGameObject(UITestObj, Vec3(195.f, -363.5f, -1.f), L"UI");

	UITestObj = new CGameObject;
	UITestObj->SetName(L"UI_InvenSlot11");

	UITestObj->AddComponent(new CTransform);
	UITestObj->AddComponent(new CMeshRender);
	UITestObj->AddComponent(new CUI_Button);
	UITestObj->AddComponent(new CUIScript_Button);

	UITestObj->Transform()->SetRelativeScale(Vec3(54.731707f, 34.f, 1.f));

	UITestObj->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	UITestObj->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std2DMtrl"), 0);
	UITestObj->MeshRender()->GetMaterial(0)->SetTexParam(TEX_PARAM::TEX_0, CResMgr::GetInst()->GetInst()->FindRes<CTexture>(L"Ico_ItemGradebg_04.png"));
	UITestObj->MeshRender()->GetDynamicMaterial(0);

	m_aInventoryList[1][1].first = UITestObj;

	SpawnGameObject(UITestObj, Vec3(252.f, -363.5f, -1.f), L"UI");

	UITestObj = new CGameObject;
	UITestObj->SetName(L"UI_InvenSlot12");

	UITestObj->AddComponent(new CTransform);
	UITestObj->AddComponent(new CMeshRender);
	UITestObj->AddComponent(new CUI_Button);
	UITestObj->AddComponent(new CUIScript_Button);

	UITestObj->Transform()->SetRelativeScale(Vec3(54.731707f, 34.f, 1.f));

	UITestObj->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	UITestObj->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std2DMtrl"), 0);
	UITestObj->MeshRender()->GetMaterial(0)->SetTexParam(TEX_PARAM::TEX_0, CResMgr::GetInst()->GetInst()->FindRes<CTexture>(L"Ico_ItemGradebg_04.png"));
	UITestObj->MeshRender()->GetDynamicMaterial(0);

	m_aInventoryList[1][2].first = UITestObj;

	SpawnGameObject(UITestObj, Vec3(309.f, -363.5f, -1.f), L"UI");

	UITestObj = new CGameObject;
	UITestObj->SetName(L"UI_InvenSlot13");

	UITestObj->AddComponent(new CTransform);
	UITestObj->AddComponent(new CMeshRender);
	UITestObj->AddComponent(new CUI_Button);
	UITestObj->AddComponent(new CUIScript_Button);

	UITestObj->Transform()->SetRelativeScale(Vec3(54.731707f, 34.f, 1.f));

	UITestObj->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	UITestObj->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std2DMtrl"), 0);
	UITestObj->MeshRender()->GetMaterial(0)->SetTexParam(TEX_PARAM::TEX_0, CResMgr::GetInst()->GetInst()->FindRes<CTexture>(L"Ico_ItemGradebg_04.png"));
	UITestObj->MeshRender()->GetDynamicMaterial(0);

	m_aInventoryList[1][3].first = UITestObj;

	SpawnGameObject(UITestObj, Vec3(366.f, -363.5f, -1.f), L"UI");

	UITestObj = new CGameObject;
	UITestObj->SetName(L"UI_InvenSlot14");

	UITestObj->AddComponent(new CTransform);
	UITestObj->AddComponent(new CMeshRender);
	UITestObj->AddComponent(new CUI_Button);
	UITestObj->AddComponent(new CUIScript_Button);

	UITestObj->Transform()->SetRelativeScale(Vec3(54.731707f, 34.f, 1.f));

	UITestObj->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	UITestObj->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std2DMtrl"), 0);
	UITestObj->MeshRender()->GetMaterial(0)->SetTexParam(TEX_PARAM::TEX_0, CResMgr::GetInst()->GetInst()->FindRes<CTexture>(L"Ico_ItemGradebg_04.png"));
	UITestObj->MeshRender()->GetDynamicMaterial(0);

	m_aInventoryList[1][4].first = UITestObj;

	SpawnGameObject(UITestObj, Vec3(423.f, -363.5f, -1.f), L"UI");
#pragma endregion
}

void ER_UIMgr::CreateInventoryItem()
{
#pragma region InventoryItemImg
	CGameObject* UITestObj = ER_ItemMgr::GetInst()->GetItemObj(84)->Clone();
	UITestObj->SetName(L"UI_InvenItemImg00");

	UITestObj->AddComponent(new CTransform);
	UITestObj->AddComponent(new CMeshRender);
	UITestObj->AddComponent(new CUI_Button);
	UITestObj->AddComponent(new CUIScript_Button);

	UITestObj->Transform()->SetRelativeScale(Vec3(54.731707f, 34.f, 1.f));

	UITestObj->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	UITestObj->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std2DMtrl"), 0);
	UITestObj->MeshRender()->GetMaterial(0)->SetTexParam(TEX_PARAM::TEX_0, UITestObj->GetScript<ER_DataScript_Item>()->GetItemTex().Get());
	UITestObj->MeshRender()->GetDynamicMaterial(0);

	m_aInventoryList[0][0].second = UITestObj;

	SpawnGameObject(UITestObj, Vec3(195.f, -325.f, -1.1f), L"UI");


	UITestObj = ER_ItemMgr::GetInst()->GetItemObj(84)->Clone();
	UITestObj->SetName(L"UI_InvenItemImg01");

	UITestObj->AddComponent(new CTransform);
	UITestObj->AddComponent(new CMeshRender);
	UITestObj->AddComponent(new CUI_Button);
	UITestObj->AddComponent(new CUIScript_Button);

	UITestObj->Transform()->SetRelativeScale(Vec3(54.731707f, 34.f, 1.f));

	UITestObj->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	UITestObj->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std2DMtrl"), 0);
	UITestObj->MeshRender()->GetMaterial(0)->SetTexParam(TEX_PARAM::TEX_0, UITestObj->GetScript<ER_DataScript_Item>()->GetItemTex().Get());
	UITestObj->MeshRender()->GetDynamicMaterial(0);

	m_aInventoryList[0][1].second = UITestObj;

	SpawnGameObject(UITestObj, Vec3(252.f, -325.f, -1.1f), L"UI");

	UITestObj = ER_ItemMgr::GetInst()->GetItemObj(84)->Clone();
	UITestObj->SetName(L"UI_InvenItemImg02");

	UITestObj->AddComponent(new CTransform);
	UITestObj->AddComponent(new CMeshRender);
	UITestObj->AddComponent(new CUI_Button);
	UITestObj->AddComponent(new CUIScript_Button);

	UITestObj->Transform()->SetRelativeScale(Vec3(54.731707f, 34.f, 1.f));

	UITestObj->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	UITestObj->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std2DMtrl"), 0);
	UITestObj->MeshRender()->GetMaterial(0)->SetTexParam(TEX_PARAM::TEX_0, UITestObj->GetScript<ER_DataScript_Item>()->GetItemTex().Get());
	UITestObj->MeshRender()->GetDynamicMaterial(0);

	m_aInventoryList[0][2].second = UITestObj;

	SpawnGameObject(UITestObj, Vec3(309.f, -325.f, -1.1f), L"UI");

	UITestObj = ER_ItemMgr::GetInst()->GetItemObj(84)->Clone();
	UITestObj->SetName(L"UI_InvenItemImg03");

	UITestObj->AddComponent(new CTransform);
	UITestObj->AddComponent(new CMeshRender);
	UITestObj->AddComponent(new CUI_Button);
	UITestObj->AddComponent(new CUIScript_Button);

	UITestObj->Transform()->SetRelativeScale(Vec3(54.731707f, 34.f, 1.f));

	UITestObj->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	UITestObj->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std2DMtrl"), 0);
	UITestObj->MeshRender()->GetMaterial(0)->SetTexParam(TEX_PARAM::TEX_0, UITestObj->GetScript<ER_DataScript_Item>()->GetItemTex().Get());
	UITestObj->MeshRender()->GetDynamicMaterial(0);

	m_aInventoryList[0][3].second = UITestObj;

	SpawnGameObject(UITestObj, Vec3(366.f, -325.f, -1.1f), L"UI");

	UITestObj = ER_ItemMgr::GetInst()->GetItemObj(84)->Clone();
	UITestObj->SetName(L"UI_InvenItemImg04");

	UITestObj->AddComponent(new CTransform);
	UITestObj->AddComponent(new CMeshRender);
	UITestObj->AddComponent(new CUI_Button);
	UITestObj->AddComponent(new CUIScript_Button);

	UITestObj->Transform()->SetRelativeScale(Vec3(54.731707f, 34.f, 1.f));

	UITestObj->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	UITestObj->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std2DMtrl"), 0);
	UITestObj->MeshRender()->GetMaterial(0)->SetTexParam(TEX_PARAM::TEX_0, UITestObj->GetScript<ER_DataScript_Item>()->GetItemTex().Get());
	UITestObj->MeshRender()->GetDynamicMaterial(0);

	m_aInventoryList[0][4].second = UITestObj;

	SpawnGameObject(UITestObj, Vec3(423.f, -325.f, -1.1f), L"UI");

	UITestObj = ER_ItemMgr::GetInst()->GetItemObj(84)->Clone();
	UITestObj->SetName(L"UI_InvenItemImg10");

	UITestObj->AddComponent(new CTransform);
	UITestObj->AddComponent(new CMeshRender);
	UITestObj->AddComponent(new CUI_Button);
	UITestObj->AddComponent(new CUIScript_Button);

	UITestObj->Transform()->SetRelativeScale(Vec3(54.731707f, 34.f, 1.f));

	UITestObj->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	UITestObj->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std2DMtrl"), 0);
	UITestObj->MeshRender()->GetMaterial(0)->SetTexParam(TEX_PARAM::TEX_0, UITestObj->GetScript<ER_DataScript_Item>()->GetItemTex().Get());
	UITestObj->MeshRender()->GetDynamicMaterial(0);

	m_aInventoryList[1][0].second = UITestObj;

	SpawnGameObject(UITestObj, Vec3(195.f, -363.5f, -1.1f), L"UI");

	UITestObj = ER_ItemMgr::GetInst()->GetItemObj(84)->Clone();
	UITestObj->SetName(L"UI_InvenItemImg11");

	UITestObj->AddComponent(new CTransform);
	UITestObj->AddComponent(new CMeshRender);
	UITestObj->AddComponent(new CUI_Button);
	UITestObj->AddComponent(new CUIScript_Button);

	UITestObj->Transform()->SetRelativeScale(Vec3(54.731707f, 34.f, 1.f));

	UITestObj->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	UITestObj->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std2DMtrl"), 0);
	UITestObj->MeshRender()->GetMaterial(0)->SetTexParam(TEX_PARAM::TEX_0, UITestObj->GetScript<ER_DataScript_Item>()->GetItemTex().Get());
	UITestObj->MeshRender()->GetDynamicMaterial(0);

	m_aInventoryList[1][1].second = UITestObj;

	SpawnGameObject(UITestObj, Vec3(252.f, -363.5f, -1.1f), L"UI");

	UITestObj = ER_ItemMgr::GetInst()->GetItemObj(84)->Clone();
	UITestObj->SetName(L"UI_InvenItemImg12");

	UITestObj->AddComponent(new CTransform);
	UITestObj->AddComponent(new CMeshRender);
	UITestObj->AddComponent(new CUI_Button);
	UITestObj->AddComponent(new CUIScript_Button);

	UITestObj->Transform()->SetRelativeScale(Vec3(54.731707f, 34.f, 1.f));

	UITestObj->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	UITestObj->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std2DMtrl"), 0);
	UITestObj->MeshRender()->GetMaterial(0)->SetTexParam(TEX_PARAM::TEX_0, UITestObj->GetScript<ER_DataScript_Item>()->GetItemTex().Get());
	UITestObj->MeshRender()->GetDynamicMaterial(0);

	m_aInventoryList[1][2].second = UITestObj;

	SpawnGameObject(UITestObj, Vec3(309.f, -363.5f, -1.1f), L"UI");

	UITestObj = ER_ItemMgr::GetInst()->GetItemObj(84)->Clone();
	UITestObj->SetName(L"UI_InvenItemImg13");

	UITestObj->AddComponent(new CTransform);
	UITestObj->AddComponent(new CMeshRender);
	UITestObj->AddComponent(new CUI_Button);
	UITestObj->AddComponent(new CUIScript_Button);

	UITestObj->Transform()->SetRelativeScale(Vec3(54.731707f, 34.f, 1.f));

	UITestObj->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	UITestObj->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std2DMtrl"), 0);
	UITestObj->MeshRender()->GetMaterial(0)->SetTexParam(TEX_PARAM::TEX_0, UITestObj->GetScript<ER_DataScript_Item>()->GetItemTex().Get());
	UITestObj->MeshRender()->GetDynamicMaterial(0);

	m_aInventoryList[1][3].second = UITestObj;

	SpawnGameObject(UITestObj, Vec3(366.f, -363.5f, -1.1f), L"UI");

	UITestObj = ER_ItemMgr::GetInst()->GetItemObj(84)->Clone();
	UITestObj->SetName(L"UI_InvenItemImg14");

	UITestObj->AddComponent(new CTransform);
	UITestObj->AddComponent(new CMeshRender);
	UITestObj->AddComponent(new CUI_Button);
	UITestObj->AddComponent(new CUIScript_Button);

	UITestObj->Transform()->SetRelativeScale(Vec3(54.731707f, 34.f, 1.f));

	UITestObj->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	UITestObj->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std2DMtrl"), 0);
	UITestObj->MeshRender()->GetMaterial(0)->SetTexParam(TEX_PARAM::TEX_0, UITestObj->GetScript<ER_DataScript_Item>()->GetItemTex().Get());
	UITestObj->MeshRender()->GetDynamicMaterial(0);

	m_aInventoryList[1][4].second = UITestObj;

	SpawnGameObject(UITestObj, Vec3(423.f, -363.5f, -1.1f), L"UI");
#pragma endregion
}

void ER_UIMgr::CreateEquipSlot()
{
#pragma region EquipSlot
	CGameObject* UITestObj = new CGameObject;
	UITestObj->SetName(L"UI_EquipSlot00");

	UITestObj->AddComponent(new CTransform);
	UITestObj->AddComponent(new CMeshRender);
	UITestObj->AddComponent(new CUI_Button);
	UITestObj->AddComponent(new CUIScript_Button);

	UITestObj->Transform()->SetRelativeScale(Vec3(47.f, 29.196969f, 1.f));

	UITestObj->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	UITestObj->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std2DMtrl"), 0);
	UITestObj->MeshRender()->GetMaterial(0)->SetTexParam(TEX_PARAM::TEX_0, CResMgr::GetInst()->GetInst()->FindRes<CTexture>(L"Ico_ItemGradebg_04.png"));
	UITestObj->MeshRender()->GetDynamicMaterial(0);

	m_aEquipList[0][0].first = UITestObj;


	SpawnGameObject(UITestObj, Vec3(-286.f, -313.f, -1.f), L"UI");

	UITestObj = new CGameObject;
	UITestObj->SetName(L"UI_EquipSlot01");

	UITestObj->AddComponent(new CTransform);
	UITestObj->AddComponent(new CMeshRender);
	UITestObj->AddComponent(new CUI_Button);
	UITestObj->AddComponent(new CUIScript_Button);

	UITestObj->Transform()->SetRelativeScale(Vec3(47.f, 29.196969f, 1.f));

	UITestObj->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	UITestObj->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std2DMtrl"), 0);
	UITestObj->MeshRender()->GetMaterial(0)->SetTexParam(TEX_PARAM::TEX_0, CResMgr::GetInst()->GetInst()->FindRes<CTexture>(L"Ico_ItemGradebg_04.png"));
	UITestObj->MeshRender()->GetDynamicMaterial(0);

	m_aEquipList[0][1].first = UITestObj;

	SpawnGameObject(UITestObj, Vec3(-237.5f, -313.f, -1.f), L"UI");


	UITestObj = new CGameObject;
	UITestObj->SetName(L"UI_EquipSlot02");

	UITestObj->AddComponent(new CTransform);
	UITestObj->AddComponent(new CMeshRender);
	UITestObj->AddComponent(new CUI_Button);
	UITestObj->AddComponent(new CUIScript_Button);

	UITestObj->Transform()->SetRelativeScale(Vec3(47.f, 29.196969f, 1.f));

	UITestObj->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	UITestObj->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std2DMtrl"), 0);
	UITestObj->MeshRender()->GetMaterial(0)->SetTexParam(TEX_PARAM::TEX_0, CResMgr::GetInst()->GetInst()->FindRes<CTexture>(L"Ico_ItemGradebg_04.png"));
	UITestObj->MeshRender()->GetDynamicMaterial(0);

	m_aEquipList[0][2].first = UITestObj;

	SpawnGameObject(UITestObj, Vec3(-189.f, -313.f, -1.f), L"UI");


	UITestObj = new CGameObject;
	UITestObj->SetName(L"UI_EquipSlot10");

	UITestObj->AddComponent(new CTransform);
	UITestObj->AddComponent(new CMeshRender);
	UITestObj->AddComponent(new CUI_Button);
	UITestObj->AddComponent(new CUIScript_Button);

	UITestObj->Transform()->SetRelativeScale(Vec3(47.f, 29.196969f, 1.f));

	UITestObj->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	UITestObj->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std2DMtrl"), 0);
	UITestObj->MeshRender()->GetMaterial(0)->SetTexParam(TEX_PARAM::TEX_0, CResMgr::GetInst()->GetInst()->FindRes<CTexture>(L"Ico_ItemGradebg_04.png"));
	UITestObj->MeshRender()->GetDynamicMaterial(0);

	m_aEquipList[1][0].first = UITestObj;

	SpawnGameObject(UITestObj, Vec3(-259.5f, -354.5f, -1.f), L"UI");


	UITestObj = new CGameObject;
	UITestObj->SetName(L"UI_EquipSlot11");

	UITestObj->AddComponent(new CTransform);
	UITestObj->AddComponent(new CMeshRender);
	UITestObj->AddComponent(new CUI_Button);
	UITestObj->AddComponent(new CUIScript_Button);

	UITestObj->Transform()->SetRelativeScale(Vec3(47.f, 29.196969f, 1.f));

	UITestObj->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	UITestObj->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std2DMtrl"), 0);
	UITestObj->MeshRender()->GetMaterial(0)->SetTexParam(TEX_PARAM::TEX_0, CResMgr::GetInst()->GetInst()->FindRes<CTexture>(L"Ico_ItemGradebg_04.png"));
	UITestObj->MeshRender()->GetDynamicMaterial(0);

	m_aEquipList[1][1].first = UITestObj;

	SpawnGameObject(UITestObj, Vec3(-211.f, -354.5f, -1.f), L"UI");
#pragma endregion
}

void ER_UIMgr::CreateEquipItem()
{
	CGameObject* UITestObj = ER_ItemMgr::GetInst()->GetItemObj(85)->Clone();
	UITestObj->SetName(L"UI_EquipSlot00");

	UITestObj->AddComponent(new CTransform);
	UITestObj->AddComponent(new CMeshRender);
	UITestObj->AddComponent(new CUI_Button);
	UITestObj->AddComponent(new CUIScript_Button);

	UITestObj->Transform()->SetRelativeScale(Vec3(47.f, 29.196969f, 1.f));

	UITestObj->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	UITestObj->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std2DMtrl"), 0);
	UITestObj->MeshRender()->GetMaterial(0)->SetTexParam(TEX_PARAM::TEX_0, UITestObj->GetScript<ER_DataScript_Item>()->GetItemTex().Get());
	UITestObj->MeshRender()->GetDynamicMaterial(0);

	m_aEquipList[0][0].second = UITestObj;

	SpawnGameObject(UITestObj, Vec3(-286.f, -313.f, -1.1f), L"UI");



	UITestObj = ER_ItemMgr::GetInst()->GetItemObj(85)->Clone();
	UITestObj->SetName(L"UI_EquipSlot01");

	UITestObj->AddComponent(new CTransform);
	UITestObj->AddComponent(new CMeshRender);
	UITestObj->AddComponent(new CUI_Button);
	UITestObj->AddComponent(new CUIScript_Button);

	UITestObj->Transform()->SetRelativeScale(Vec3(47.f, 29.196969f, 1.f));

	UITestObj->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	UITestObj->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std2DMtrl"), 0);
	UITestObj->MeshRender()->GetMaterial(0)->SetTexParam(TEX_PARAM::TEX_0, UITestObj->GetScript<ER_DataScript_Item>()->GetItemTex().Get());
	UITestObj->MeshRender()->GetDynamicMaterial(0);

	m_aEquipList[0][1].second = UITestObj;

	SpawnGameObject(UITestObj, Vec3(-237.5f, -313.f, -1.1f), L"UI");


	UITestObj = ER_ItemMgr::GetInst()->GetItemObj(85)->Clone();
	UITestObj->SetName(L"UI_EquipSlot02");

	UITestObj->AddComponent(new CTransform);
	UITestObj->AddComponent(new CMeshRender);
	UITestObj->AddComponent(new CUI_Button);
	UITestObj->AddComponent(new CUIScript_Button);

	UITestObj->Transform()->SetRelativeScale(Vec3(47.f, 29.196969f, 1.f));

	UITestObj->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	UITestObj->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std2DMtrl"), 0);
	UITestObj->MeshRender()->GetMaterial(0)->SetTexParam(TEX_PARAM::TEX_0, UITestObj->GetScript<ER_DataScript_Item>()->GetItemTex().Get());
	UITestObj->MeshRender()->GetDynamicMaterial(0);

	m_aEquipList[0][2].second = UITestObj;

	SpawnGameObject(UITestObj, Vec3(-189.f, -313.f, -1.1f), L"UI");


	UITestObj = ER_ItemMgr::GetInst()->GetItemObj(85)->Clone();
	UITestObj->SetName(L"UI_EquipSlot10");
	
	UITestObj->AddComponent(new CTransform);
	UITestObj->AddComponent(new CMeshRender);
	UITestObj->AddComponent(new CUI_Button);
	UITestObj->AddComponent(new CUIScript_Button);
	
	UITestObj->Transform()->SetRelativeScale(Vec3(47.f, 29.196969f, 1.f));
	
	UITestObj->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	UITestObj->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std2DMtrl"), 0);
	UITestObj->MeshRender()->GetMaterial(0)->SetTexParam(TEX_PARAM::TEX_0, UITestObj->GetScript<ER_DataScript_Item>()->GetItemTex().Get());
	UITestObj->MeshRender()->GetDynamicMaterial(0);
	
	m_aEquipList[1][0].second = UITestObj;

	SpawnGameObject(UITestObj, Vec3(-259.5f, -354.5f, -1.1f), L"UI");
	
	
	UITestObj = ER_ItemMgr::GetInst()->GetItemObj(85)->Clone();
	UITestObj->SetName(L"UI_EquipSlot11");
	
	UITestObj->AddComponent(new CTransform);
	UITestObj->AddComponent(new CMeshRender);
	UITestObj->AddComponent(new CUI_Button);
	UITestObj->AddComponent(new CUIScript_Button);
	
	UITestObj->Transform()->SetRelativeScale(Vec3(47.f, 29.196969f, 1.f));
	
	UITestObj->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	UITestObj->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std2DMtrl"), 0);
	UITestObj->MeshRender()->GetMaterial(0)->SetTexParam(TEX_PARAM::TEX_0, UITestObj->GetScript<ER_DataScript_Item>()->GetItemTex().Get());
	UITestObj->MeshRender()->GetDynamicMaterial(0);
	
	m_aEquipList[1][1].second = UITestObj;

	SpawnGameObject(UITestObj, Vec3(-211.f, -354.5f, -1.1f), L"UI");
}

void ER_UIMgr::CreateDropInventory()
{
#pragma region dropInventoryBackground
	m_pItemBox = new CGameObject();
	m_pItemBox->SetName(L"UI_ItemBox_BackGround");

	m_pItemBox->AddComponent(new CTransform);
	m_pItemBox->AddComponent(new CMeshRender);
	m_pItemBox->AddComponent(new CUI_Button);
	m_pItemBox->AddComponent(new CUIScript_Button);

	m_pItemBox->Transform()->SetRelativeScale(Vec3(261.02362f, 150.f, 1.f));

	m_pItemBox->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	m_pItemBox->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std2DMtrl"), 0);
	m_pItemBox->MeshRender()->GetMaterial(0)->SetTexParam(TEX_PARAM::TEX_0, CResMgr::GetInst()->GetInst()->FindRes<CTexture>(L"ItemBox_BackGround.png"));
	m_pItemBox->MeshRender()->GetDynamicMaterial(0);

	SpawnGameObject(m_pItemBox, Vec3(0.f, 0.f, -1.1f), L"UI");
#pragma endregion
	
#pragma region dropInventorySlot
	CGameObject* dropItem = new CGameObject();
	dropItem->SetName(L"UI_ItemBoxSlotm00");

	dropItem->AddComponent(new CTransform);
	dropItem->AddComponent(new CMeshRender);
	dropItem->AddComponent(new CUI_Button);
	dropItem->AddComponent(new CUIScript_Button);

	dropItem->Transform()->SetRelativeScale(Vec3(0.20222f, 0.21000f, 1.f));
	dropItem->Transform()->SetRelativePos(Vec3(-0.35f, 0.06f, -1.f));

	dropItem->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	dropItem->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std2DMtrl"), 0);
	dropItem->MeshRender()->GetMaterial(0)->SetTexParam(TEX_PARAM::TEX_0, CResMgr::GetInst()->GetInst()->FindRes<CTexture>(L"ItemBg_UnCommon.png"));
	dropItem->MeshRender()->GetDynamicMaterial(0);

	m_aItemBoxList[0][0].first = dropItem;
	m_pItemBox->AddChild(dropItem);
	
	
	dropItem = new CGameObject();
	dropItem->SetName(L"UI_ItemBoxSlot01");

	dropItem->AddComponent(new CTransform);
	dropItem->AddComponent(new CMeshRender);
	dropItem->AddComponent(new CUI_Button);
	dropItem->AddComponent(new CUIScript_Button);

	dropItem->Transform()->SetRelativeScale(Vec3(0.20222f, 0.21000f, 1.f));
	dropItem->Transform()->SetRelativePos(Vec3(-0.115f, 0.06f, -1.f));

	dropItem->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	dropItem->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std2DMtrl"), 0);
	dropItem->MeshRender()->GetMaterial(0)->SetTexParam(TEX_PARAM::TEX_0, CResMgr::GetInst()->GetInst()->FindRes<CTexture>(L"ItemBg_UnCommon.png"));
	dropItem->MeshRender()->GetDynamicMaterial(0);

	m_aItemBoxList[0][1].first = dropItem;
	m_pItemBox->AddChild(dropItem);



	dropItem = new CGameObject();
	dropItem->SetName(L"UI_ItemBoxSlot02");

	dropItem->AddComponent(new CTransform);
	dropItem->AddComponent(new CMeshRender);
	dropItem->AddComponent(new CUI_Button);
	dropItem->AddComponent(new CUIScript_Button);

	dropItem->Transform()->SetRelativeScale(Vec3(0.20222f, 0.21000f, 1.f));
	dropItem->Transform()->SetRelativePos(Vec3(0.12f, 0.06f, -1.f));

	dropItem->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	dropItem->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std2DMtrl"), 0);
	dropItem->MeshRender()->GetMaterial(0)->SetTexParam(TEX_PARAM::TEX_0, CResMgr::GetInst()->GetInst()->FindRes<CTexture>(L"ItemBg_UnCommon.png"));
	dropItem->MeshRender()->GetDynamicMaterial(0);

	m_aItemBoxList[0][2].first = dropItem;
	m_pItemBox->AddChild(dropItem);



	dropItem = new CGameObject();
	dropItem->SetName(L"UI_ItemBoxSlot03");

	dropItem->AddComponent(new CTransform);
	dropItem->AddComponent(new CMeshRender);
	dropItem->AddComponent(new CUI_Button);
	dropItem->AddComponent(new CUIScript_Button);

	dropItem->Transform()->SetRelativeScale(Vec3(0.20222f, 0.21000f, 1.f));
	dropItem->Transform()->SetRelativePos(Vec3(0.355f, 0.06f, -1.f));

	dropItem->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	dropItem->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std2DMtrl"), 0);
	dropItem->MeshRender()->GetMaterial(0)->SetTexParam(TEX_PARAM::TEX_0, CResMgr::GetInst()->GetInst()->FindRes<CTexture>(L"ItemBg_UnCommon.png"));
	dropItem->MeshRender()->GetDynamicMaterial(0);

	m_aItemBoxList[0][3].first = dropItem;
	m_pItemBox->AddChild(dropItem);


	dropItem = new CGameObject();
	dropItem->SetName(L"UI_ItemBoxSlot10");

	dropItem->AddComponent(new CTransform);
	dropItem->AddComponent(new CMeshRender);
	dropItem->AddComponent(new CUI_Button);
	dropItem->AddComponent(new CUIScript_Button);

	dropItem->Transform()->SetRelativeScale(Vec3(0.20222f, 0.21000f, 1.f));
	dropItem->Transform()->SetRelativePos(Vec3(-0.35f, -0.227f, -1.f));

	dropItem->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	dropItem->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std2DMtrl"), 0);
	dropItem->MeshRender()->GetMaterial(0)->SetTexParam(TEX_PARAM::TEX_0, CResMgr::GetInst()->GetInst()->FindRes<CTexture>(L"ItemBg_UnCommon.png"));
	dropItem->MeshRender()->GetDynamicMaterial(0);

	m_aItemBoxList[1][0].first = dropItem;
	m_pItemBox->AddChild(dropItem);

	dropItem = new CGameObject();
	dropItem->SetName(L"UI_ItemBoxSlot11");

	dropItem->AddComponent(new CTransform);
	dropItem->AddComponent(new CMeshRender);
	dropItem->AddComponent(new CUI_Button);
	dropItem->AddComponent(new CUIScript_Button);

	dropItem->Transform()->SetRelativeScale(Vec3(0.20222f, 0.21000f, 1.f));
	dropItem->Transform()->SetRelativePos(Vec3(-0.115f, -0.227f, -1.f));

	dropItem->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	dropItem->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std2DMtrl"), 0);
	dropItem->MeshRender()->GetMaterial(0)->SetTexParam(TEX_PARAM::TEX_0, CResMgr::GetInst()->GetInst()->FindRes<CTexture>(L"ItemBg_UnCommon.png"));
	dropItem->MeshRender()->GetDynamicMaterial(0);

	m_aItemBoxList[1][1].first = dropItem;
	m_pItemBox->AddChild(dropItem);



	dropItem = new CGameObject();
	dropItem->SetName(L"UI_ItemBoxSlot12");

	dropItem->AddComponent(new CTransform);
	dropItem->AddComponent(new CMeshRender);
	dropItem->AddComponent(new CUI_Button);
	dropItem->AddComponent(new CUIScript_Button);

	dropItem->Transform()->SetRelativeScale(Vec3(0.20222f, 0.21000f, 1.f));
	dropItem->Transform()->SetRelativePos(Vec3(0.12f, -0.227f, -1.f));

	dropItem->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	dropItem->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std2DMtrl"), 0);
	dropItem->MeshRender()->GetMaterial(0)->SetTexParam(TEX_PARAM::TEX_0, CResMgr::GetInst()->GetInst()->FindRes<CTexture>(L"ItemBg_UnCommon.png"));
	dropItem->MeshRender()->GetDynamicMaterial(0);

	m_aItemBoxList[1][2].first = dropItem;
	m_pItemBox->AddChild(dropItem);



	dropItem = new CGameObject();
	dropItem->SetName(L"UI_ItemBoxSlot13");

	dropItem->AddComponent(new CTransform);
	dropItem->AddComponent(new CMeshRender);
	dropItem->AddComponent(new CUI_Button);
	dropItem->AddComponent(new CUIScript_Button);

	dropItem->Transform()->SetRelativeScale(Vec3(0.20222f, 0.21000f, 1.f));
	dropItem->Transform()->SetRelativePos(Vec3(0.355f, -0.227f, -1.f));

	dropItem->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	dropItem->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std2DMtrl"), 0);
	dropItem->MeshRender()->GetMaterial(0)->SetTexParam(TEX_PARAM::TEX_0, CResMgr::GetInst()->GetInst()->FindRes<CTexture>(L"ItemBg_UnCommon.png"));
	dropItem->MeshRender()->GetDynamicMaterial(0);

	m_aItemBoxList[1][3].first = dropItem;
	m_pItemBox->AddChild(dropItem);
#pragma endregion

#pragma region dropInventoryItem
	dropItem = ER_ItemMgr::GetInst()->GetItemObj(86)->Clone();
	dropItem->SetName(L"UI_ItemBoxITem00");

	dropItem->AddComponent(new CTransform);
	dropItem->AddComponent(new CMeshRender);
	dropItem->AddComponent(new CUI_Button);
	dropItem->AddComponent(new CUIScript_Button);

	dropItem->Transform()->SetRelativeScale(Vec3(0.20222f, 0.21000f, 1.f));
	dropItem->Transform()->SetRelativePos(Vec3(-0.35f, 0.06f, -1.1f));

	dropItem->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	dropItem->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std2DMtrl"), 0);
	dropItem->MeshRender()->GetMaterial(0)->SetTexParam(TEX_PARAM::TEX_0, dropItem->GetScript<ER_DataScript_Item>()->GetItemTex().Get());
	dropItem->MeshRender()->GetDynamicMaterial(0);

	m_aItemBoxList[0][0].second = dropItem;
	m_pItemBox->AddChild(dropItem);


	dropItem = ER_ItemMgr::GetInst()->GetItemObj(86)->Clone();
	dropItem->SetName(L"UI_ItemBoxITem01");

	dropItem->AddComponent(new CTransform);
	dropItem->AddComponent(new CMeshRender);
	dropItem->AddComponent(new CUI_Button);
	dropItem->AddComponent(new CUIScript_Button);

	dropItem->Transform()->SetRelativeScale(Vec3(0.20222f, 0.21000f, 1.f));
	dropItem->Transform()->SetRelativePos(Vec3(-0.115f, 0.06f, -1.1f));

	dropItem->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	dropItem->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std2DMtrl"), 0);
	dropItem->MeshRender()->GetMaterial(0)->SetTexParam(TEX_PARAM::TEX_0, dropItem->GetScript<ER_DataScript_Item>()->GetItemTex().Get());
	dropItem->MeshRender()->GetDynamicMaterial(0);

	m_aItemBoxList[0][1].second = dropItem;
	m_pItemBox->AddChild(dropItem);



	dropItem = ER_ItemMgr::GetInst()->GetItemObj(86)->Clone();
	dropItem->SetName(L"UI_ItemBoxITem02");

	dropItem->AddComponent(new CTransform);
	dropItem->AddComponent(new CMeshRender);
	dropItem->AddComponent(new CUI_Button);
	dropItem->AddComponent(new CUIScript_Button);

	dropItem->Transform()->SetRelativeScale(Vec3(0.20222f, 0.21000f, 1.f));
	dropItem->Transform()->SetRelativePos(Vec3(0.12f, 0.06f, -1.1f));

	dropItem->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	dropItem->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std2DMtrl"), 0);
	dropItem->MeshRender()->GetMaterial(0)->SetTexParam(TEX_PARAM::TEX_0, dropItem->GetScript<ER_DataScript_Item>()->GetItemTex().Get());
	dropItem->MeshRender()->GetDynamicMaterial(0);

	m_aItemBoxList[0][2].second = dropItem;
	m_pItemBox->AddChild(dropItem);



	dropItem = ER_ItemMgr::GetInst()->GetItemObj(86)->Clone();
	dropItem->SetName(L"UI_ItemBoxITem03");

	dropItem->AddComponent(new CTransform);
	dropItem->AddComponent(new CMeshRender);
	dropItem->AddComponent(new CUI_Button);
	dropItem->AddComponent(new CUIScript_Button);

	dropItem->Transform()->SetRelativeScale(Vec3(0.20222f, 0.21000f, 1.f));
	dropItem->Transform()->SetRelativePos(Vec3(0.355f, 0.06f, -1.1f));

	dropItem->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	dropItem->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std2DMtrl"), 0);
	dropItem->MeshRender()->GetMaterial(0)->SetTexParam(TEX_PARAM::TEX_0, dropItem->GetScript<ER_DataScript_Item>()->GetItemTex().Get());
	dropItem->MeshRender()->GetDynamicMaterial(0);

	m_aItemBoxList[0][3].second = dropItem;
	m_pItemBox->AddChild(dropItem);


	dropItem = ER_ItemMgr::GetInst()->GetItemObj(86)->Clone();
	dropItem->SetName(L"UI_ItemBoxITem10");

	dropItem->AddComponent(new CTransform);
	dropItem->AddComponent(new CMeshRender);
	dropItem->AddComponent(new CUI_Button);
	dropItem->AddComponent(new CUIScript_Button);

	dropItem->Transform()->SetRelativeScale(Vec3(0.20222f, 0.21000f, 1.f));
	dropItem->Transform()->SetRelativePos(Vec3(-0.35f, -0.227f, -1.1f));

	dropItem->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	dropItem->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std2DMtrl"), 0);
	dropItem->MeshRender()->GetMaterial(0)->SetTexParam(TEX_PARAM::TEX_0, dropItem->GetScript<ER_DataScript_Item>()->GetItemTex().Get());
	dropItem->MeshRender()->GetDynamicMaterial(0);

	m_aItemBoxList[1][0].second = dropItem;
	m_pItemBox->AddChild(dropItem);



	dropItem = ER_ItemMgr::GetInst()->GetItemObj(86)->Clone();
	dropItem->SetName(L"UI_ItemBoxITem11");

	dropItem->AddComponent(new CTransform);
	dropItem->AddComponent(new CMeshRender);
	dropItem->AddComponent(new CUI_Button);
	dropItem->AddComponent(new CUIScript_Button);

	dropItem->Transform()->SetRelativeScale(Vec3(0.20222f, 0.21000f, 1.f));
	dropItem->Transform()->SetRelativePos(Vec3(-0.115f, -0.227f, -1.1f));

	dropItem->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	dropItem->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std2DMtrl"), 0);
	dropItem->MeshRender()->GetMaterial(0)->SetTexParam(TEX_PARAM::TEX_0, dropItem->GetScript<ER_DataScript_Item>()->GetItemTex().Get());
	dropItem->MeshRender()->GetDynamicMaterial(0);

	m_aItemBoxList[1][1].second = dropItem;
	m_pItemBox->AddChild(dropItem);



	dropItem = ER_ItemMgr::GetInst()->GetItemObj(86)->Clone();
	dropItem->SetName(L"UI_ItemBoxITem12");

	dropItem->AddComponent(new CTransform);
	dropItem->AddComponent(new CMeshRender);
	dropItem->AddComponent(new CUI_Button);
	dropItem->AddComponent(new CUIScript_Button);

	dropItem->Transform()->SetRelativeScale(Vec3(0.20222f, 0.21000f, 1.f));
	dropItem->Transform()->SetRelativePos(Vec3(0.12f, -0.227f, -1.1f));

	dropItem->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	dropItem->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std2DMtrl"), 0);
	dropItem->MeshRender()->GetMaterial(0)->SetTexParam(TEX_PARAM::TEX_0, dropItem->GetScript<ER_DataScript_Item>()->GetItemTex().Get());
	dropItem->MeshRender()->GetDynamicMaterial(0);

	m_aItemBoxList[1][2].second = dropItem;
	m_pItemBox->AddChild(dropItem);



	dropItem = ER_ItemMgr::GetInst()->GetItemObj(86)->Clone();
	dropItem->SetName(L"UI_ItemBoxITem13");

	dropItem->AddComponent(new CTransform);
	dropItem->AddComponent(new CMeshRender);
	dropItem->AddComponent(new CUI_Button);
	dropItem->AddComponent(new CUIScript_Button);

	dropItem->Transform()->SetRelativeScale(Vec3(0.20222f, 0.21000f, 1.f));
	dropItem->Transform()->SetRelativePos(Vec3(0.355f, -0.227f, -1.1f));

	dropItem->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	dropItem->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std2DMtrl"), 0);
	dropItem->MeshRender()->GetMaterial(0)->SetTexParam(TEX_PARAM::TEX_0, dropItem->GetScript<ER_DataScript_Item>()->GetItemTex().Get());
	dropItem->MeshRender()->GetDynamicMaterial(0);

	m_aItemBoxList[1][3].second = dropItem;
	m_pItemBox->AddChild(dropItem);

#pragma endregion

	//m_pItemBox->SetEnable(false);
}

Vec3 ER_UIMgr::WorldPosToUIPos(const Vec3& worldPos)
{
	// World 위치 벡터
	XMVECTOR pos = XMLoadFloat3(&worldPos);

	// 뷰 매트릭스와 프로젝션 매트릭스 로드
	XMMATRIX view = CRenderMgr::GetInst()->GetMainCam()->GetViewMat();
	XMMATRIX proj = CRenderMgr::GetInst()->GetMainCam()->GetProjMat();

	Vec2 vResol = CDevice::GetInst()->GetRenderResolution();

	// 스크린 좌표로 변환
	XMVECTOR screenPos = XMVector3Project(pos, 0.0f, 0.0f, vResol.x, vResol.y, 0.0f, 1.0f, proj, view, XMMatrixIdentity());

	Vec3 result;
	XMStoreFloat3(&result, screenPos);

	result.x -= (vResol.x / 2.f);
	result.y -= (vResol.y / 2.f);

	return result;
}

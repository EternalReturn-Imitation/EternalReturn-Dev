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
	int a = 0;
}

void ER_UIMgr::GameStart()
{
	CreateBaseUI();

	CreateHPBar();

	CreateInventorySlot();

	CreateInventoryItem();

	CreateEquipSlot();

	CreateEquipItem();
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

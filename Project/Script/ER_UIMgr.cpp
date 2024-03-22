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
#include <FontEngine\FW1FontWrapper.h>
#include "ER_struct.h"
#include <Engine\CPathMgr.h>
#include <Engine\CEngine.h>

//테스트용
#include <Engine\CPathFindMgr.h>
#include <Engine\CCollisionMgr.h>
#include <tchar.h>

ER_UIMgr::ER_UIMgr()
	:m_bHPChangeTrigger(false)
{
	if (Gdiplus::GdiplusStartup(&m_uGdiplusToken, &m_gGdiplusStartupInput, NULL) != Gdiplus::Ok)
		assert(false);
}

ER_UIMgr::~ER_UIMgr()
{
	// 사용 후 정리
	DeleteObject(m_hBitmap); // 비트맵 리소스 해제
	Gdiplus::GdiplusShutdown(m_uGdiplusToken); // GDI+ 정리
}

void ER_UIMgr::init()
{
	wstring ws = CPathMgr::GetInst()->GetContentPath();
	ws += L"texture\\UI\\cursor\\Cursor_01.png";

	m_hBitmap = LoadPNGAsBitmap(ws.c_str());	
}

void ER_UIMgr::tick()
{
	//if (KEY_TAP(KEY::F)) {
	//	UpdateSteminar();
	//}

	//HPReturnBar업데이트
	if (m_bHPChangeTrigger) {
		UpdateHPReturnBar();
	}

	if (m_hBitmap)
	{
		HWND hWnd = CEngine::GetInst()->GetMainWnd();
		HCURSOR hCursor = BitmapToCursor(hWnd, m_hBitmap);
		SetCursor(hCursor);
	}
}

void ER_UIMgr::GameStart()
{
	m_pGradeTexture[0] = CResMgr::GetInst()->FindRes<CTexture>(L"ItemBg_Common.png");
	m_pGradeTexture[1] = CResMgr::GetInst()->FindRes<CTexture>(L"ItemBg_UnCommon.png");
	m_pGradeTexture[2] = CResMgr::GetInst()->FindRes<CTexture>(L"ItemBg_Rare.png");
	m_pGradeTexture[3] = CResMgr::GetInst()->FindRes<CTexture>(L"ItemBg_Epic.png");
	m_pGradeTexture[4] = CResMgr::GetInst()->FindRes<CTexture>(L"ItemBg_Legendary.png");

	CreateBaseUI();

	CreateHPBar();

	CreateSkill();

	CreateInventorySlot();

	CreateInventoryItem();

	CreateEquipSlot();

	CreateEquipItem();

	CreateDropInventory();

	CreateStatText();

	UpdateStat();
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

	UITestObj->Transform()->SetRelativeScale(Vec3(195.12195f, 125.f, 1.f));

	UITestObj->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	UITestObj->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std2DMtrl"), 0);
	UITestObj->MeshRender()->GetMaterial(0)->SetTexParam(TEX_PARAM::TEX_0, CResMgr::GetInst()->GetInst()->FindRes<CTexture>(L"MainStatus_Background_Copy.png"));
	UITestObj->MeshRender()->GetDynamicMaterial(0);

	SpawnGameObject(UITestObj, Vec3(-624.f, -386.f, 0.f), L"UI");


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

	m_pPCFace->Transform()->SetRelativeScale(Vec3(125.f, 162.6984125f, 1.f));
	m_pPCFace->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	m_pPCFace->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std2DMtrl"), 0);
	m_pPCFace->MeshRender()->GetMaterial(0)->SetTexParam(TEX_PARAM::TEX_0, ER_GameSystem::GetInst()->GetPlayerCharacter()->GetScript<ER_DataScript_Character>()->GetPortraitTex().Get());
	m_pPCFace->MeshRender()->GetDynamicMaterial(0);

	SpawnGameObject(m_pPCFace, Vec3(-460.f, -368.f, 0.f), L"UI");


	UITestObj = new CGameObject;
	UITestObj->SetName(L"UI_Equip_BackGround");

	UITestObj->AddComponent(new CTransform);
	UITestObj->AddComponent(new CMeshRender);
	UITestObj->AddComponent(new CUI_Button);
	UITestObj->AddComponent(new CUIScript_Button);

	UITestObj->Transform()->SetRelativeScale(Vec3(198.17073125f, 125.f, 1.f));

	UITestObj->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	UITestObj->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std2DMtrl"), 0);
	UITestObj->MeshRender()->GetMaterial(0)->SetTexParam(TEX_PARAM::TEX_0, CResMgr::GetInst()->GetInst()->FindRes<CTexture>(L"Equip_BackGround_Copy.png"));
	UITestObj->MeshRender()->GetDynamicMaterial(0);

	SpawnGameObject(UITestObj, Vec3(-297.f, -386.5f, 0.f), L"UI");

	UITestObj = new CGameObject;
	UITestObj->SetName(L"UI_Skill_BackGround");

	UITestObj->AddComponent(new CTransform);
	UITestObj->AddComponent(new CMeshRender);
	UITestObj->AddComponent(new CUI_Button);
	UITestObj->AddComponent(new CUIScript_Button);

	UITestObj->Transform()->SetRelativeScale(Vec3(393.2926825f, 125.f, 1.f));

	UITestObj->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	UITestObj->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std2DMtrl"), 0);
	UITestObj->MeshRender()->GetMaterial(0)->SetTexParam(TEX_PARAM::TEX_0, CResMgr::GetInst()->GetInst()->FindRes<CTexture>(L"Skill_BackGround.png"));
	UITestObj->MeshRender()->GetDynamicMaterial(0);

	SpawnGameObject(UITestObj, Vec3(0.f, -387.f, 0.f), L"UI");

	UITestObj = new CGameObject;
	UITestObj->SetName(L"UI_Inventory_BackGround");

	UITestObj->AddComponent(new CTransform);
	UITestObj->AddComponent(new CMeshRender);
	UITestObj->AddComponent(new CUI_Button);
	UITestObj->AddComponent(new CUIScript_Button);

	UITestObj->Transform()->SetRelativeScale(Vec3(414.156625f, 125.0f, 1.f));

	UITestObj->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	UITestObj->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std2DMtrl"), 0);
	UITestObj->MeshRender()->GetMaterial(0)->SetTexParam(TEX_PARAM::TEX_0, CResMgr::GetInst()->GetInst()->FindRes<CTexture>(L"Inventory_BackGround.png"));
	UITestObj->MeshRender()->GetDynamicMaterial(0);

	SpawnGameObject(UITestObj, Vec3(405.f, -386.5f, 0.f), L"UI");


	UITestObj = new CGameObject;
	UITestObj->SetName(L"UI_TimeBg");

	UITestObj->AddComponent(new CTransform);
	UITestObj->AddComponent(new CMeshRender);
	UITestObj->AddComponent(new CUI_Button);
	UITestObj->AddComponent(new CUIScript_Button);

	UITestObj->Transform()->SetRelativeScale(Vec3(255.434775f, 62.5f, 1.f));

	UITestObj->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	UITestObj->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std2DMtrl"), 0);
	UITestObj->MeshRender()->GetMaterial(0)->SetTexParam(TEX_PARAM::TEX_0, CResMgr::GetInst()->GetInst()->FindRes<CTexture>(L"UI_TimeBg.png"));
	UITestObj->MeshRender()->GetDynamicMaterial(0);

	SpawnGameObject(UITestObj, Vec3(0.f, 396.f, -3.f), L"UI");
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

	m_pHPBar->Transform()->SetRelativeScale(Vec3(375.f, 15.f, 1.f));

	m_pHPBar->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	m_pHPBar->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std2DMtrl"), 0);
	m_pHPBar->MeshRender()->GetMaterial(0)->SetTexParam(TEX_PARAM::TEX_0, CResMgr::GetInst()->GetInst()->FindRes<CTexture>(L"HPBar_UI.png"));
	m_pHPBar->MeshRender()->GetDynamicMaterial(0);

	SpawnGameObject(m_pHPBar, Vec3(1.f, -415.5f, -1.2f), L"UI");

	m_pHPReturnBar = new CGameObject;
	m_pHPReturnBar->SetName(L"UI_HPReturnBar");

	m_pHPReturnBar->AddComponent(new CTransform);
	m_pHPReturnBar->AddComponent(new CMeshRender);
	m_pHPReturnBar->AddComponent(new CUI_Button);
	m_pHPReturnBar->AddComponent(new CUIScript_Button);

	m_pHPReturnBar->Transform()->SetRelativeScale(Vec3(375.f, 15.f, 1.f));

	m_pHPReturnBar->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	m_pHPReturnBar->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std2DMtrl"), 0);
	m_pHPReturnBar->MeshRender()->GetMaterial(0)->SetTexParam(TEX_PARAM::TEX_0, CResMgr::GetInst()->GetInst()->FindRes<CTexture>(L"ReturnBar_UI.png"));
	m_pHPReturnBar->MeshRender()->GetDynamicMaterial(0);

	SpawnGameObject(m_pHPReturnBar, Vec3(1.f, -415.5f, -1.f), L"UI");

	m_pStemnarBar = new CGameObject;
	m_pStemnarBar->SetName(L"UI_SteminarBar");

	m_pStemnarBar->AddComponent(new CTransform);
	m_pStemnarBar->AddComponent(new CMeshRender);
	m_pStemnarBar->AddComponent(new CUI_Button);
	m_pStemnarBar->AddComponent(new CUIScript_Button);

	m_pStemnarBar->Transform()->SetRelativeScale(Vec3(375.f, 15.f, 1.f));

	m_pStemnarBar->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	m_pStemnarBar->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std2DMtrl"), 0);
	m_pStemnarBar->MeshRender()->GetMaterial(0)->SetTexParam(TEX_PARAM::TEX_0, CResMgr::GetInst()->GetInst()->FindRes<CTexture>(L"SPBar_UI.png"));
	m_pStemnarBar->MeshRender()->GetDynamicMaterial(0);

	SpawnGameObject(m_pStemnarBar, Vec3(1.f, -435.f, -1.1f), L"UI");
}

void ER_UIMgr::CreateSkill()
{
	CGameObject* skill = new CGameObject;
	skill->SetName(L"UI_SKill_Q");

	skill->AddComponent(new CTransform);
	skill->AddComponent(new CMeshRender);
	skill->AddComponent(new CUI_Button);
	skill->AddComponent(new CUIScript_Button);

	skill->Transform()->SetRelativeScale(Vec3(57.5f, 57.5f, 1.f));

	skill->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	skill->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std2DMtrl"), 0);
	skill->MeshRender()->GetMaterial(0)->SetTexParam(TEX_PARAM::TEX_0, ER_GameSystem::GetInst()->GetPlayerCharacter()->GetScript<ER_DataScript_Character>()->GetSkill(0)->TexSkill.Get());
	skill->MeshRender()->GetDynamicMaterial(0);

	SpawnGameObject(skill, Vec3(-157.5f, -362.5f, -1.f), L"UI");

	skill = new CGameObject;
	skill->SetName(L"UI_SKill_W");

	skill->AddComponent(new CTransform);
	skill->AddComponent(new CMeshRender);
	skill->AddComponent(new CUI_Button);
	skill->AddComponent(new CUIScript_Button);

	skill->Transform()->SetRelativeScale(Vec3(57.5f, 57.5f, 1.f));

	skill->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	skill->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std2DMtrl"), 0);
	skill->MeshRender()->GetMaterial(0)->SetTexParam(TEX_PARAM::TEX_0, ER_GameSystem::GetInst()->GetPlayerCharacter()->GetScript<ER_DataScript_Character>()->GetSkill(1)->TexSkill.Get());
	skill->MeshRender()->GetDynamicMaterial(0);

	SpawnGameObject(skill, Vec3(-92.f, -362.5f, -1.f), L"UI");

	skill = new CGameObject;
	skill->SetName(L"UI_SKill_E");

	skill->AddComponent(new CTransform);
	skill->AddComponent(new CMeshRender);
	skill->AddComponent(new CUI_Button);
	skill->AddComponent(new CUIScript_Button);

	skill->Transform()->SetRelativeScale(Vec3(57.5f, 57.5f, 1.f));

	skill->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	skill->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std2DMtrl"), 0);
	skill->MeshRender()->GetMaterial(0)->SetTexParam(TEX_PARAM::TEX_0, ER_GameSystem::GetInst()->GetPlayerCharacter()->GetScript<ER_DataScript_Character>()->GetSkill(2)->TexSkill.Get());
	skill->MeshRender()->GetDynamicMaterial(0);

	SpawnGameObject(skill, Vec3(-26.5f, -362.5f, -1.f), L"UI");

	skill = new CGameObject;
	skill->SetName(L"UI_SKill_R");

	skill->AddComponent(new CTransform);
	skill->AddComponent(new CMeshRender);
	skill->AddComponent(new CUI_Button);
	skill->AddComponent(new CUIScript_Button);

	skill->Transform()->SetRelativeScale(Vec3(57.5f, 57.5f, 1.f));

	skill->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	skill->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std2DMtrl"), 0);
	skill->MeshRender()->GetMaterial(0)->SetTexParam(TEX_PARAM::TEX_0, ER_GameSystem::GetInst()->GetPlayerCharacter()->GetScript<ER_DataScript_Character>()->GetSkill(3)->TexSkill.Get());
	skill->MeshRender()->GetDynamicMaterial(0);

	SpawnGameObject(skill, Vec3(39.f, -362.5f, -1.f), L"UI");
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

	UITestObj->Transform()->SetRelativeScale(Vec3(68.41463375f, 42.5f, 1.f));

	UITestObj->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	UITestObj->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std2DMtrl"), 0);
	UITestObj->MeshRender()->GetMaterial(0)->SetTexParam(TEX_PARAM::TEX_0, CResMgr::GetInst()->GetInst()->FindRes<CTexture>(L"Ico_ItemGradebg_04.png"));
	UITestObj->MeshRender()->GetDynamicMaterial(0);

	UITestObj->GetScript<CUIScript_Button>()->SetUIPos(0);
	UITestObj->GetScript<CUIScript_Button>()->SetUICase(1);

	m_aInventoryList[0][0].first = UITestObj;

	SpawnGameObject(UITestObj, Vec3(242.5f, -375.5f, -1.f), L"UI");


	UITestObj = new CGameObject;
	UITestObj->SetName(L"UI_InvenSlot01");

	UITestObj->AddComponent(new CTransform);
	UITestObj->AddComponent(new CMeshRender);
	UITestObj->AddComponent(new CUI_Button);
	UITestObj->AddComponent(new CUIScript_Button);

	UITestObj->Transform()->SetRelativeScale(Vec3(68.41463375f, 42.5f, 1.f));

	UITestObj->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	UITestObj->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std2DMtrl"), 0);
	UITestObj->MeshRender()->GetMaterial(0)->SetTexParam(TEX_PARAM::TEX_0, CResMgr::GetInst()->GetInst()->FindRes<CTexture>(L"Ico_ItemGradebg_04.png"));
	UITestObj->MeshRender()->GetDynamicMaterial(0);

	UITestObj->GetScript<CUIScript_Button>()->SetUIPos(1);
	UITestObj->GetScript<CUIScript_Button>()->SetUICase(1);

	m_aInventoryList[0][1].first = UITestObj;

	SpawnGameObject(UITestObj, Vec3(313.5f, -375.5f, -1.f), L"UI");

	UITestObj = new CGameObject;
	UITestObj->SetName(L"UI_InvenSlot02");

	UITestObj->AddComponent(new CTransform);
	UITestObj->AddComponent(new CMeshRender);
	UITestObj->AddComponent(new CUI_Button);
	UITestObj->AddComponent(new CUIScript_Button);

	UITestObj->Transform()->SetRelativeScale(Vec3(68.41463375f, 42.5f, 1.f));

	UITestObj->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	UITestObj->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std2DMtrl"), 0);
	UITestObj->MeshRender()->GetMaterial(0)->SetTexParam(TEX_PARAM::TEX_0, CResMgr::GetInst()->GetInst()->FindRes<CTexture>(L"Ico_ItemGradebg_04.png"));
	UITestObj->MeshRender()->GetDynamicMaterial(0);

	UITestObj->GetScript<CUIScript_Button>()->SetUIPos(2);
	UITestObj->GetScript<CUIScript_Button>()->SetUICase(1);

	m_aInventoryList[0][2].first = UITestObj;

	SpawnGameObject(UITestObj, Vec3(384.5f, -375.5f, -1.f), L"UI");

	UITestObj = new CGameObject;
	UITestObj->SetName(L"UI_InvenSlot03");

	UITestObj->AddComponent(new CTransform);
	UITestObj->AddComponent(new CMeshRender);
	UITestObj->AddComponent(new CUI_Button);
	UITestObj->AddComponent(new CUIScript_Button);

	UITestObj->Transform()->SetRelativeScale(Vec3(68.41463375f, 42.5f, 1.f));

	UITestObj->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	UITestObj->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std2DMtrl"), 0);
	UITestObj->MeshRender()->GetMaterial(0)->SetTexParam(TEX_PARAM::TEX_0, CResMgr::GetInst()->GetInst()->FindRes<CTexture>(L"Ico_ItemGradebg_04.png"));
	UITestObj->MeshRender()->GetDynamicMaterial(0);

	UITestObj->GetScript<CUIScript_Button>()->SetUIPos(3);
	UITestObj->GetScript<CUIScript_Button>()->SetUICase(1);

	m_aInventoryList[0][3].first = UITestObj;

	SpawnGameObject(UITestObj, Vec3(455.5f, -375.5f, -1.f), L"UI");

	UITestObj = new CGameObject;
	UITestObj->SetName(L"UI_InvenSlot04");

	UITestObj->AddComponent(new CTransform);
	UITestObj->AddComponent(new CMeshRender);
	UITestObj->AddComponent(new CUI_Button);
	UITestObj->AddComponent(new CUIScript_Button);

	UITestObj->Transform()->SetRelativeScale(Vec3(68.41463375f, 42.5f, 1.f));

	UITestObj->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	UITestObj->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std2DMtrl"), 0);
	UITestObj->MeshRender()->GetMaterial(0)->SetTexParam(TEX_PARAM::TEX_0, CResMgr::GetInst()->GetInst()->FindRes<CTexture>(L"Ico_ItemGradebg_04.png"));
	UITestObj->MeshRender()->GetDynamicMaterial(0);

	UITestObj->GetScript<CUIScript_Button>()->SetUIPos(4);
	UITestObj->GetScript<CUIScript_Button>()->SetUICase(1);

	m_aInventoryList[0][4].first = UITestObj;

	SpawnGameObject(UITestObj, Vec3(526.5f, -375.5f, -1.f), L"UI");

	UITestObj = new CGameObject;
	UITestObj->SetName(L"UI_InvenSlot10");

	UITestObj->AddComponent(new CTransform);
	UITestObj->AddComponent(new CMeshRender);
	UITestObj->AddComponent(new CUI_Button);
	UITestObj->AddComponent(new CUIScript_Button);

	UITestObj->Transform()->SetRelativeScale(Vec3(68.41463375f, 42.5f, 1.f));

	UITestObj->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	UITestObj->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std2DMtrl"), 0);
	UITestObj->MeshRender()->GetMaterial(0)->SetTexParam(TEX_PARAM::TEX_0, CResMgr::GetInst()->GetInst()->FindRes<CTexture>(L"Ico_ItemGradebg_04.png"));
	UITestObj->MeshRender()->GetDynamicMaterial(0);

	UITestObj->GetScript<CUIScript_Button>()->SetUIPos(5);
	UITestObj->GetScript<CUIScript_Button>()->SetUICase(1);

	m_aInventoryList[1][0].first = UITestObj;

	SpawnGameObject(UITestObj, Vec3(242.5f, -423.f, -1.f), L"UI");

	UITestObj = new CGameObject;
	UITestObj->SetName(L"UI_InvenSlot11");

	UITestObj->AddComponent(new CTransform);
	UITestObj->AddComponent(new CMeshRender);
	UITestObj->AddComponent(new CUI_Button);
	UITestObj->AddComponent(new CUIScript_Button);

	UITestObj->Transform()->SetRelativeScale(Vec3(68.41463375f, 42.5f, 1.f));

	UITestObj->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	UITestObj->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std2DMtrl"), 0);
	UITestObj->MeshRender()->GetMaterial(0)->SetTexParam(TEX_PARAM::TEX_0, CResMgr::GetInst()->GetInst()->FindRes<CTexture>(L"Ico_ItemGradebg_04.png"));
	UITestObj->MeshRender()->GetDynamicMaterial(0);

	UITestObj->GetScript<CUIScript_Button>()->SetUIPos(6);
	UITestObj->GetScript<CUIScript_Button>()->SetUICase(1);

	m_aInventoryList[1][1].first = UITestObj;

	SpawnGameObject(UITestObj, Vec3(313.5f, -423.f, -1.f), L"UI");

	UITestObj = new CGameObject;
	UITestObj->SetName(L"UI_InvenSlot12");

	UITestObj->AddComponent(new CTransform);
	UITestObj->AddComponent(new CMeshRender);
	UITestObj->AddComponent(new CUI_Button);
	UITestObj->AddComponent(new CUIScript_Button);

	UITestObj->Transform()->SetRelativeScale(Vec3(68.41463375f, 42.5f, 1.f));

	UITestObj->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	UITestObj->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std2DMtrl"), 0);
	UITestObj->MeshRender()->GetMaterial(0)->SetTexParam(TEX_PARAM::TEX_0, CResMgr::GetInst()->GetInst()->FindRes<CTexture>(L"Ico_ItemGradebg_04.png"));
	UITestObj->MeshRender()->GetDynamicMaterial(0);

	UITestObj->GetScript<CUIScript_Button>()->SetUIPos(7);
	UITestObj->GetScript<CUIScript_Button>()->SetUICase(1);

	m_aInventoryList[1][2].first = UITestObj;

	SpawnGameObject(UITestObj, Vec3(384.5f, -423.f, -1.f), L"UI");

	UITestObj = new CGameObject;
	UITestObj->SetName(L"UI_InvenSlot13");

	UITestObj->AddComponent(new CTransform);
	UITestObj->AddComponent(new CMeshRender);
	UITestObj->AddComponent(new CUI_Button);
	UITestObj->AddComponent(new CUIScript_Button);

	UITestObj->Transform()->SetRelativeScale(Vec3(68.41463375f, 42.5f, 1.f));

	UITestObj->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	UITestObj->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std2DMtrl"), 0);
	UITestObj->MeshRender()->GetMaterial(0)->SetTexParam(TEX_PARAM::TEX_0, CResMgr::GetInst()->GetInst()->FindRes<CTexture>(L"Ico_ItemGradebg_04.png"));
	UITestObj->MeshRender()->GetDynamicMaterial(0);

	UITestObj->GetScript<CUIScript_Button>()->SetUIPos(8);
	UITestObj->GetScript<CUIScript_Button>()->SetUICase(1);

	m_aInventoryList[1][3].first = UITestObj;

	SpawnGameObject(UITestObj, Vec3(455.5f, -423.f, -1.f), L"UI");

	UITestObj = new CGameObject;
	UITestObj->SetName(L"UI_InvenSlot14");

	UITestObj->AddComponent(new CTransform);
	UITestObj->AddComponent(new CMeshRender);
	UITestObj->AddComponent(new CUI_Button);
	UITestObj->AddComponent(new CUIScript_Button);

	UITestObj->Transform()->SetRelativeScale(Vec3(68.41463375f, 42.5f, 1.f));

	UITestObj->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	UITestObj->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std2DMtrl"), 0);
	UITestObj->MeshRender()->GetMaterial(0)->SetTexParam(TEX_PARAM::TEX_0, CResMgr::GetInst()->GetInst()->FindRes<CTexture>(L"Ico_ItemGradebg_04.png"));
	UITestObj->MeshRender()->GetDynamicMaterial(0);

	UITestObj->GetScript<CUIScript_Button>()->SetUIPos(9);
	UITestObj->GetScript<CUIScript_Button>()->SetUICase(1);

	m_aInventoryList[1][4].first = UITestObj;

	SpawnGameObject(UITestObj, Vec3(526.5f, -423.f, -1.f), L"UI");
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

	UITestObj->Transform()->SetRelativeScale(Vec3(68.41463375f, 42.5f, 1.f));

	UITestObj->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	UITestObj->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std2DMtrl"), 0);
	UITestObj->MeshRender()->GetMaterial(0)->SetTexParam(TEX_PARAM::TEX_0, UITestObj->GetScript<ER_DataScript_Item>()->GetItemTex().Get());
	UITestObj->MeshRender()->GetDynamicMaterial(0);

	m_aInventoryList[0][0].second = UITestObj;

	UITestObj->GetScript<CUIScript_Button>()->SetUIPos(0);
	UITestObj->GetScript<CUIScript_Button>()->SetUICase(1);

	SpawnGameObject(UITestObj, Vec3(242.5f, -375.5f, -1.1f), L"UI");


	UITestObj = ER_ItemMgr::GetInst()->GetItemObj(84)->Clone();
	UITestObj->SetName(L"UI_InvenItemImg01");

	UITestObj->AddComponent(new CTransform);
	UITestObj->AddComponent(new CMeshRender);
	UITestObj->AddComponent(new CUI_Button);
	UITestObj->AddComponent(new CUIScript_Button);

	UITestObj->Transform()->SetRelativeScale(Vec3(68.41463375f, 42.5f, 1.f));

	UITestObj->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	UITestObj->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std2DMtrl"), 0);
	UITestObj->MeshRender()->GetMaterial(0)->SetTexParam(TEX_PARAM::TEX_0, UITestObj->GetScript<ER_DataScript_Item>()->GetItemTex().Get());
	UITestObj->MeshRender()->GetDynamicMaterial(0);

	UITestObj->GetScript<CUIScript_Button>()->SetUIPos(1);
	UITestObj->GetScript<CUIScript_Button>()->SetUICase(1);

	m_aInventoryList[0][1].second = UITestObj;

	SpawnGameObject(UITestObj, Vec3(313.5f, -375.5f, -1.1f), L"UI");

	UITestObj = ER_ItemMgr::GetInst()->GetItemObj(84)->Clone();
	UITestObj->SetName(L"UI_InvenItemImg02");

	UITestObj->AddComponent(new CTransform);
	UITestObj->AddComponent(new CMeshRender);
	UITestObj->AddComponent(new CUI_Button);
	UITestObj->AddComponent(new CUIScript_Button);

	UITestObj->Transform()->SetRelativeScale(Vec3(68.41463375f, 42.5f, 1.f));

	UITestObj->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	UITestObj->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std2DMtrl"), 0);
	UITestObj->MeshRender()->GetMaterial(0)->SetTexParam(TEX_PARAM::TEX_0, UITestObj->GetScript<ER_DataScript_Item>()->GetItemTex().Get());
	UITestObj->MeshRender()->GetDynamicMaterial(0);

	UITestObj->GetScript<CUIScript_Button>()->SetUIPos(2);
	UITestObj->GetScript<CUIScript_Button>()->SetUICase(1);

	m_aInventoryList[0][2].second = UITestObj;

	SpawnGameObject(UITestObj, Vec3(384.5f, -375.5f, -1.1f), L"UI");

	UITestObj = ER_ItemMgr::GetInst()->GetItemObj(84)->Clone();
	UITestObj->SetName(L"UI_InvenItemImg03");

	UITestObj->AddComponent(new CTransform);
	UITestObj->AddComponent(new CMeshRender);
	UITestObj->AddComponent(new CUI_Button);
	UITestObj->AddComponent(new CUIScript_Button);

	UITestObj->Transform()->SetRelativeScale(Vec3(68.41463375f, 42.5f, 1.f));

	UITestObj->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	UITestObj->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std2DMtrl"), 0);
	UITestObj->MeshRender()->GetMaterial(0)->SetTexParam(TEX_PARAM::TEX_0, UITestObj->GetScript<ER_DataScript_Item>()->GetItemTex().Get());
	UITestObj->MeshRender()->GetDynamicMaterial(0);

	UITestObj->GetScript<CUIScript_Button>()->SetUIPos(3);
	UITestObj->GetScript<CUIScript_Button>()->SetUICase(1);

	m_aInventoryList[0][3].second = UITestObj;

	SpawnGameObject(UITestObj, Vec3(455.5f, -375.5f, -1.1f), L"UI");

	UITestObj = ER_ItemMgr::GetInst()->GetItemObj(84)->Clone();
	UITestObj->SetName(L"UI_InvenItemImg04");

	UITestObj->AddComponent(new CTransform);
	UITestObj->AddComponent(new CMeshRender);
	UITestObj->AddComponent(new CUI_Button);
	UITestObj->AddComponent(new CUIScript_Button);

	UITestObj->Transform()->SetRelativeScale(Vec3(68.41463375f, 42.5f, 1.f));

	UITestObj->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	UITestObj->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std2DMtrl"), 0);
	UITestObj->MeshRender()->GetMaterial(0)->SetTexParam(TEX_PARAM::TEX_0, UITestObj->GetScript<ER_DataScript_Item>()->GetItemTex().Get());
	UITestObj->MeshRender()->GetDynamicMaterial(0);

	UITestObj->GetScript<CUIScript_Button>()->SetUIPos(4);
	UITestObj->GetScript<CUIScript_Button>()->SetUICase(1);

	m_aInventoryList[0][4].second = UITestObj;

	SpawnGameObject(UITestObj, Vec3(526.5f, -375.5f, -1.1f), L"UI");

	UITestObj = ER_ItemMgr::GetInst()->GetItemObj(84)->Clone();
	UITestObj->SetName(L"UI_InvenItemImg10");

	UITestObj->AddComponent(new CTransform);
	UITestObj->AddComponent(new CMeshRender);
	UITestObj->AddComponent(new CUI_Button);
	UITestObj->AddComponent(new CUIScript_Button);

	UITestObj->Transform()->SetRelativeScale(Vec3(68.41463375f, 42.5f, 1.f));

	UITestObj->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	UITestObj->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std2DMtrl"), 0);
	UITestObj->MeshRender()->GetMaterial(0)->SetTexParam(TEX_PARAM::TEX_0, UITestObj->GetScript<ER_DataScript_Item>()->GetItemTex().Get());
	UITestObj->MeshRender()->GetDynamicMaterial(0);

	UITestObj->GetScript<CUIScript_Button>()->SetUIPos(5);
	UITestObj->GetScript<CUIScript_Button>()->SetUICase(1);

	m_aInventoryList[1][0].second = UITestObj;

	SpawnGameObject(UITestObj, Vec3(242.5f, -423.f, -1.1f), L"UI");

	UITestObj = ER_ItemMgr::GetInst()->GetItemObj(84)->Clone();
	UITestObj->SetName(L"UI_InvenItemImg11");

	UITestObj->AddComponent(new CTransform);
	UITestObj->AddComponent(new CMeshRender);
	UITestObj->AddComponent(new CUI_Button);
	UITestObj->AddComponent(new CUIScript_Button);

	UITestObj->Transform()->SetRelativeScale(Vec3(68.41463375f, 42.5f, 1.f));

	UITestObj->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	UITestObj->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std2DMtrl"), 0);
	UITestObj->MeshRender()->GetMaterial(0)->SetTexParam(TEX_PARAM::TEX_0, UITestObj->GetScript<ER_DataScript_Item>()->GetItemTex().Get());
	UITestObj->MeshRender()->GetDynamicMaterial(0);

	UITestObj->GetScript<CUIScript_Button>()->SetUIPos(6);
	UITestObj->GetScript<CUIScript_Button>()->SetUICase(1);

	m_aInventoryList[1][1].second = UITestObj;

	SpawnGameObject(UITestObj, Vec3(313.5f, -423.f, -1.1f), L"UI");

	UITestObj = ER_ItemMgr::GetInst()->GetItemObj(84)->Clone();
	UITestObj->SetName(L"UI_InvenItemImg12");

	UITestObj->AddComponent(new CTransform);
	UITestObj->AddComponent(new CMeshRender);
	UITestObj->AddComponent(new CUI_Button);
	UITestObj->AddComponent(new CUIScript_Button);

	UITestObj->Transform()->SetRelativeScale(Vec3(68.41463375f, 42.5f, 1.f));

	UITestObj->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	UITestObj->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std2DMtrl"), 0);
	UITestObj->MeshRender()->GetMaterial(0)->SetTexParam(TEX_PARAM::TEX_0, UITestObj->GetScript<ER_DataScript_Item>()->GetItemTex().Get());
	UITestObj->MeshRender()->GetDynamicMaterial(0);

	UITestObj->GetScript<CUIScript_Button>()->SetUIPos(7);
	UITestObj->GetScript<CUIScript_Button>()->SetUICase(1);

	m_aInventoryList[1][2].second = UITestObj;

	SpawnGameObject(UITestObj, Vec3(384.5f, -423.f, -1.1f), L"UI");

	UITestObj = ER_ItemMgr::GetInst()->GetItemObj(84)->Clone();
	UITestObj->SetName(L"UI_InvenItemImg13");

	UITestObj->AddComponent(new CTransform);
	UITestObj->AddComponent(new CMeshRender);
	UITestObj->AddComponent(new CUI_Button);
	UITestObj->AddComponent(new CUIScript_Button);

	UITestObj->Transform()->SetRelativeScale(Vec3(68.41463375f, 42.5f, 1.f));

	UITestObj->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	UITestObj->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std2DMtrl"), 0);
	UITestObj->MeshRender()->GetMaterial(0)->SetTexParam(TEX_PARAM::TEX_0, UITestObj->GetScript<ER_DataScript_Item>()->GetItemTex().Get());
	UITestObj->MeshRender()->GetDynamicMaterial(0);

	UITestObj->GetScript<CUIScript_Button>()->SetUIPos(8);
	UITestObj->GetScript<CUIScript_Button>()->SetUICase(1);

	m_aInventoryList[1][3].second = UITestObj;

	SpawnGameObject(UITestObj, Vec3(455.5f, -423.f, -1.1f), L"UI");

	UITestObj = ER_ItemMgr::GetInst()->GetItemObj(84)->Clone();
	UITestObj->SetName(L"UI_InvenItemImg14");

	UITestObj->AddComponent(new CTransform);
	UITestObj->AddComponent(new CMeshRender);
	UITestObj->AddComponent(new CUI_Button);
	UITestObj->AddComponent(new CUIScript_Button);

	UITestObj->Transform()->SetRelativeScale(Vec3(68.41463375f, 42.5f, 1.f));

	UITestObj->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	UITestObj->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std2DMtrl"), 0);
	UITestObj->MeshRender()->GetMaterial(0)->SetTexParam(TEX_PARAM::TEX_0, UITestObj->GetScript<ER_DataScript_Item>()->GetItemTex().Get());
	UITestObj->MeshRender()->GetDynamicMaterial(0);

	UITestObj->GetScript<CUIScript_Button>()->SetUIPos(9);
	UITestObj->GetScript<CUIScript_Button>()->SetUICase(1);

	m_aInventoryList[1][4].second = UITestObj;

	SpawnGameObject(UITestObj, Vec3(526.5f, -423.f, -1.1f), L"UI");
#pragma endregion

	for (int i = 0; i < 2; ++i) {
		for (int j = 0; j < 5; ++j) {
			m_aInventoryList[i][j].first->SetEnable(false);
			m_aInventoryList[i][j].second->SetEnable(false);
		}
	}
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

	UITestObj->Transform()->SetRelativeScale(Vec3(58.75f, 36.49621125f, 1.f));

	UITestObj->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	UITestObj->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std2DMtrl"), 0);
	UITestObj->MeshRender()->GetMaterial(0)->SetTexParam(TEX_PARAM::TEX_0, CResMgr::GetInst()->GetInst()->FindRes<CTexture>(L"Ico_ItemGradebg_04.png"));
	UITestObj->MeshRender()->GetDynamicMaterial(0);

	UITestObj->GetScript<CUIScript_Button>()->SetUIPos(0);
	UITestObj->GetScript<CUIScript_Button>()->SetUICase(2);

	m_aEquipList[0][0].first = UITestObj;


	SpawnGameObject(UITestObj, Vec3(-358.f, -361.f, -1.f), L"UI");

	UITestObj = new CGameObject;
	UITestObj->SetName(L"UI_EquipSlot01");

	UITestObj->AddComponent(new CTransform);
	UITestObj->AddComponent(new CMeshRender);
	UITestObj->AddComponent(new CUI_Button);
	UITestObj->AddComponent(new CUIScript_Button);

	UITestObj->Transform()->SetRelativeScale(Vec3(58.75f, 36.49621125f, 1.f));

	UITestObj->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	UITestObj->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std2DMtrl"), 0);
	UITestObj->MeshRender()->GetMaterial(0)->SetTexParam(TEX_PARAM::TEX_0, CResMgr::GetInst()->GetInst()->FindRes<CTexture>(L"Ico_ItemGradebg_04.png"));
	UITestObj->MeshRender()->GetDynamicMaterial(0);

	UITestObj->GetScript<CUIScript_Button>()->SetUIPos(1);
	UITestObj->GetScript<CUIScript_Button>()->SetUICase(2);

	m_aEquipList[0][1].first = UITestObj;

	SpawnGameObject(UITestObj, Vec3(-297.f, -361.f, -1.f), L"UI");


	UITestObj = new CGameObject;
	UITestObj->SetName(L"UI_EquipSlot02");

	UITestObj->AddComponent(new CTransform);
	UITestObj->AddComponent(new CMeshRender);
	UITestObj->AddComponent(new CUI_Button);
	UITestObj->AddComponent(new CUIScript_Button);

	UITestObj->Transform()->SetRelativeScale(Vec3(58.75f, 36.49621125f, 1.f));

	UITestObj->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	UITestObj->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std2DMtrl"), 0);
	UITestObj->MeshRender()->GetMaterial(0)->SetTexParam(TEX_PARAM::TEX_0, CResMgr::GetInst()->GetInst()->FindRes<CTexture>(L"Ico_ItemGradebg_04.png"));
	UITestObj->MeshRender()->GetDynamicMaterial(0);

	UITestObj->GetScript<CUIScript_Button>()->SetUIPos(2);
	UITestObj->GetScript<CUIScript_Button>()->SetUICase(2);

	m_aEquipList[0][2].first = UITestObj;

	SpawnGameObject(UITestObj, Vec3(-236.f, -361.f, -1.f), L"UI");


	UITestObj = new CGameObject;
	UITestObj->SetName(L"UI_EquipSlot10");

	UITestObj->AddComponent(new CTransform);
	UITestObj->AddComponent(new CMeshRender);
	UITestObj->AddComponent(new CUI_Button);
	UITestObj->AddComponent(new CUIScript_Button);

	UITestObj->Transform()->SetRelativeScale(Vec3(58.75f, 36.49621125f, 1.f));

	UITestObj->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	UITestObj->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std2DMtrl"), 0);
	UITestObj->MeshRender()->GetMaterial(0)->SetTexParam(TEX_PARAM::TEX_0, CResMgr::GetInst()->GetInst()->FindRes<CTexture>(L"Ico_ItemGradebg_04.png"));
	UITestObj->MeshRender()->GetDynamicMaterial(0);

	UITestObj->GetScript<CUIScript_Button>()->SetUIPos(3);
	UITestObj->GetScript<CUIScript_Button>()->SetUICase(2);

	m_aEquipList[1][0].first = UITestObj;

	SpawnGameObject(UITestObj, Vec3(-325.f, -411.5f, -1.f), L"UI");


	UITestObj = new CGameObject;
	UITestObj->SetName(L"UI_EquipSlot11");

	UITestObj->AddComponent(new CTransform);
	UITestObj->AddComponent(new CMeshRender);
	UITestObj->AddComponent(new CUI_Button);
	UITestObj->AddComponent(new CUIScript_Button);

	UITestObj->Transform()->SetRelativeScale(Vec3(58.75f, 36.49621125f, 1.f));

	UITestObj->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	UITestObj->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std2DMtrl"), 0);
	UITestObj->MeshRender()->GetMaterial(0)->SetTexParam(TEX_PARAM::TEX_0, CResMgr::GetInst()->GetInst()->FindRes<CTexture>(L"Ico_ItemGradebg_04.png"));
	UITestObj->MeshRender()->GetDynamicMaterial(0);

	UITestObj->GetScript<CUIScript_Button>()->SetUIPos(4);
	UITestObj->GetScript<CUIScript_Button>()->SetUICase(2);

	m_aEquipList[1][1].first = UITestObj;

	SpawnGameObject(UITestObj, Vec3(-264.f, -411.5f, -1.f), L"UI");
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

	UITestObj->Transform()->SetRelativeScale(Vec3(58.75f, 36.49621125f, 1.f));

	UITestObj->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	UITestObj->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std2DMtrl"), 0);
	UITestObj->MeshRender()->GetMaterial(0)->SetTexParam(TEX_PARAM::TEX_0, UITestObj->GetScript<ER_DataScript_Item>()->GetItemTex().Get());
	UITestObj->MeshRender()->GetDynamicMaterial(0);

	UITestObj->GetScript<CUIScript_Button>()->SetUIPos(0);
	UITestObj->GetScript<CUIScript_Button>()->SetUICase(2);

	m_aEquipList[0][0].second = UITestObj;

	SpawnGameObject(UITestObj, Vec3(-358.f, -361.f, -1.f), L"UI");



	UITestObj = ER_ItemMgr::GetInst()->GetItemObj(85)->Clone();
	UITestObj->SetName(L"UI_EquipSlot01");

	UITestObj->AddComponent(new CTransform);
	UITestObj->AddComponent(new CMeshRender);
	UITestObj->AddComponent(new CUI_Button);
	UITestObj->AddComponent(new CUIScript_Button);

	UITestObj->Transform()->SetRelativeScale(Vec3(58.75f, 36.49621125f, 1.f));

	UITestObj->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	UITestObj->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std2DMtrl"), 0);
	UITestObj->MeshRender()->GetMaterial(0)->SetTexParam(TEX_PARAM::TEX_0, UITestObj->GetScript<ER_DataScript_Item>()->GetItemTex().Get());
	UITestObj->MeshRender()->GetDynamicMaterial(0);

	UITestObj->GetScript<CUIScript_Button>()->SetUIPos(1);
	UITestObj->GetScript<CUIScript_Button>()->SetUICase(2);

	m_aEquipList[0][1].second = UITestObj;

	SpawnGameObject(UITestObj, Vec3(-297.f, -361.f, -1.f), L"UI");


	UITestObj = ER_ItemMgr::GetInst()->GetItemObj(85)->Clone();
	UITestObj->SetName(L"UI_EquipSlot02");

	UITestObj->AddComponent(new CTransform);
	UITestObj->AddComponent(new CMeshRender);
	UITestObj->AddComponent(new CUI_Button);
	UITestObj->AddComponent(new CUIScript_Button);

	UITestObj->Transform()->SetRelativeScale(Vec3(58.75f, 36.49621125f, 1.f));

	UITestObj->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	UITestObj->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std2DMtrl"), 0);
	UITestObj->MeshRender()->GetMaterial(0)->SetTexParam(TEX_PARAM::TEX_0, UITestObj->GetScript<ER_DataScript_Item>()->GetItemTex().Get());
	UITestObj->MeshRender()->GetDynamicMaterial(0);

	UITestObj->GetScript<CUIScript_Button>()->SetUIPos(2);
	UITestObj->GetScript<CUIScript_Button>()->SetUICase(2);

	m_aEquipList[0][2].second = UITestObj;

	SpawnGameObject(UITestObj, Vec3(-236.f, -361.f, -1.f), L"UI");


	UITestObj = ER_ItemMgr::GetInst()->GetItemObj(85)->Clone();
	UITestObj->SetName(L"UI_EquipSlot10");
	
	UITestObj->AddComponent(new CTransform);
	UITestObj->AddComponent(new CMeshRender);
	UITestObj->AddComponent(new CUI_Button);
	UITestObj->AddComponent(new CUIScript_Button);
	
	UITestObj->Transform()->SetRelativeScale(Vec3(58.75f, 36.49621125f, 1.f));
	
	UITestObj->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	UITestObj->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std2DMtrl"), 0);
	UITestObj->MeshRender()->GetMaterial(0)->SetTexParam(TEX_PARAM::TEX_0, UITestObj->GetScript<ER_DataScript_Item>()->GetItemTex().Get());
	UITestObj->MeshRender()->GetDynamicMaterial(0);

	UITestObj->GetScript<CUIScript_Button>()->SetUIPos(3);
	UITestObj->GetScript<CUIScript_Button>()->SetUICase(2);

	m_aEquipList[1][0].second = UITestObj;

	SpawnGameObject(UITestObj, Vec3(-325.f, -411.5f, -1.f), L"UI");
	
	
	UITestObj = ER_ItemMgr::GetInst()->GetItemObj(85)->Clone();
	UITestObj->SetName(L"UI_EquipSlot11");
	
	UITestObj->AddComponent(new CTransform);
	UITestObj->AddComponent(new CMeshRender);
	UITestObj->AddComponent(new CUI_Button);
	UITestObj->AddComponent(new CUIScript_Button);
	
	UITestObj->Transform()->SetRelativeScale(Vec3(58.75f, 36.49621125f, 1.f));
	
	UITestObj->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	UITestObj->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std2DMtrl"), 0);
	UITestObj->MeshRender()->GetMaterial(0)->SetTexParam(TEX_PARAM::TEX_0, UITestObj->GetScript<ER_DataScript_Item>()->GetItemTex().Get());
	UITestObj->MeshRender()->GetDynamicMaterial(0);

	UITestObj->GetScript<CUIScript_Button>()->SetUIPos(4);
	UITestObj->GetScript<CUIScript_Button>()->SetUICase(2);

	m_aEquipList[1][1].second = UITestObj;

	SpawnGameObject(UITestObj, Vec3(-264.f, -411.5f, -1.f), L"UI");


	m_aEquipList[0][0].first->SetEnable(false);
	m_aEquipList[0][0].second->SetEnable(false);
	
	m_aEquipList[0][1].first->SetEnable(false);
	m_aEquipList[0][1].second->SetEnable(false);
	
	m_aEquipList[0][2].first->SetEnable(false);
	m_aEquipList[0][2].second->SetEnable(false);
	
	m_aEquipList[1][0].first->SetEnable(false);
	m_aEquipList[1][0].second->SetEnable(false);
	
	m_aEquipList[1][1].first->SetEnable(false);
	m_aEquipList[1][1].second->SetEnable(false);
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
	m_pItemBox->MeshRender()->GetMaterial(0)->SetTexParam(TEX_PARAM::TEX_0, CResMgr::GetInst()->GetInst()->FindRes<CTexture>(L"ItemBox_BackGround_Copy.png"));
	int a = 2;
	m_pItemBox->MeshRender()->GetMaterial(0)->SetScalarParam(INT_2, &a);
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

	dropItem->Transform()->SetRelativeScale(Vec3(53.f, 32.f, 1.f));
	dropItem->Transform()->SetRelativePos(Vec3(-92.f, 8.f, -1.f));

	dropItem->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	dropItem->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std2DMtrl"), 0);
	dropItem->MeshRender()->GetMaterial(0)->SetTexParam(TEX_PARAM::TEX_0, CResMgr::GetInst()->GetInst()->FindRes<CTexture>(L"ItemBg_UnCommon.png"));
	dropItem->MeshRender()->GetDynamicMaterial(0);

	dropItem->GetScript<CUIScript_Button>()->SetUIPos(0);
	dropItem->GetScript<CUIScript_Button>()->SetUICase(0);
	dropItem->GetScript<CUIScript_Button>()->SetBtnCase(1);

	dropItem->Transform()->SetAbsolute(true);

	m_aItemBoxList[0][0].first = dropItem;
	m_pItemBox->AddChild(dropItem);
	
	
	dropItem = new CGameObject();
	dropItem->SetName(L"UI_ItemBoxSlot01");

	dropItem->AddComponent(new CTransform);
	dropItem->AddComponent(new CMeshRender);
	dropItem->AddComponent(new CUI_Button);
	dropItem->AddComponent(new CUIScript_Button);

	dropItem->Transform()->SetRelativeScale(Vec3(53.f, 32.f, 1.f));
	dropItem->Transform()->SetRelativePos(Vec3(-30.f, 8.f, -1.f));

	dropItem->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	dropItem->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std2DMtrl"), 0);
	dropItem->MeshRender()->GetMaterial(0)->SetTexParam(TEX_PARAM::TEX_0, CResMgr::GetInst()->GetInst()->FindRes<CTexture>(L"ItemBg_UnCommon.png"));
	dropItem->MeshRender()->GetDynamicMaterial(0);

	dropItem->GetScript<CUIScript_Button>()->SetUIPos(1);
	dropItem->GetScript<CUIScript_Button>()->SetUICase(0);
	dropItem->GetScript<CUIScript_Button>()->SetBtnCase(1);

	dropItem->Transform()->SetAbsolute(true);

	m_aItemBoxList[0][1].first = dropItem;
	m_pItemBox->AddChild(dropItem);



	dropItem = new CGameObject();
	dropItem->SetName(L"UI_ItemBoxSlot02");

	dropItem->AddComponent(new CTransform);
	dropItem->AddComponent(new CMeshRender);
	dropItem->AddComponent(new CUI_Button);
	dropItem->AddComponent(new CUIScript_Button);

	dropItem->Transform()->SetRelativeScale(Vec3(53.f, 32.f, 1.f));
	dropItem->Transform()->SetRelativePos(Vec3(32.f, 8.f, -1.f));

	dropItem->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	dropItem->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std2DMtrl"), 0);
	dropItem->MeshRender()->GetMaterial(0)->SetTexParam(TEX_PARAM::TEX_0, CResMgr::GetInst()->GetInst()->FindRes<CTexture>(L"ItemBg_UnCommon.png"));
	dropItem->MeshRender()->GetDynamicMaterial(0);

	dropItem->GetScript<CUIScript_Button>()->SetUIPos(2);
	dropItem->GetScript<CUIScript_Button>()->SetUICase(0);
	dropItem->GetScript<CUIScript_Button>()->SetBtnCase(1);

	dropItem->Transform()->SetAbsolute(true);

	m_aItemBoxList[0][2].first = dropItem;
	m_pItemBox->AddChild(dropItem);



	dropItem = new CGameObject();
	dropItem->SetName(L"UI_ItemBoxSlot03");

	dropItem->AddComponent(new CTransform);
	dropItem->AddComponent(new CMeshRender);
	dropItem->AddComponent(new CUI_Button);
	dropItem->AddComponent(new CUIScript_Button);

	dropItem->Transform()->SetRelativeScale(Vec3(53.f, 32.f, 1.f));
	dropItem->Transform()->SetRelativePos(Vec3(94.f, 8.f, -1.f));

	dropItem->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	dropItem->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std2DMtrl"), 0);
	dropItem->MeshRender()->GetMaterial(0)->SetTexParam(TEX_PARAM::TEX_0, CResMgr::GetInst()->GetInst()->FindRes<CTexture>(L"ItemBg_UnCommon.png"));
	dropItem->MeshRender()->GetDynamicMaterial(0);

	dropItem->GetScript<CUIScript_Button>()->SetUIPos(3);
	dropItem->GetScript<CUIScript_Button>()->SetUICase(0);
	dropItem->GetScript<CUIScript_Button>()->SetBtnCase(1);

	dropItem->Transform()->SetAbsolute(true);

	m_aItemBoxList[0][3].first = dropItem;
	m_pItemBox->AddChild(dropItem);


	dropItem = new CGameObject();
	dropItem->SetName(L"UI_ItemBoxSlot10");

	dropItem->AddComponent(new CTransform);
	dropItem->AddComponent(new CMeshRender);
	dropItem->AddComponent(new CUI_Button);
	dropItem->AddComponent(new CUIScript_Button);

	dropItem->Transform()->SetRelativeScale(Vec3(53.f, 32.f, 1.f));
	dropItem->Transform()->SetRelativePos(Vec3(-92.f, -34.f, -1.f));

	dropItem->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	dropItem->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std2DMtrl"), 0);
	dropItem->MeshRender()->GetMaterial(0)->SetTexParam(TEX_PARAM::TEX_0, CResMgr::GetInst()->GetInst()->FindRes<CTexture>(L"ItemBg_UnCommon.png"));
	dropItem->MeshRender()->GetDynamicMaterial(0);

	dropItem->GetScript<CUIScript_Button>()->SetUIPos(4);
	dropItem->GetScript<CUIScript_Button>()->SetUICase(0);
	dropItem->GetScript<CUIScript_Button>()->SetBtnCase(1);

	dropItem->Transform()->SetAbsolute(true);

	m_aItemBoxList[1][0].first = dropItem;
	m_pItemBox->AddChild(dropItem);



	dropItem = new CGameObject();
	dropItem->SetName(L"UI_ItemBoxSlot11");

	dropItem->AddComponent(new CTransform);
	dropItem->AddComponent(new CMeshRender);
	dropItem->AddComponent(new CUI_Button);
	dropItem->AddComponent(new CUIScript_Button);

	dropItem->Transform()->SetRelativeScale(Vec3(53.f, 32.f, 1.f));
	dropItem->Transform()->SetRelativePos(Vec3(-30.f, -34.f, -1.f));

	dropItem->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	dropItem->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std2DMtrl"), 0);
	dropItem->MeshRender()->GetMaterial(0)->SetTexParam(TEX_PARAM::TEX_0, CResMgr::GetInst()->GetInst()->FindRes<CTexture>(L"ItemBg_UnCommon.png"));
	dropItem->MeshRender()->GetDynamicMaterial(0);

	dropItem->GetScript<CUIScript_Button>()->SetUIPos(5);
	dropItem->GetScript<CUIScript_Button>()->SetUICase(0);
	dropItem->GetScript<CUIScript_Button>()->SetBtnCase(1);

	dropItem->Transform()->SetAbsolute(true);

	m_aItemBoxList[1][1].first = dropItem;
	m_pItemBox->AddChild(dropItem);



	dropItem = new CGameObject();
	dropItem->SetName(L"UI_ItemBoxSlot12");

	dropItem->AddComponent(new CTransform);
	dropItem->AddComponent(new CMeshRender);
	dropItem->AddComponent(new CUI_Button);
	dropItem->AddComponent(new CUIScript_Button);

	dropItem->Transform()->SetRelativeScale(Vec3(53.f, 32.f, 1.f));
	dropItem->Transform()->SetRelativePos(Vec3(32.f, -34.f, -1.f));

	dropItem->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	dropItem->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std2DMtrl"), 0);
	dropItem->MeshRender()->GetMaterial(0)->SetTexParam(TEX_PARAM::TEX_0, CResMgr::GetInst()->GetInst()->FindRes<CTexture>(L"ItemBg_UnCommon.png"));
	dropItem->MeshRender()->GetDynamicMaterial(0);

	dropItem->GetScript<CUIScript_Button>()->SetUIPos(6);
	dropItem->GetScript<CUIScript_Button>()->SetUICase(0);
	dropItem->GetScript<CUIScript_Button>()->SetBtnCase(1);

	dropItem->Transform()->SetAbsolute(true);

	m_aItemBoxList[1][2].first = dropItem;
	m_pItemBox->AddChild(dropItem);



	dropItem = new CGameObject();
	dropItem->SetName(L"UI_ItemBoxSlot13");

	dropItem->AddComponent(new CTransform);
	dropItem->AddComponent(new CMeshRender);
	dropItem->AddComponent(new CUI_Button);
	dropItem->AddComponent(new CUIScript_Button);

	dropItem->Transform()->SetRelativeScale(Vec3(53.f, 32.f, 1.f));
	dropItem->Transform()->SetRelativePos(Vec3(94.f, -34.f, -1.f));

	dropItem->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	dropItem->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std2DMtrl"), 0);
	dropItem->MeshRender()->GetMaterial(0)->SetTexParam(TEX_PARAM::TEX_0, CResMgr::GetInst()->GetInst()->FindRes<CTexture>(L"ItemBg_UnCommon.png"));
	dropItem->MeshRender()->GetDynamicMaterial(0);

	dropItem->GetScript<CUIScript_Button>()->SetUIPos(7);
	dropItem->GetScript<CUIScript_Button>()->SetUICase(0);
	dropItem->GetScript<CUIScript_Button>()->SetBtnCase(1);

	dropItem->Transform()->SetAbsolute(true);

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

	dropItem->GetScript<CUIScript_Button>()->SetUIPos(0);
	dropItem->GetScript<CUIScript_Button>()->SetUICase(0);
	dropItem->GetScript<CUIScript_Button>()->SetBtnCase(1);

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

	dropItem->GetScript<CUIScript_Button>()->SetUIPos(1);
	dropItem->GetScript<CUIScript_Button>()->SetUICase(0);
	dropItem->GetScript<CUIScript_Button>()->SetBtnCase(1);

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

	dropItem->GetScript<CUIScript_Button>()->SetUIPos(2);
	dropItem->GetScript<CUIScript_Button>()->SetUICase(0);
	dropItem->GetScript<CUIScript_Button>()->SetBtnCase(1);

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

	dropItem->GetScript<CUIScript_Button>()->SetUIPos(3);
	dropItem->GetScript<CUIScript_Button>()->SetUICase(0);
	dropItem->GetScript<CUIScript_Button>()->SetBtnCase(1);

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

	dropItem->GetScript<CUIScript_Button>()->SetUIPos(4);
	dropItem->GetScript<CUIScript_Button>()->SetUICase(0);
	dropItem->GetScript<CUIScript_Button>()->SetBtnCase(1);

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

	dropItem->GetScript<CUIScript_Button>()->SetUIPos(5);
	dropItem->GetScript<CUIScript_Button>()->SetUICase(0);
	dropItem->GetScript<CUIScript_Button>()->SetBtnCase(1);

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

	dropItem->GetScript<CUIScript_Button>()->SetUIPos(6);
	dropItem->GetScript<CUIScript_Button>()->SetUICase(0);
	dropItem->GetScript<CUIScript_Button>()->SetBtnCase(1);

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

	dropItem->GetScript<CUIScript_Button>()->SetUIPos(7);
	dropItem->GetScript<CUIScript_Button>()->SetUICase(0);
	dropItem->GetScript<CUIScript_Button>()->SetBtnCase(1);

	m_aItemBoxList[1][3].second = dropItem;
	m_pItemBox->AddChild(dropItem);

#pragma endregion

	m_pItemBox->SetEnable(false);
	
	for (int i = 0; i < 2; ++i) {
		for (int j = 0; j < 4; ++j) {
			m_aItemBoxList[i][j].first->SetEnable(false);
			m_aItemBoxList[i][j].second->SetEnable(false);
		}
	}
}

void ER_UIMgr::CreateStatText()
{
	// Text Obj
	CGameObject* testTextObj = new CGameObject;
	AddComponents(testTextObj, _TRANSFORM | _MESHRENDER | _TEXT);
	testTextObj->SetName(L"UI_stat00");

	// 텍스트 출력 필수요소 : _TRANSFORM | _MESHRENDER | _TEXT
	// Std2DUIMtrl 사용
	testTextObj->Transform()->SetRelativeScale(Vec3(10.f, 10.f, 0.f));
	testTextObj->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	testTextObj->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std2DUIMtrl"), 0);

	// 텍스쳐없어도되지만 텍스쳐지정 안하면 마젠타색상출력돼서 쉐이더코드처리필요
	testTextObj->MeshRender()->GetMaterial(0)->SetTexParam(TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"Empty_Text.png"));


	// 폰트 넥슨Lv2고딕 과, FW1_CENTER | FW1_VCENTER Flags는 기본값으로설정해놓음.
	// 예시 : 폰트 패밀리이름(파일이름아님), OffsetPos, FontSize, RGBA값, 폰트출력 Flag.
	testTextObj->Text()->TextInit(L"넥슨Lv2고딕", Vec2(0.f, 0.f), 15.f, FONT_RGBA(255, 255, 255, 255), FW1_CENTER | FW1_VCENTER);
	testTextObj->Text()->InputString(L"테스트");
	testTextObj->Text()->SetName(L"테스트00");
	
	m_pStatText[0][0] = testTextObj;

	SpawnGameObject(testTextObj, Vec3(-655.f, -342.5f, 0.f), L"UI");


	testTextObj = new CGameObject;
	AddComponents(testTextObj, _TRANSFORM | _MESHRENDER | _TEXT);
	testTextObj->SetName(L"UI_stat01");

	// 텍스트 출력 필수요소 : _TRANSFORM | _MESHRENDER | _TEXT
	// Std2DUIMtrl 사용
	testTextObj->Transform()->SetRelativeScale(Vec3(10.f, 10.f, 0.f));
	testTextObj->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	testTextObj->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std2DUIMtrl"), 0);

	// 텍스쳐없어도되지만 텍스쳐지정 안하면 마젠타색상출력돼서 쉐이더코드처리필요
	testTextObj->MeshRender()->GetMaterial(0)->SetTexParam(TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"Empty_Text.png"));


	// 폰트 넥슨Lv2고딕 과, FW1_CENTER | FW1_VCENTER Flags는 기본값으로설정해놓음.
	// 예시 : 폰트 패밀리이름(파일이름아님), OffsetPos, FontSize, RGBA값, 폰트출력 Flag.
	testTextObj->Text()->TextInit(L"넥슨Lv2고딕", Vec2(0.f, 0.f), 15.f, FONT_RGBA(255, 255, 255, 255), FW1_CENTER | FW1_VCENTER);
	testTextObj->Text()->InputString(L"테스트");

	m_pStatText[0][1] = testTextObj;

	SpawnGameObject(testTextObj, Vec3(-567.f, -342.5f, 0.f), L"UI");


	testTextObj = new CGameObject;
	AddComponents(testTextObj, _TRANSFORM | _MESHRENDER | _TEXT);
	testTextObj->SetName(L"UI_stat10");

	// 텍스트 출력 필수요소 : _TRANSFORM | _MESHRENDER | _TEXT
	// Std2DUIMtrl 사용
	testTextObj->Transform()->SetRelativeScale(Vec3(10.f, 10.f, 0.f));
	testTextObj->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	testTextObj->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std2DUIMtrl"), 0);

	// 텍스쳐없어도되지만 텍스쳐지정 안하면 마젠타색상출력돼서 쉐이더코드처리필요
	testTextObj->MeshRender()->GetMaterial(0)->SetTexParam(TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"Empty_Text.png"));


	// 폰트 넥슨Lv2고딕 과, FW1_CENTER | FW1_VCENTER Flags는 기본값으로설정해놓음.
	// 예시 : 폰트 패밀리이름(파일이름아님), OffsetPos, FontSize, RGBA값, 폰트출력 Flag.
	testTextObj->Text()->TextInit(L"넥슨Lv2고딕", Vec2(0.f, 0.f), 15.f, FONT_RGBA(255, 255, 255, 255), FW1_CENTER | FW1_VCENTER);
	testTextObj->Text()->InputString(L"테스트");

	m_pStatText[1][0] = testTextObj;

	SpawnGameObject(testTextObj, Vec3(-655.f, -372.f, 0.f), L"UI");


	testTextObj = new CGameObject;
	AddComponents(testTextObj, _TRANSFORM | _MESHRENDER | _TEXT);
	testTextObj->SetName(L"UI_stat11");

	// 텍스트 출력 필수요소 : _TRANSFORM | _MESHRENDER | _TEXT
	// Std2DUIMtrl 사용
	testTextObj->Transform()->SetRelativeScale(Vec3(10.f, 10.f, 0.f));
	testTextObj->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	testTextObj->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std2DUIMtrl"), 0);

	// 텍스쳐없어도되지만 텍스쳐지정 안하면 마젠타색상출력돼서 쉐이더코드처리필요
	testTextObj->MeshRender()->GetMaterial(0)->SetTexParam(TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"Empty_Text.png"));


	// 폰트 넥슨Lv2고딕 과, FW1_CENTER | FW1_VCENTER Flags는 기본값으로설정해놓음.
	// 예시 : 폰트 패밀리이름(파일이름아님), OffsetPos, FontSize, RGBA값, 폰트출력 Flag.
	testTextObj->Text()->TextInit(L"넥슨Lv2고딕", Vec2(0.f, 0.f), 15.f, FONT_RGBA(255, 255, 255, 255), FW1_CENTER | FW1_VCENTER);
	testTextObj->Text()->InputString(L"테스트");

	m_pStatText[1][1] = testTextObj;

	SpawnGameObject(testTextObj, Vec3(-567.f, -372.f, 0.f), L"UI");

	testTextObj = new CGameObject;
	AddComponents(testTextObj, _TRANSFORM | _MESHRENDER | _TEXT);
	testTextObj->SetName(L"UI_stat20");

	// 텍스트 출력 필수요소 : _TRANSFORM | _MESHRENDER | _TEXT
	// Std2DUIMtrl 사용
	testTextObj->Transform()->SetRelativeScale(Vec3(10.f, 10.f, 0.f));
	testTextObj->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	testTextObj->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std2DUIMtrl"), 0);

	// 텍스쳐없어도되지만 텍스쳐지정 안하면 마젠타색상출력돼서 쉐이더코드처리필요
	testTextObj->MeshRender()->GetMaterial(0)->SetTexParam(TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"Empty_Text.png"));


	// 폰트 넥슨Lv2고딕 과, FW1_CENTER | FW1_VCENTER Flags는 기본값으로설정해놓음.
	// 예시 : 폰트 패밀리이름(파일이름아님), OffsetPos, FontSize, RGBA값, 폰트출력 Flag.
	testTextObj->Text()->TextInit(L"넥슨Lv2고딕", Vec2(0.f, 0.f), 15.f, FONT_RGBA(255, 255, 255, 255), FW1_CENTER | FW1_VCENTER);
	testTextObj->Text()->InputString(L"테스트");

	m_pStatText[2][0] = testTextObj;

	SpawnGameObject(testTextObj, Vec3(-655.f, -401.5f, 0.f), L"UI");


	testTextObj = new CGameObject;
	AddComponents(testTextObj, _TRANSFORM | _MESHRENDER | _TEXT);
	testTextObj->SetName(L"UI_stat21");

	// 텍스트 출력 필수요소 : _TRANSFORM | _MESHRENDER | _TEXT
	// Std2DUIMtrl 사용
	testTextObj->Transform()->SetRelativeScale(Vec3(10.f, 10.f, 0.f));
	testTextObj->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	testTextObj->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std2DUIMtrl"), 0);

	// 텍스쳐없어도되지만 텍스쳐지정 안하면 마젠타색상출력돼서 쉐이더코드처리필요
	testTextObj->MeshRender()->GetMaterial(0)->SetTexParam(TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"Empty_Text.png"));


	// 폰트 넥슨Lv2고딕 과, FW1_CENTER | FW1_VCENTER Flags는 기본값으로설정해놓음.
	// 예시 : 폰트 패밀리이름(파일이름아님), OffsetPos, FontSize, RGBA값, 폰트출력 Flag.
	testTextObj->Text()->TextInit(L"넥슨Lv2고딕", Vec2(0.f, 0.f), 15.f, FONT_RGBA(255, 255, 255, 255), FW1_CENTER | FW1_VCENTER);
	testTextObj->Text()->InputString(L"테스트");

	m_pStatText[2][1] = testTextObj;

	SpawnGameObject(testTextObj, Vec3(-567.f, -401.5f, 0.f), L"UI");

	testTextObj = new CGameObject;
	AddComponents(testTextObj, _TRANSFORM | _MESHRENDER | _TEXT);
	testTextObj->SetName(L"UI_stat30");

	// 텍스트 출력 필수요소 : _TRANSFORM | _MESHRENDER | _TEXT
	// Std2DUIMtrl 사용
	testTextObj->Transform()->SetRelativeScale(Vec3(10.f, 10.f, 0.f));
	testTextObj->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	testTextObj->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std2DUIMtrl"), 0);

	// 텍스쳐없어도되지만 텍스쳐지정 안하면 마젠타색상출력돼서 쉐이더코드처리필요
	testTextObj->MeshRender()->GetMaterial(0)->SetTexParam(TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"Empty_Text.png"));


	// 폰트 넥슨Lv2고딕 과, FW1_CENTER | FW1_VCENTER Flags는 기본값으로설정해놓음.
	// 예시 : 폰트 패밀리이름(파일이름아님), OffsetPos, FontSize, RGBA값, 폰트출력 Flag.
	testTextObj->Text()->TextInit(L"넥슨Lv2고딕", Vec2(0.f, 0.f), 15.f, FONT_RGBA(255, 255, 255, 255), FW1_CENTER | FW1_VCENTER);
	testTextObj->Text()->InputString(L"테스트");

	m_pStatText[3][0] = testTextObj;

	SpawnGameObject(testTextObj, Vec3(-655.f, -431.f, 0.f), L"UI");


	testTextObj = new CGameObject;
	AddComponents(testTextObj, _TRANSFORM | _MESHRENDER | _TEXT);
	testTextObj->SetName(L"UI_stat31");

	// 텍스트 출력 필수요소 : _TRANSFORM | _MESHRENDER | _TEXT
	// Std2DUIMtrl 사용
	testTextObj->Transform()->SetRelativeScale(Vec3(10.f, 10.f, 0.f));
	testTextObj->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	testTextObj->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std2DUIMtrl"), 0);

	// 텍스쳐없어도되지만 텍스쳐지정 안하면 마젠타색상출력돼서 쉐이더코드처리필요
	testTextObj->MeshRender()->GetMaterial(0)->SetTexParam(TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"Empty_Text.png"));


	// 폰트 넥슨Lv2고딕 과, FW1_CENTER | FW1_VCENTER Flags는 기본값으로설정해놓음.
	// 예시 : 폰트 패밀리이름(파일이름아님), OffsetPos, FontSize, RGBA값, 폰트출력 Flag.
	testTextObj->Text()->TextInit(L"넥슨Lv2고딕", Vec2(0.f, 0.f), 15.f, FONT_RGBA(255, 255, 255, 255), FW1_CENTER | FW1_VCENTER);
	testTextObj->Text()->InputString(L"테스트");

	m_pStatText[3][1] = testTextObj;

	SpawnGameObject(testTextObj, Vec3(-567.f, -431.f, 0.f), L"UI");


}

void ER_UIMgr::UpdateStat()
{
	ER_Ingame_Stats* status = ER_GameSystem::GetInst()->GetPlayerCharacter()->GetScript<ER_DataScript_Character>()->GetStatus();

	m_pStatText[0][0]->Text()->InputString(std::to_wstring(status->iAttackPower));			//공격력
	m_pStatText[0][1]->Text()->InputString(std::to_wstring(status->iDefense));				//방어력
	m_pStatText[1][0]->Text()->InputString(std::to_wstring(status->fAttackSpeed));			//공격속도
	m_pStatText[1][1]->Text()->InputString(std::to_wstring(status->fMovementSpeed));		//이동속도
	m_pStatText[2][0]->Text()->InputString(std::to_wstring(status->fCriticalStrikeChance));	//치명타 확률
	m_pStatText[2][1]->Text()->InputString(std::to_wstring(status->fVisionRange));			//시야
	m_pStatText[3][0]->Text()->InputString(std::to_wstring(status->fHPRegen));				//체력 재생
	m_pStatText[3][1]->Text()->InputString(std::to_wstring(status->fSPRegen));				//스태미너 재생
}

void ER_UIMgr::UpdateHP()
{
	ER_Ingame_Stats* stat = ER_GameSystem::GetInst()->GetPlayerCharacter()->GetScript<ER_DataScript_Character>()->GetStatus();
	float maxHP = stat->iMaxHP;
	float curHP = stat->iHP;

	//흰 바 먼저 초기화
	m_pHPReturnBar->Transform()->SetRelativePos(m_pHPBar->Transform()->GetRelativePos());
	m_pHPReturnBar->Transform()->SetRelativeScale(m_pHPBar->Transform()->GetRelativeScale());
		
	//실제 체력바 계산
	float decreaseRate = ((curHP) / maxHP) * 375.f;

	Vec3 pos = m_pHPBar->Transform()->GetRelativePos();
	pos.x = 1.f;
	pos.x = pos.x - ((375-decreaseRate) / 2);
	Vec3 scale = m_pHPBar->Transform()->GetRelativeScale();
	scale.x = decreaseRate;

	m_pHPBar->Transform()->SetRelativePos(pos);
	m_pHPBar->Transform()->SetRelativeScale(scale);

	m_bHPChangeTrigger = true;
}

void ER_UIMgr::UpdateHPReturnBar()
{
	Vec3 pos = m_pHPReturnBar->Transform()->GetRelativePos();
	Vec3 scale = m_pHPReturnBar->Transform()->GetRelativeScale();

	if (scale.x > m_pHPBar->Transform()->GetRelativeScale().x) {
		float decreaseRate = (75.f * DT);
		scale.x -= decreaseRate;
		pos.x -= (decreaseRate / 2);
		m_pHPReturnBar->Transform()->SetRelativePos(pos);
		m_pHPReturnBar->Transform()->SetRelativeScale(scale);
	}
	else {
		m_bHPChangeTrigger = false;
	}
}

void ER_UIMgr::UpdateSteminar()
{
	ER_Ingame_Stats* stat = ER_GameSystem::GetInst()->GetPlayerCharacter()->GetScript<ER_DataScript_Character>()->GetStatus();
	float maxSR = stat->iMaxSP;
	float curSR = stat->iSP;

	float decreaseRate = ((curSR) / maxSR) * 375.f;

	Vec3 pos = m_pStemnarBar->Transform()->GetRelativePos();
	pos.x = 1.f;
	pos.x = pos.x - ((375 - decreaseRate) / 2);
	Vec3 scale = m_pStemnarBar->Transform()->GetRelativeScale();
	scale.x = decreaseRate;

	m_pStemnarBar->Transform()->SetRelativePos(pos);
	m_pStemnarBar->Transform()->SetRelativeScale(scale);
}

HBITMAP ER_UIMgr::LoadPNGAsBitmap(LPCTSTR szFilename)
{
	Gdiplus::Bitmap* pImage = Gdiplus::Bitmap::FromFile(szFilename, FALSE);
	HBITMAP hBmp = NULL;

	if (pImage && pImage->GetLastStatus() == Gdiplus::Ok)
	{
		pImage->GetHBITMAP(Gdiplus::Color(0, 0, 0, 0), &hBmp);
	}

	delete pImage;
	return hBmp;
}

HCURSOR ER_UIMgr::BitmapToCursor(HWND hWnd, HBITMAP hBitmap)
{
	ICONINFO iconInfo = {};
	iconInfo.fIcon = FALSE; // 커서로 사용하기 위해 FALSE로 설정
	iconInfo.xHotspot = 0;  // 커서의 핫스팟 X 좌표
	iconInfo.yHotspot = 0;  // 커서의 핫스팟 Y 좌표
	iconInfo.hbmMask = hBitmap;  // 마스크 비트맵
	iconInfo.hbmColor = hBitmap; // 컬러 비트맵

	HCURSOR hCursor = CreateIconIndirect(&iconInfo);
	return hCursor;
}

Vec3 ER_UIMgr::WorldPosToUIPos(Vec3 worldPos)
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
	
	result.y = -result.y;
	
	result.z = 0.f;
	
	return result;
}

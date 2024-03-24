#include "pch.h"
#include "ER_UIMgr.h"

#include <Engine\components.h>
#include <Engine\CResMgr.h>

#include "ER_UIScript_SkillSlot.h"
#include "ER_UIScript_SkillUpBtn.h"
#include "ER_UIScript_StatusBarGauge.h""

ER_UIMgr::ER_UIMgr()
	: StatusBar_Center(nullptr)
	, StatusBar_CharacterInfo(nullptr)
	, StatusBar_Inventory(nullptr)
	, StatusBar_SkillSlot{}
	, StatusBar_SkillLevelUpBtn{}
	, StatusBar_Gauge{}
{
}

ER_UIMgr::~ER_UIMgr()
{
}

void ER_UIMgr::LobbyUI()
{
}

void ER_UIMgr::InGameUI()
{
	// [ 스폰 오브젝트 ]
	// 1. 중앙보드
	// 2. 스킬 레벨업 버튼 4개
}

void ER_UIMgr::CreateCenter()
{
	// [ 중앙 UI 보드 생성 ]
	StatusBar_Center = new CGameObject;
	StatusBar_Center->SetName(L"UI_StatusBar_Center");
	AddComponents(StatusBar_Center, _TRANSFORM | _MESHRENDER);
	
	StatusBar_Center->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	StatusBar_Center->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"2DUISTD_Mtrl"), 0);
	StatusBar_Center->MeshRender()->GetDynamicMaterial(0);

	StatusBar_Center->MeshRender()->GetMaterial(0)->SetTexParam(TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"Img_MainBgCenter.png"));
	StatusBar_Center->Transform()->LoadPrefab(L"UI_StatusBar_Center");

	for (int i = 0; i < 4; ++i)
	{
		// [ 스킬레벨업 외부 스킬슬롯 별도 생성 ]
		StatusBar_SkillLevelUpBtn[i] = new CGameObject;
		StatusBar_SkillLevelUpBtn[i]->AddComponent(new ER_UIScript_SkillUpBtn);
		StatusBar_SkillLevelUpBtn[i]->GetScript<ER_UIScript_SkillUpBtn>()->SetUI(i);
		
		// [ 중앙보드 내부 스킬슬롯 UI 생성 ]
		StatusBar_SkillSlot[i] = new CGameObject;
		StatusBar_SkillSlot[i]->AddComponent(new ER_UIScript_SkillSlot);
		StatusBar_SkillSlot[i]->GetScript<ER_UIScript_SkillSlot>()->SetUI(i, StatusBar_SkillLevelUpBtn[i]);
		
		// 스크립트 추가해주고 인자로 세팅함수 넣어주기
		StatusBar_Center->AddChild(StatusBar_SkillSlot[i]);
	}

	// [ 스테이터스바 게이지 UI 생성 ]
	for (int i = 0; i < 2; ++i)
	{
		StatusBar_Gauge[i] = new CGameObject;
		StatusBar_Gauge[i]->AddComponent(new ER_UIScript_StatusBarGauge);
		StatusBar_Gauge[i]->GetScript<ER_UIScript_StatusBarGauge>()->SetUI(i);

		StatusBar_Center->AddChild(StatusBar_Gauge[i]);
	}

}

void ER_UIMgr::CreateCharacterInfo()
{
	StatusBar_CharacterInfo = new CGameObject;

	// [ 캐릭터정보 UI 보드 생성 ]
	StatusBar_CharacterInfo = new CGameObject;
	StatusBar_CharacterInfo->SetName(L"UI_StatusBar_CharacterInfo");
	AddComponents(StatusBar_CharacterInfo, _TRANSFORM | _MESHRENDER);

	StatusBar_CharacterInfo->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	StatusBar_CharacterInfo->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"2DUISTD_Mtrl"), 0);
	StatusBar_CharacterInfo->MeshRender()->GetDynamicMaterial(0);

	StatusBar_CharacterInfo->MeshRender()->GetMaterial(0)->SetTexParam(TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"Img_MainBg_02.png"));
	// StatusBar_Center->Transform()->LoadPrefab(L"UI_StatusBar_Center");
}

void ER_UIMgr::CreateInventory()
{
	StatusBar_Inventory = new CGameObject;

	// [ 인벤토리 UI 보드 생성 ]
	StatusBar_Inventory = new CGameObject;
	StatusBar_Inventory->SetName(L"UI_StatusBar_Inventory");
	AddComponents(StatusBar_Inventory, _TRANSFORM | _MESHRENDER);

	StatusBar_Inventory->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	StatusBar_Inventory->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"2DUISTD_Mtrl"), 0);
	StatusBar_Inventory->MeshRender()->GetDynamicMaterial(0);

	StatusBar_Inventory->MeshRender()->GetMaterial(0)->SetTexParam(TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"Img_MainBg_04.png"));
}

void ER_UIMgr::init()
{
	// 스테이터스 UI 생성
	CreateCenter();
	CreateCharacterInfo();
	CreateInventory();
}

void ER_UIMgr::tick()
{
}

void ER_UIMgr::SpawnUI()
{
	SpawnGameObject(StatusBar_Center, L"UI");
	SpawnGameObject(StatusBar_CharacterInfo, L"UI");
	SpawnGameObject(StatusBar_Inventory, L"UI");

	for (auto Btn : StatusBar_SkillLevelUpBtn)
		SpawnGameObject(Btn, L"UI");
}

void ER_UIMgr::RegistPlayerCharacetr()
{
	// StatusBar UI에 정보 넣어주기
	for (int i = 0; i < 4; ++i)
	{
		StatusBar_SkillSlot[i]->GetScript<ER_UIScript_SkillSlot>()->RegistPlayerCharacetr();
	}

	// guage
	for (int i = 0; i < 2; ++i)
	{
		StatusBar_Gauge[i]->GetScript<ER_UIScript_StatusBarGauge>()->RegistPlayerCharacetr();
	}
}

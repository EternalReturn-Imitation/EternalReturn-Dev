#include "pch.h"
#include "ER_UIMgr.h"

#include <Engine\components.h>
#include <Engine\CResMgr.h>

#include "ER_UIScript_SkillSlot.h"

ER_UIMgr::ER_UIMgr()
	: StatusBar_Center(nullptr)
	, StatusBar_CharacterInfo(nullptr)
	, StatusBar_Inventory(nullptr)
	, StatusBar_SkillLevelUpBtn{}
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
}

void ER_UIMgr::CreateCenter()
{
	// [ 중앙 UI 보드 생성 ]
	StatusBar_Center = new CGameObject;
	StatusBar_Center->SetName(L"UI_StatusBar_Center");
	AddComponents(StatusBar_Center, _TRANSFORM | _MESHRENDER);
	
	StatusBar_Center->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	StatusBar_Center->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std2DUIMtrl"), 0);
	StatusBar_Center->MeshRender()->GetDynamicMaterial(0);

	StatusBar_Center->MeshRender()->GetMaterial(0)->SetTexParam(TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"Img_MainBg_03.png"));
	StatusBar_Center->Transform()->LoadPrefab(L"UI_StatusBar_Center");

	// 스킬슬롯 UI 추가
	for (int i = 0; i < 4; ++i)
	{
		StatusBar_SkillSlot[i] = new CGameObject;
		StatusBar_SkillSlot[i]->AddComponent(new ER_UIScript_SkillSlot);
		StatusBar_SkillSlot[i]->GetScript< ER_UIScript_SkillSlot>()->SetUI(i);
		
		// StatusBar_SkillLevelUpBtn[i] = new CGameObject;
		
		// 스크립트 추가해주고 인자로 세팅함수 넣어주기


		StatusBar_Center->AddChild(StatusBar_SkillSlot[i]);
	}




	
}

void ER_UIMgr::CreateCharacterInfo()
{
	StatusBar_CharacterInfo = new CGameObject;
}

void ER_UIMgr::CreateInventory()
{
	StatusBar_Inventory = new CGameObject;
}

void ER_UIMgr::CreateSkillUpBtn()
{
	for (int i = 0; i < 4; ++i)
		StatusBar_SkillLevelUpBtn[i] = new CGameObject;
}

void ER_UIMgr::init()
{
	// 스테이터스 UI 생성
	CreateCenter();
}

void ER_UIMgr::tick()
{
}

void ER_UIMgr::SpawnUI()
{
	SpawnGameObject(StatusBar_Center, L"UI");
}

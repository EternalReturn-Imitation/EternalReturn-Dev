#include "pch.h"
#include "ER_ItemMgr.h"

#include "ImGuiMgr.h"
#include "ItemDataUI.h"

ER_ItemMgr::ER_ItemMgr()
{
}

ER_ItemMgr::~ER_ItemMgr()
{
	Safe_Del_Vec(m_vecItem);
}

void ER_ItemMgr::init()
{
	// 아이템 데이터 로딩
	Load();

	// 레시피 업데이트
	RecipeUpdate();
	((ItemDataUI*)ImGuiMgr::GetInst()->FindUI("##ItemDataUI"))->RegistItemMgr();
}
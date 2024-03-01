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
	Load();
	((ItemDataUI*)ImGuiMgr::GetInst()->FindUI("##ItemDataUI"))->RegistItemMgr();
}

void ER_ItemMgr::Update()
{
}

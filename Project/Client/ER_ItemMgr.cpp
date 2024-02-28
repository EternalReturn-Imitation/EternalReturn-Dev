#include "pch.h"
#include "ER_ItemMgr.h"

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
}

void ER_ItemMgr::Update()
{
}

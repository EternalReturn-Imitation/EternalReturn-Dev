#include "pch.h"
#include "ER_DataScript_Item.h"

ER_DataScript_Item::ER_DataScript_Item()
	: CScript((UINT)SCRIPT_TYPE::ER_DATASCRIPT_ITEM)
	, m_eItemCode()
	, m_eGrade()
	, m_eType()
	, m_eSlot()
	, m_uniRecipe{}
	, m_tItemStats{}
	, m_bEquiped(false)
{
}

ER_DataScript_Item::~ER_DataScript_Item()
{
}
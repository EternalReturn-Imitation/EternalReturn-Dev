#include "pch.h"
#include "ER_Data_ItemScript.h"

ER_Data_ItemScript::ER_Data_ItemScript()
	: CScript((UINT)SCRIPT_TYPE::ER_DATA_ITEMSCRIPT)
	, m_eItemCode()
	, m_eGrade()
	, m_eType()
	, m_eSlot()
	, m_uniRecipe{}
	, m_tItemStats{}
{
}

ER_Data_ItemScript::~ER_Data_ItemScript()
{
}
#include "pch.h"
#include "ER_Item.h"

#include <Engine\CPathMgr.h>


ER_Item::ER_Item()
	: m_eItemCode()
	, m_eGrade()
	, m_eType()
	, m_eSlot()
	, m_uniRecipe{}
	, m_tItemStats{}
{
}

ER_Item::~ER_Item()
{
}
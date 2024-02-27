#include "pch.h"
#include "ER_ItemMgr.h"

#include <Engine\CPathMgr.h>

extern const wchar_t* ER_ITEM_WSTR[(UINT)ER_ITEM_CODE::END] =
{
	L"°¡À§",
	L"°¡Á×",
	L"°ÅºÏÀÌ µîµüÁö",
	L"°í¹«",
	L"°íÃ¶",
	L"°î±ªÀÌ",
	L"±êÅÐ",
	L"²É",
	L"³ª¹µ°¡Áö",
	L"³ì½¼ °Ë",
	L"´ÜºÀ",
	L"µ¹¸ÍÀÌ",
	L"¶óÀÌÅÍ",
	L"¸®º»",
	L"¸¶ÆÐ",
	L"¸¸³âÇÊ",
	L"¸ÁÄ¡",
	L"¸Ó¸®¶ì",
	L"¸ñÀå°©",
	L"¸ø",
	L"¹ßÅÍ PPK",
	L"¹èÅÍ¸®",
	L"ºØ´ë",
	L"¼º¹è",
	L"¼ÅÃ÷",
	L"¼Õ¸ñ½Ã°è",
	L"¼è»ç½½",
	L"½ÊÀÚ°¡",
	L"¾ç±Ã",
	L"¿ÀÀÏ",
	L"¿Ê°¨",
	L"¿îµ¿È­",
	L"¿ø¼®",
	L"À¯¸®º´",
	L"ÀÚÀü°Å Çï¸ä",
	L"Á¢ÂøÁ¦",
	L"Á¾ÀÌ",
	L"Åº»ê¼ö",
	L"Æ®·³ÇÁ Ä«µå",
	L"ÆÈÂî",
	L"ÇÇ¾Æ³ë¼±",
	L"È­¾à",
	L"°¡Á× ¹æÆÐ",
	L"°íÀå³­ ½Ã°è",
	L"±ºº¹",
	L"±â¸§¸ÔÀÎ Ãµ",
	L"µ¡´í ½½¸®ÆÛ",
	L"·çºñ",
	L"¸Å±×³Ñ - ÆÄÀÌ¼±",
	L"¸Ó¸®Å×",
	L"¸ÕÁöÅÐÀÌ°³",
	L"ºÎÃ÷",
	L"ºñÆÄ´Üµµ",
	L"»ç½½ ³´",
	L"»çÁ¦º¹",
	L"Àå°Ë",
	L"¼ºÀÚÀÇ À¯»ê",
	L"¾ÆÀÌ¾ð ³ÊÅ¬",
	L"¾ÈÀü¸ð",
	L"¿î¸íÀÇ ²É",
	L"À¯¸® Á¶°¢",
	L"ÀÌ¿Â ÀüÁö",
	L"Àá¼öº¹",
	L"Àå±Ã",
	L"Àç",
	L"ÀüÀÚ ºÎÇ°",
	L"Á¤±³ÇÑ µµ¸é",
	L"Ã¶ÆÇ",
	L"ÇÏÀÌÈú",
	L"È²±Ý",
	L"°­Ã¶ ¹æÆÐ",
	L"°ÇÆ²¸´",
	L"±ÝÆÈÂî",
	L"¸Å±×³Ñ - ¾Æ³ªÄÜ´Ù",
	L"¸ðÅÍ",
	L"¹æÅº¸ð",
	L"¹æÅºÁ¶³¢",
	L"º®·Â±Ã",
	L"º¸°Ë",
	L"»ç½ÅÀÇ³´",
	L"¾ÆÀÌÅ×¸£ ±êÅÐ",
	L"À¯¸®ÆÇ",
	L"ÀüÅõÈ­",
	L"Á¤Àå",
	L"Æ¼¾Æ¶ó",
	L"ÆÇ±Ý °©¿Ê",
	L"°æ·®È­ ºÎÃ÷",
	L"±¤ÇÐ¹ÌÃ¤ ½´Æ®",
	L"·Îºó",
	L"¸Å±×³Ñ - º¸¾Æ",
	L"¹ÙÀÌÅ» ¼¾¼­",
	L"Áý»çº¹",
	L"ºÎÄÉÆÈ·Î½º",
	L"ºê·¹ÀÌÁú °ÇÆ²¸´",
	L"¼º±â»çÀÇ °©¿Ê",
	L"¼öÁ¤ Æ¼¾Æ¶ó",
	L"¿¢½ºÄ®¸®¹ö",
	L"¿ÀÅä¹ÙÀÌ Çï¸ä",
	L"Àü¼ú - ops Çï¸ä",
	L"Á¦¹èÀÇ È°",
	L"Á¦»çÀåÀÇ ¿¹º¹",
	L"Å¬¸µ¿Â ºÎÃ÷",
	L"Åº´Þ·Î½ºÀÇ ÆÈÂî",
	L"Ç³È­·û",
	L"ÇÃ¶óÁî¸¶ ¾ÆÅ©",
	L"ÇÏ¸£Æä",
	L"EOD ½´Æ®"
};

void ER_ItemMgr::RegistItem()
{
	ER_Item* item = new ER_Item();
	item->m_strItemName = L"Àå±Ã";
	item->m_ItemIcon = nullptr;
	item->m_eItemCode = 0;
	item->m_eGrade = 2;
	item->m_eType = 2;
	item->m_eSlot = 2;
	item->m_tItemStats = {};

	m_vecItem.push_back(item);

	item = new ER_Item();
	item->m_strItemName = L"¿¢½ºÄ®¸®¹ö";
	item->m_ItemIcon = nullptr;
	item->m_eItemCode = 1;
	item->m_eGrade = 1;
	item->m_eType = 1;
	item->m_eSlot = 1;
	item->m_tItemStats = {};

	m_vecItem.push_back(item);

	item = new ER_Item();
	item->m_strItemName = L"Á¦¹èÀÇ È°";
	item->m_ItemIcon = nullptr;
	item->m_eItemCode = 2;
	item->m_eGrade = 3;
	item->m_eType = 3;
	item->m_eSlot = 3;
	item->m_tItemStats = {};

	m_vecItem.push_back(item);
}

void ER_ItemMgr::RegistRecipe()
{

}

int ER_ItemMgr::Save()
{
	path path_content = CPathMgr::GetInst()->GetContentPath();
	path path_item = path_content.wstring() + L"Data";

	if (false == exists(path_item))
	{
		create_directory(path_item);
	}

	wstring strFilePath = CPathMgr::GetInst()->GetContentPath();
	strFilePath += L"Data\\Item.data";

	FILE* pFile = nullptr;
	_wfopen_s(&pFile, strFilePath.c_str(), L"wb");

	// Item Cnt
	size_t iItemCnt = m_vecItem.size();
	fwrite(&iItemCnt, sizeof(size_t), 1, pFile);

	for (size_t i = 0; i < iItemCnt; ++i)
	{
		SaveItemData(m_vecItem[i], pFile);
	}
	
	fclose(pFile);

	return S_OK;
}

int ER_ItemMgr::Load()
{
	wstring strPath = CPathMgr::GetInst()->GetContentPath();
	strPath += L"Data\\Item.data";

	FILE* pFile = nullptr;
	_wfopen_s(&pFile, strPath.c_str(), L"rb");

	if (nullptr == pFile)
		return S_FALSE;

	size_t iItemCnt = 0;
	fread(&iItemCnt, sizeof(size_t), 1, pFile);

	m_vecItem.resize(iItemCnt);

	// Item Data Load
	for (size_t i = 0; i < iItemCnt; ++i)
	{
		m_vecItem[i] = (LoadItemData(pFile));
		
		// recipe°¡ ÀÖ´Â °æ¿ì
		if (0 != m_vecItem[(UINT)i]->m_uniRecipe.recipe)
			AddRecipe(m_vecItem[(UINT)i]->m_uniRecipe.recipe, (UINT)i);
	}
	
	fclose(pFile);

	return S_OK;
}

int ER_ItemMgr::SaveItemData(ER_Item* _Item, FILE* _File)
{
	// ItemName
	SaveWString(_Item->m_strItemName, _File);

	// Texture
	SaveResRef(_Item->m_ItemIcon.Get(), _File);

	// ItemCode
	fwrite(&_Item->m_eItemCode, sizeof(UINT), 1, _File);

	// ItemGrade
	fwrite(&_Item->m_eGrade, sizeof(UINT), 1, _File);

	// ItemType
	fwrite(&_Item->m_eType, sizeof(UINT), 1, _File);

	// ItemSlot
	fwrite(&_Item->m_eSlot, sizeof(UINT), 1, _File);

	// ItemRecipe
	fwrite(&_Item->m_uniRecipe, sizeof(DWORD_PTR), 1, _File);

	// ItemStat
	fwrite(&_Item->m_tItemStats, sizeof(ER_tStats), 1, _File);

	return 0;
}

ER_Item* ER_ItemMgr::LoadItemData(FILE* _File)
{
	ER_Item* pItem = new ER_Item;

	// Item Name
	wstring strName;
	LoadWString(strName, _File);
	pItem->SetItemName(strName.c_str());

	// Texture
	LoadResRef(pItem->m_ItemIcon, _File);

	// ItemCode
	fread(&pItem->m_eItemCode, sizeof(UINT), 1, _File);

	// ItemGrade
	fread(&pItem->m_eGrade, sizeof(UINT), 1, _File);

	// ItemType
	fread(&pItem->m_eType, sizeof(UINT), 1, _File);

	// ItemSlot
	fread(&pItem->m_eSlot, sizeof(UINT), 1, _File);

	// Item Recipe
	fread(&pItem->m_uniRecipe, sizeof(DWORD_PTR), 1, _File);

	// ItemStat
	fread(&pItem->m_tItemStats, sizeof(ER_tStats), 1, _File);

	return pItem;
}

int ER_ItemMgr::AddRecipe(DWORD_PTR _recipe, UINT _ItemCode)
{
	m_umapRecipe.insert(make_pair(_recipe, _ItemCode));
	
	return 0;
}

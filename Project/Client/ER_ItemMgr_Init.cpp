#include "pch.h"
#include "ER_ItemMgr.h"

#include <Engine\CPathMgr.h>

#include <Engine\CResMgr.h>
#include <Engine\CTexture.h>

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
		
		// recipe가 있는 경우
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
	if ((UINT)ER_ITEM_GRADE::COMMON == _Item->m_eGrade)
	{	
		DWORD_PTR tmp = 0;
		fwrite(&tmp, sizeof(DWORD_PTR), 1, _File);
	}
	else
		fwrite(&_Item->m_uniRecipe, sizeof(DWORD_PTR), 1, _File);

	// ItemStat
	if ((UINT)ER_ITEM_SLOT::NONE == _Item->m_eSlot && (UINT)ER_ITEM_TYPE::CONSUMABLES != _Item->m_eType)
	{
		ER_tStats tmp = {};
		fwrite(&tmp, sizeof(ER_tStats), 1, _File);
	}
	else
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

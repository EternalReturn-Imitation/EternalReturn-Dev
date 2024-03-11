#include "pch.h"
#include "ER_ItemMgr.h"

#include <Engine\CPathMgr.h>

#include <Engine\CResMgr.h>
#include <Engine\CTexture.h>

#include <Script\ER_Data_ItemScript.h>

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
	}
	
	fclose(pFile);

	return S_OK;
}

int ER_ItemMgr::SaveItemData(CGameObject* _Item, FILE* _File)
{
	ER_Data_ItemScript* ItemContext = _Item->GetScript<ER_Data_ItemScript>();
	// ItemName
	SaveWString(ItemContext->m_strItemName, _File);

	// Texture
	SaveResRef(ItemContext->m_ItemIcon.Get(), _File);

	// ItemCode
	fwrite(&ItemContext->m_eItemCode, sizeof(UINT), 1, _File);

	// ItemGrade
	fwrite(&ItemContext->m_eGrade, sizeof(UINT), 1, _File);

	// ItemType
	fwrite(&ItemContext->m_eType, sizeof(UINT), 1, _File);

	// ItemSlot
	fwrite(&ItemContext->m_eSlot, sizeof(UINT), 1, _File);

	// Weapontype
	fwrite(&ItemContext->m_eWeapon_type, sizeof(UINT), 1, _File);

	// ItemRecipe
	if ((UINT)ER_ITEM_GRADE::COMMON == ItemContext->m_eGrade)
	{	
		DWORD_PTR tmp = 0;
		fwrite(&tmp, sizeof(DWORD_PTR), 1, _File);
	}
	else
		fwrite(&ItemContext->m_uniRecipe, sizeof(DWORD_PTR), 1, _File);

	// ItemStat
	if ((UINT)ER_ITEM_SLOT::NONE == ItemContext->m_eSlot && (UINT)ER_ITEM_TYPE::CONSUMABLES != ItemContext->m_eType)
	{
		ER_ItemStats tmp = {};
		fwrite(&tmp, sizeof(ER_ItemStats), 1, _File);
	}
	else
		fwrite(&ItemContext->m_tItemStats, sizeof(ER_ItemStats), 1, _File);

	return 0;
}

CGameObject* ER_ItemMgr::LoadItemData(FILE* _File)
{
	CGameObject* pItem = new CGameObject;
	pItem->AddComponent(new ER_Data_ItemScript());

	ER_Data_ItemScript* ItemContext = pItem->GetScript<ER_Data_ItemScript>();

	// Item Name
	wstring strName;
	LoadWString(strName, _File);
	ItemContext->SetItemName(strName.c_str());

	// Texture
	LoadResRef(ItemContext->m_ItemIcon, _File);

	// ItemCode
	fread(&ItemContext->m_eItemCode, sizeof(UINT), 1, _File);

	// ItemGrade
	fread(&ItemContext->m_eGrade, sizeof(UINT), 1, _File);

	// ItemType
	fread(&ItemContext->m_eType, sizeof(UINT), 1, _File);

	// ItemSlot
	fread(&ItemContext->m_eSlot, sizeof(UINT), 1, _File);

	// Weapontype
	fread(&ItemContext->m_eWeapon_type, sizeof(UINT), 1, _File);

	// Item Recipe
	fread(&ItemContext->m_uniRecipe, sizeof(DWORD_PTR), 1, _File);

	// ItemStat
	fread(&ItemContext->m_tItemStats, sizeof(ER_ItemStats), 1, _File);

	return pItem;
}

void ER_ItemMgr::RecipeUpdate()
{
	// 기존 레시피 컨테이너 클리어
	if(!m_umapRecipe.empty())
		m_umapRecipe.clear();
	
	UINT iItemCnt = (UINT)m_vecItem.size();

	for (UINT i = 0; i < iItemCnt; ++i)
	{
		ER_Data_ItemScript* ItemContext = m_vecItem[i]->GetScript<ER_Data_ItemScript>();

		if (0 != ItemContext->m_uniRecipe.recipe)
			m_umapRecipe.insert(make_pair(ItemContext->m_uniRecipe.recipe, i));
	};
}

int ER_ItemMgr::SearchRecipe(UINT _ingr_1, UINT _ingr_2, int& _res)
{
	// DWORD_PTR 생성
	UINT Litem = _ingr_1;
	UINT Ritem = _ingr_2;
	
	if (Ritem < Litem)
	{
		Litem = _ingr_2;
		Ritem = _ingr_1;
	}
	else if (Litem == Ritem)
		return S_FALSE;

	ER_RECIPE res = {};
	res.ingredient_1 = Litem;
	res.ingredient_2 = Ritem;

	unordered_map<DWORD_PTR, UINT>::iterator iter = m_umapRecipe.find(res.recipe);
	
	if (m_umapRecipe.end() == iter)
		return S_FALSE;

	_res = iter->second;

	return S_OK;
}

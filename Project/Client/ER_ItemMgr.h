#pragma once
#include <Engine/CSingleton.h>
#include "ER_Item.h"

class ER_ItemMgr
	: public CSingleton<ER_ItemMgr>
{
	SINGLE(ER_ItemMgr);

private:
	vector<ER_Item*> m_vecItem;						// 아이템 리스트
	unordered_map<DWORD_PTR, UINT> m_umapRecipe;	// 조합식 목록

private:
	int Save();
	int Load();

	int SaveItemData(ER_Item* _Item, FILE* _File);
	ER_Item* LoadItemData(FILE* _File);

	int AddRecipe(DWORD_PTR _recipe, UINT _ItemCode);

public:
	void init();
	void Update();

	friend class ItemDataUI;

};
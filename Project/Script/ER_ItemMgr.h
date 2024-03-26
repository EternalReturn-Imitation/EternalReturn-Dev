#pragma once
#include <Engine/CSingleton.h>

class CGameObject;

class ER_ItemMgr
	: public CSingleton<ER_ItemMgr>
{
	SINGLE(ER_ItemMgr);

private:
	vector<CGameObject*> m_vecItem;						// 아이템 리스트
	unordered_map<DWORD_PTR, UINT> m_umapRecipe;		// 조합식 목록

private:
	int Save();
	int Load();

	int SaveItemData(CGameObject* _Item, FILE* _File);
	CGameObject* LoadItemData(FILE* _File);

	void RecipeUpdate();

	void random();

public:
	int SearchRecipe(UINT _ingr_1, UINT _ingr_2, int& _res);
	vector<CGameObject*> GetItemObjs() { return m_vecItem; }
	CGameObject* GetItemObj(int _i) { return m_vecItem[_i]; }

public:
	void init();

	friend class ItemDataUI;
};
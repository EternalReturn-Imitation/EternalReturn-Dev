#pragma once
#include <Engine/CSingleton.h>
#include "ER_define.h"	

class CGameObject;
class ER_DataScript_Character;

class ER_ItemMgr
	: public CSingleton<ER_ItemMgr>
{
	SINGLE(ER_ItemMgr);

private:
	vector<CGameObject*> m_vecItem;												// 아이템 리스트
	unordered_map<DWORD_PTR, UINT> m_umapRecipe;								// 조합식 목록
	unordered_map<UINT, DWORD_PTR> m_umapIngredient;							// 재료 목록

	vector<CGameObject*> m_vecItemBox[(UINT)LUMIAISLAND::END];					// 현재 레벨에 있는 아이템 박스 리스트
	

private:
	int Save();
	int Load();

	int SaveItemData(CGameObject* _Item, FILE* _File);
	CGameObject* LoadItemData(FILE* _File);

	void RecipeUpdate();

	void random();

	void GetIngredient(UINT _ItemID, std::queue<UINT>* _IngredientQueue);
	void GetIngredient(UINT _ItemID, unordered_map<UINT, int>* _Ingrediendmap);

public:
	int SearchRecipe(UINT _ingr_1, UINT _ingr_2, int& _res);
	vector<CGameObject*> GetItemObjs() { return m_vecItem; }
	CGameObject* GetItemObj(int _i) { return m_vecItem[_i]; }

	void SpawnItemLevel();

	void RegistItemBox(CGameObject* _pItemBox,LUMIAISLAND _areaName) { m_vecItemBox[(UINT)_areaName].push_back(_pItemBox); }
	
public:
	void init();

	friend class ER_DataScript_Character;
	friend class ItemDataUI;
};
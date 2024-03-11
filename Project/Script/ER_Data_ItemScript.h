#pragma once
#include <Engine/CScript.h>
#include "ER_define.h"
#include "ER_struct.h"

class ER_Data_ItemScript :
    public CScript
{
private:
	wstring			m_strItemName;	// 아이템 이름
	Ptr<CTexture>	m_ItemIcon;		// 아이콘 텍스쳐

	UINT			m_eItemCode;	// 아이템 번호
	UINT			m_eGrade;		// 아이템 등급
	UINT			m_eType;		// 아이템 타입
	UINT			m_eSlot;		// 아이템 장착슬롯
	UINT			m_eWeapon_type;	// 무기 타입

	ER_RECIPE		m_uniRecipe;	// 재료 아이템

	ER_ItemStats	m_tItemStats;	// 아이템 스탯

private:
	void SetItemName(const wchar_t* _strName) { m_strItemName = _strName; }
	void SetItemName(wstring _strName) { m_strItemName = _strName; }
	void SetItemTex(Ptr<CTexture> _pTex) { m_ItemIcon = _pTex; }
	void SetItemInfo(UINT _code, UINT _grade, UINT _type, UINT _slot)
	{
		m_eItemCode = _code;
		m_eGrade = _grade;
		m_eType = _type;
		m_eSlot = _slot;
	}
	void SetItemCode(UINT _code) { m_eItemCode = _code; }
	void SetItemGrade(UINT _grade) { m_eGrade = _grade; }
	void SetItemType(UINT _type) { m_eType = _type; }
	void SetItemSlot(UINT _slot) { m_eSlot = _slot; }
	void SetWPType(UINT _WPType) { m_eWeapon_type = _WPType; }
	void SetItemIngr_1(UINT _ingr_1) { m_uniRecipe.ingredient_1 = _ingr_1; }
	void SetItemIngr_2(UINT _ingr_2) { m_uniRecipe.ingredient_2 = _ingr_2; }
	void SetItemRecipe(ER_RECIPE _recipe) { m_uniRecipe = _recipe; }

	void GetRecipe(ER_RECIPE _recipe) { m_uniRecipe = _recipe; }
	void SetItemStats(ER_ItemStats& _stats) { m_tItemStats = _stats; }

public:
	const wstring& GetItemName() { return m_strItemName; }
	Ptr<CTexture> GetItemTex() { return m_ItemIcon; }

	UINT GetCode() { return (UINT)m_eItemCode; }
	UINT GetGrade() { return (UINT)m_eGrade; }
	UINT GetType() { return (UINT)m_eType; }
	UINT GetSlot() { return (UINT)m_eSlot; }
	UINT GetWPType() { return (UINT)m_eWeapon_type; }

	ER_RECIPE GetRecipe() { return m_uniRecipe; }

	const ER_ItemStats& GetStats() { return m_tItemStats; }

	CLONE(ER_Data_ItemScript);
public:
	ER_Data_ItemScript();
	~ER_Data_ItemScript();

	friend class ER_ItemMgr;
	friend class ItemDataUI;
};


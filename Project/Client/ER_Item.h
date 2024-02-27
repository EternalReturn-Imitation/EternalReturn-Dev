#pragma once
#include <Engine\ptr.h>
#include <Engine\CTexture.h>

struct ER_tStats
{
	short iAttackPower;					// 공격력
	short iAttackPowerPerLevel;			// 레벨당 공격력
	short iSkillAmplification;			// 스킬 증폭
	short iSkillAmplificationPerLevel;	// 레벨당 스킬 증폭
	short iMaxHP;						// 체력
	short iMaxHPPerLevel;				// 레벨당 체력
	float fHPRegen;						// 체력 재생
	float fHPRegenPerLevel;				// 레벨당 체력 재생
	short iMaxSP;						// 스테미너
	short iMaxSPPerLevel;				// 레벨당 스테미너
	float fSPRegen;						// 스테미나 재생
	float fSPRegenPerLevel;				// 레벨당 스테미너 재생
	short iDefense;						// 방어력
	float fAttackSpeed;					// 공격 속도 %
	float fCriticalStrikeChance;		// 치명타 확률
	float fCriticalStrikeDamage;		// 치명타 추가 데미지
	float fMovementSpeed;				// 이동 속도
	float fVisionRange;					// 시야
	float fCooldownReduction;			// 쿨타임 감소
	float fOmnisyphon;					// 모든 피해 흡혈
	short padding;						// 2byte 패딩
};

class ER_Item
{
private:
	wstring			m_strItemName;	// 아이템 이름
	Ptr<CTexture>	m_ItemIcon;		// 아이콘 텍스쳐
	
	UINT			m_eItemCode;	// 아이템 번호
	UINT			m_eGrade;		// 아이템 등급
	UINT			m_eType;		// 아이템 타입
	UINT			m_eSlot;		// 아이템 장착슬롯

	ER_RECIPE		m_uniRecipe;	// 재료 아이템
	
	ER_tStats		m_tItemStats;	// 아이템 스탯

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

	void GetRecipe(ER_RECIPE _recipe) { m_uniRecipe = _recipe; }
	void SetItemStats(ER_tStats& _stats) { m_tItemStats = _stats; }

public:
	const wstring& GetItemName() { return m_strItemName; }
	Ptr<CTexture> GetItemTex() { return m_ItemIcon; }

	UINT GetCode() { return (UINT)m_eItemCode; }
	UINT GetGrade() { return (UINT)m_eGrade; }
	UINT GetType() { return (UINT)m_eType; }
	UINT GetSlot() { return (UINT)m_eSlot; }

	ER_RECIPE GetRecipe() { return m_uniRecipe; }

	const ER_tStats& GetStats() { return m_tItemStats; }

public:
	ER_Item();
	~ER_Item();

	friend class ER_ItemMgr;
	friend class ItemDataUI;
};
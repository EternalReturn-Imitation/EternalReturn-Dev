#pragma once

#define STATEDELEGATE_ENTER(obj, CharacterName, func) obj->SetStateEnter((SCRIPT_DELEGATE)&##ER_ActionScript_##CharacterName::func##Enter)
#define STATEDELEGATE_UPDATE(obj, CharacterName, func) obj->SetStateUpdate((SCRIPT_DELEGATE)&ER_ActionScript_##CharacterName::func##Update)
#define STATEDELEGATE_EXIT(obj, CharacterName, func) obj->SetStateExit((SCRIPT_DELEGATE)&##ER_ActionScript_##CharacterName::func##Exit)

#define STATEDATA_SET(State, FSMData) StateList[ER_CHAR_ACT::State]->SetData(FSMData)
#define STATEDATA_GET(State) StateList[ER_CHAR_ACT::State]->GetData()

#define BATTLE_SKILL(AttackObj, HittedObj, className, CalcFunc, SkillInfo) ER_BattleSystem::GetInst()->Battle_Skill(AttackObj, HittedObj, this, (SKILL_DMG_CALC)&className::CalcFunc, SkillInfo)
#define BATTLE_COMMON(AttackObj, HittedObj) Battle_Common(AttackObj, HittedObj)


enum class ER_ITEM_GRADE
{
	COMMON,		// ÀÏ¹Ý
	UNCOMMONE,	// °í±Þ
	RARE,		// Èñ±Í
	EPIC,		// ¿µ¿õ
};

enum class ER_ITEM_TYPE
{
	EQUIPMENT,			// Àåºñ
	CONSUMABLES,		// ¼Òºñ
	INGREDIENT,			// Àç·á
};

enum class ER_ITEM_SLOT
{
	WEAPONS,			// ¹«±â
	CHEST,				// ¿Ê
	HEAD,				// ¸Ó¸®
	ARM,				// ÆÈ
	LEG,				// ´Ù¸®
	END,				// ÀåÂøºÒ°¡
};

enum class ER_WEAPON_TYPE
{
	AXE,				// µµ³¢
	SWORD,				// °Ë
	PISTOL,				// ±ÇÃÑ
	GLOVE,				// ±Û·¯ºê
	BOW,				// È°
	END,				// ¹Ì¼³Á¤
};

typedef union item_combination_pair
{
	struct
	{
		UINT ingredient_1;
		UINT ingredient_2;
	};

	DWORD_PTR recipe;
}ER_RECIPE;

typedef struct item_combination_slot
{
	UINT iSlot1;
	UINT iSlot2;
	UINT iAssemblyItem;
}ER_CMB_SLOT;

enum eExpForLevelUp
{
	LEVEL1 = 0,
	LEVEL2 = 0,
	LEVEL3 = 0,
	LEVEL4 = 0,
	LEVEL5 = 0,
	LEVEL6 = 0,
	LEVEL7 = 0,
	LEVEL8 = 0,
	LEVEL9 = 0,
	LEVEL10 = 0,
	LEVEL11 = 0,
	LEVEL12 = 0,
	LEVEL13 = 0,
	LEVEL14 = 0,
	LEVEL15 = 0,
	LEVEL16 = 0,
	LEVEL17 = 0,
	LEVEL18 = 0,
	LEVEL19 = 0,
	LEVEL20 = 0
};

enum eSkillindex
{
	Q_1,
	W_1,
	E_1,
	R_1,
	Q_2,
	W_2,
	E_2,
	R_2,
	SKILLMAXSIZE,
}typedef SKILLIDX;
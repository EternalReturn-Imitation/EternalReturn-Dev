#pragma once

#define STATEDELEGATE_ENTER(obj, className, func) obj->SetStateEnter((SCRIPT_DELEGATE)&className::func##Enter)
#define STATEDELEGATE_UPDATE(obj, className, func) obj->SetStateUpdate((SCRIPT_DELEGATE)&className::func##Update)
#define STATEDELEGATE_EXIT(obj, className, func) obj->SetStateExit((SCRIPT_DELEGATE)&className::func##Exit)

#define STATEDATA_SET(State, FSMData) StateList[ER_CHAR_ACT::State]->SetData(FSMData)
#define STATEDATA_GET(State) StateList[ER_CHAR_ACT::State]->GetData()

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
	NONE,				// ÀåÂøºÒ°¡
	WEAPONS,			// ¹«±â
	HEAD,				// ¸Ó¸®
	CHEST,				// ¿Ê
	ARM,				// ÆÈ
	LEG,				// ´Ù¸®
};

enum class ER_WEAPON_TYPE
{
	NONE,				// ¹Ì¼³Á¤
	AXE,				// µµ³¢
	SWORD,				// °Ë
	PISTOL,				// ±ÇÃÑ
	GLOVE,				// ±Û·¯ºê
	BOW					// È°
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
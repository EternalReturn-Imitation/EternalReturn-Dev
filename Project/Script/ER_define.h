#pragma once

#define STATEDELEGATE_ENTER(obj, className, func) obj->SetStateEnter((SCRIPT_DELEGATE)&className::func##Enter)
#define STATEDELEGATE_UPDATE(obj, className, func) obj->SetStateUpdate((SCRIPT_DELEGATE)&className::func##Update)
#define STATEDELEGATE_EXIT(obj, className, func) obj->SetStateExit((SCRIPT_DELEGATE)&className::func##Exit)

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
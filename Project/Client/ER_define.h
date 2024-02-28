#pragma once

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


typedef union item_combination_pair
{
	struct
	{
		UINT ingredient_1;
		UINT ingredient_2;
	};

	DWORD_PTR recipe;
}ER_RECIPE;
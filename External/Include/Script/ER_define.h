#pragma once

#define STATEDELEGATE_ENTER(obj, className, func) obj->SetStateEnter((SCRIPT_DELEGATE)&className::func##Enter)
#define STATEDELEGATE_UPDATE(obj, className, func) obj->SetStateUpdate((SCRIPT_DELEGATE)&className::func##Update)
#define STATEDELEGATE_EXIT(obj, className, func) obj->SetStateExit((SCRIPT_DELEGATE)&className::func##Exit)

#define STATEDATA_SET(State, FSMData) StateList[ER_CHAR_ACT::State]->SetData(FSMData)

enum class ER_ITEM_GRADE
{
	COMMON,		// 일반
	UNCOMMONE,	// 고급
	RARE,		// 희귀
	EPIC,		// 영웅
};

enum class ER_ITEM_TYPE
{
	EQUIPMENT,			// 장비
	CONSUMABLES,		// 소비
	INGREDIENT,			// 재료
};

enum class ER_ITEM_SLOT
{
	NONE,				// 장착불가
	WEAPONS,			// 무기
	HEAD,				// 머리
	CHEST,				// 옷
	ARM,				// 팔
	LEG,				// 다리
};

enum class ER_WEAPON_TYPE
{
	NONE,				// 미설정
	AXE,				// 도끼
	SWORD,				// 검
	PISTOL,				// 권총
	GLOVE,				// 글러브
	BOW					// 활
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
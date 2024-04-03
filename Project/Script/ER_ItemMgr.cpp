#include "pch.h"
#include "ER_ItemMgr.h"

ER_ItemMgr::ER_ItemMgr()
{
}

ER_ItemMgr::~ER_ItemMgr()
{
	Safe_Del_Vec(m_vecItem);
}

void ER_ItemMgr::init()
{
	// 아이템 데이터 로딩
	Load();

	// 레시피 업데이트
	RecipeUpdate();
}

void ER_ItemMgr::random()
{
	// 난수 생성기 초기화
	std::random_device rd;  // 하드웨어 난수 생성기를 사용하여 시드 생성
	std::mt19937 gen(rd()); // 시드 값으로 난수 생성 엔진 초기화
	std::uniform_int_distribution<> dis01(0, 41); // 0과 41을 포함하는 범위에서 정수형 난수 생성
	std::uniform_int_distribution<> dis02(1, 4); // 0과 41을 포함하는 범위에서 정수형 난수 생성

	// 랜덤한 숫자 생성 및 출력
	int random_number01 = dis01(gen);
	int random_number02 = dis02(gen);
}

void ER_ItemMgr::GetIngredient(UINT _ItemID, std::queue<UINT>* _IngredientQueue)
{
	// 조합아이템인지 검사
	// 조합아이템이 아닌 경우
	if (m_umapIngredient.end() == m_umapIngredient.find(_ItemID))
	{
		(*_IngredientQueue).push(_ItemID);
		return;
	}

	ER_RECIPE recipe = {};
	recipe.recipe = m_umapIngredient.find(_ItemID)->second;

	// 재료아이템으로 검사 진행
	GetIngredient(recipe.ingredient_1, _IngredientQueue);
	GetIngredient(recipe.ingredient_2, _IngredientQueue);
}

void ER_ItemMgr::GetIngredient(UINT _ItemID, unordered_map<UINT, int>* _Ingrediendmap)
{
	// 말단 재료는 제작재료 목록에 넣지 않는다.
	if (m_umapIngredient.end() == m_umapIngredient.find(_ItemID))
		return;

	// 필요 아이템 목록에 있는아이템인지 검사
	if (_Ingrediendmap->end() != _Ingrediendmap->find(_ItemID))
	{
		// 있는 아이템인경우 갯수를 늘려준다.
		_Ingrediendmap->find(_ItemID)->second++;
	}
	else
	{
		// 신규로 넣어준다
		_Ingrediendmap->insert(make_pair(_ItemID, 1));
	}
	

	ER_RECIPE recipe = {};
	recipe.recipe = m_umapIngredient.find(_ItemID)->second;

	// 재료아이템으로 검사 진행
	GetIngredient(recipe.ingredient_1, _Ingrediendmap);
	GetIngredient(recipe.ingredient_2, _Ingrediendmap);
}

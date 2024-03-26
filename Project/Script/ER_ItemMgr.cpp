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

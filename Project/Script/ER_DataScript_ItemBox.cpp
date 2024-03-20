#include "pch.h"
#include "ER_DataScript_ItemBox.h"

#include <Engine\CResMgr.h>
#include <Engine\CTexture.h>
#include <Engine\define.h>
#include "ER_ItemMgr.h"

ER_DataScript_ItemBox::ER_DataScript_ItemBox()
	: CScript((UINT)SCRIPT_TYPE::ER_DATASCRIPT_ITEMBOX)
{
}

ER_DataScript_ItemBox::~ER_DataScript_ItemBox()
{
}

void ER_DataScript_ItemBox::init()
{
}

void ER_DataScript_ItemBox::begin()
{
	if (GetOwner()->GetRenderComponent() != nullptr && GetOwner()->GetRenderComponent()->GetMaterial(0) != nullptr)
	{
		int a = 1;
		GetOwner()->GetRenderComponent()->GetMaterial(0)->SetScalarParam(INT_3, &a);
	}

	CreateRandomItems();
}

void ER_DataScript_ItemBox::tick()
{
}

void ER_DataScript_ItemBox::BeginOverlap(CCollider3D* _Other)
{
}

void ER_DataScript_ItemBox::OnOverlap(CCollider3D* _Other)
{
}

void ER_DataScript_ItemBox::EndOverlap(CCollider3D* _Other)
{
}

void ER_DataScript_ItemBox::BeginRayOverlap()
{
	int a = 2;
	if (GetOwner()->GetRenderComponent() != nullptr && GetOwner()->GetRenderComponent()->GetMaterial(0) != nullptr)
	{
		GetOwner()->GetRenderComponent()->GetMaterial(0)->SetScalarParam(INT_3, &a);
	}
}

void ER_DataScript_ItemBox::OnRayOverlap()
{
}

void ER_DataScript_ItemBox::EndRayOverlap()
{	
	if (GetOwner()->GetRenderComponent() != nullptr && GetOwner()->GetRenderComponent()->GetMaterial(0) != nullptr)
	{
		int a = 1;
		GetOwner()->GetRenderComponent()->GetMaterial(0)->SetScalarParam(INT_3, &a);
	}
}

void ER_DataScript_ItemBox::CreateRandomItems()
{
	// 난수 생성기 초기화
	std::random_device rd;  // 하드웨어 난수 생성기를 사용하여 시드 생성
	std::mt19937 gen(rd()); // 시드 값으로 난수 생성 엔진 초기화
	std::uniform_int_distribution<> dis01(0, 41); // 0과 41을 포함하는 범위에서 정수형 난수 생성
	std::uniform_int_distribution<> dis02(1, 4); // 0과 41을 포함하는 범위에서 정수형 난수 생성

	// 랜덤한 숫자 생성 및 출력
	int random_number01 = dis01(gen);
	int random_number02 = dis02(gen);

	for (int i = 0; i < 2; ++i) {
		for (int j = 0; j < 4; ++j) {
			if (i == 1 && j > random_number02)
				break;

			m_aItemList[i][j] = ER_ItemMgr::GetInst()->GetItemObj(random_number01);

			random_number01 = dis01(gen);
		}
	}
}

#include "pch.h"
#include "ER_DataScript_Character.h"
#include "ER_DataScript_Item.h"
#include "ER_struct.h"
#include <FontEngine\FW1FontWrapper.h>
#include "ER_DataScript_Item.h"
#include "ER_ItemMgr.h"
#include "ER_GameSystem.h"
#include "ER_PlayerScript.h"

ER_DataScript_Character::ER_DataScript_Character()
	: CScript((UINT)SCRIPT_TYPE::ER_DATASCRIPT_CHARACTER)
	, m_Stats(nullptr)
	, m_StatusEffect(nullptr)
	, m_bGameDead(false)
	, m_bOutofContorl(false)
	, m_Equipment{}
	, m_Inventory{}
	, m_fSPRegenTime(0.f)
	, m_SkillPoint(0)
	, m_STDStats{}
	, m_RootItem{}
	, bCoolDownCheat(false)
	, bInvincibleCheat(false)
{
	m_Stats = new tIngame_Stats;
	m_StatusEffect = new tStatus_Effect;
}

ER_DataScript_Character::ER_DataScript_Character(const ER_DataScript_Character& _origin)
	: m_Stats(nullptr)
	, m_StatusEffect(nullptr)
	, m_strKey(_origin.m_strKey)
	, m_strName(_origin.m_strName)
	, m_STDStats(_origin.m_STDStats)
	, m_PortraitTex(_origin.m_PortraitTex)
	, m_FullTax(_origin.m_FullTax)
	, m_MapTex(_origin.m_MapTex)
	, m_bGameDead(false)
	, m_bOutofContorl(false)
	, m_Equipment{}
	, m_Inventory{}
	, CScript((UINT)SCRIPT_TYPE::ER_DATASCRIPT_CHARACTER)
	, m_fSPRegenTime(0.f)
	, m_SkillPoint(0)
	, m_RootItem{}
	, bCoolDownCheat(false)
	, bInvincibleCheat(false)
{
	m_Stats = new tIngame_Stats;
	m_StatusEffect = new tStatus_Effect;
	m_LevelUpSound = CResMgr::GetInst()->FindRes<CSound>(L"effect_levelup.wav").Get();

	for (int i = 0; i < _origin.m_SkillList.size(); ++i)
	{
		tSkill_Info* tmp = new tSkill_Info;
		*tmp = *_origin.m_SkillList[i];

		m_SkillList.push_back(tmp);
	}

	for (int i = 0; i < 5; ++i)
	{
		m_RootItem[i] = _origin.m_RootItem[i];
	}

	m_IngredientList = _origin.m_IngredientList;
	m_NeedFarmingItems = _origin.m_NeedFarmingItems;
}

ER_DataScript_Character::~ER_DataScript_Character()
{
	if (m_Stats)
		delete m_Stats;

	if (m_StatusEffect)
		delete m_StatusEffect;

	Safe_Del_Vec(m_SkillList);
	Safe_Del_Array(m_Equipment);
	Safe_Del_Array(m_Inventory);

	// 인벤토리, 장착 아이템 delete 작업
}

void ER_DataScript_Character::StatusUpdate()
{
	ER_Ingame_Stats Updatetmp = *m_Stats;
	UINT Level = Updatetmp.iLevel - 1;

	// 레벨,경험치,HP,SP 는 갱신하지 않는다.
	// 1. 기본 스테이터스
	Updatetmp.iAttackPower = m_STDStats.iAttackPower + (Level * m_STDStats.iAttackPowerPerLevel);
	Updatetmp.iDefense = m_STDStats.iDefense + (Level * m_STDStats.iDefensePerLevel);
	Updatetmp.iMaxHP = m_STDStats.iMaxHP + (Level * m_STDStats.iMaxHPPerLevel);
	Updatetmp.fHPRegen = m_STDStats.fHPRegen + (Level * m_STDStats.fHPRegenPerLevel);
	Updatetmp.iMaxSP = m_STDStats.iMaxSP + (Level * m_STDStats.iMaxSPPerLevel);
	Updatetmp.fSPRegen = m_STDStats.fSPRegen + (Level * m_STDStats.fSPRegenPerLevel);
	Updatetmp.fAttackSpeed = m_STDStats.fAttackSpeed + m_STDStats.fWpAtkSpd;
	Updatetmp.fCriticalStrikeChance = m_STDStats.fCriticalStrikeChance;
	Updatetmp.fMovementSpeed = m_STDStats.fMovementSpeed;
	Updatetmp.fVisionRange = m_STDStats.fVisionRange;
	Updatetmp.iSkillAmplification = m_STDStats.iAttackPower + (Level * m_STDStats.iAttackPowerPerLevel);
	Updatetmp.fAtkRange = m_STDStats.fWpAtkRange;
	Updatetmp.fCriticalStrikeDamage = 0;
	Updatetmp.fCooldownReduction = 0;
	Updatetmp.fOmnisyphon = 0;
	Updatetmp.iSkillAmplification = 0;

	// 2. 아이템 추가 스테이터스
	for (int i = 0; i < (UINT)ER_ITEM_SLOT::END; ++i)
	{
		// 아이템이 장착되어있지 않다
		if (m_Equipment[i] == nullptr)
			continue;

		// 아이템 정보를 얻어와 업데이트
		tItem_Stats Itemtmp = GETITEMSTATS(m_Equipment[i]);

		Updatetmp.iAttackPower += Itemtmp.iAttackPower + (Level * Itemtmp.iAttackPowerPerLevel);
		Updatetmp.iDefense += Itemtmp.iDefense;
		Updatetmp.iMaxHP += Itemtmp.iMaxHP + (Level * Itemtmp.iMaxHPPerLevel);
		Updatetmp.fHPRegen += Itemtmp.fHPRegen + (Level * Itemtmp.fHPRegenPerLevel);
		Updatetmp.iMaxSP += Itemtmp.iMaxSP + (Level * Itemtmp.iMaxSPPerLevel);
		Updatetmp.fSPRegen += Itemtmp.fSPRegen + (Level * Itemtmp.fSPRegenPerLevel);
		Updatetmp.fAttackSpeed += Itemtmp.fAttackSpeed;
		Updatetmp.fCriticalStrikeChance += Itemtmp.fCriticalStrikeChance;
		Updatetmp.fCriticalStrikeDamage += Itemtmp.fCriticalStrikeDamage;
		Updatetmp.fMovementSpeed += Itemtmp.fMovementSpeed;
		Updatetmp.fVisionRange += Itemtmp.fVisionRange;
		Updatetmp.fCooldownReduction += Itemtmp.fCooldownReduction;
		Updatetmp.fOmnisyphon += Itemtmp.fOmnisyphon;
		Updatetmp.iSkillAmplification += Itemtmp.iSkillAmplification + (Level * Itemtmp.iSkillAmplificationPerLevel);
	}

	// 최종스탯 반영
	*m_Stats = Updatetmp;
}

void ER_DataScript_Character::HPRegen(float _magnification)
{
	// 회복량 회복 후 HP값
	float HPRegen = m_Stats->iHP + (m_Stats->fHPRegen * _magnification);

	// HP 자연 회복, 최대 HP면 최대HP로 고정
	m_Stats->iHP = m_Stats->iMaxHP < (int)HPRegen ? m_Stats->iMaxHP : (int)HPRegen;
}

void ER_DataScript_Character::SPRegen(float _magnification)
{
	// 회복량 회복 후 HP값
	float SPRegen = m_Stats->iSP + (m_Stats->fSPRegen * _magnification);

	// HP 자연 회복, 최대 HP면 최대HP로 고정
	m_Stats->iSP = m_Stats->iMaxSP < (int)SPRegen ? m_Stats->iMaxSP : (int)SPRegen;
}

void ER_DataScript_Character::LevelUP()
{
	// 최대레벨 18 이하인 경우
	if (m_Stats->iLevel < 18)
	{
		m_Stats->iLevel++;	// Level 1 증가
		m_SkillPoint++;		// 스킬 포인트 1 증가
	}

	// [ 이펙트 ]
	// 레벨업 이펙트 및 애니메이션 재생

	// 레벨업 효과음 재생
	// 플레이어 캐리터 화면 안에 들어와있을때만 재생
	{
		Vec3 PlayerPos = ER_GameSystem::GetInst()->GetPlayerCharacter()->Transform()->GetRelativePos();
		Vec3 CharacterPos = Transform()->GetRelativePos();
		float ListenDist = Vec3::Distance(PlayerPos, CharacterPos);

		if (ListenDist < 18.f)
			m_LevelUpSound->Play(1, 0.5, true);
	}

	// 레벨업 갱신 전 데이터
	int LevelUpDefHP = m_Stats->iMaxHP;
	int LevelUpDefSP = m_Stats->iMaxSP;

	// [ 스테이터스 최종 반영 ]
	StatusUpdate();

	// 갱신 후 최대 HP/SP 증가한 차이값만큼 현재 HP / SP에 반영해 회복해준다.

	LevelUpDefHP = m_Stats->iMaxHP - LevelUpDefHP;
	LevelUpDefSP = m_Stats->iMaxSP - LevelUpDefSP;

	m_Stats->iHP += LevelUpDefHP;
	m_Stats->iSP += LevelUpDefSP;
}

void ER_DataScript_Character::init()
{
	// [텍스쳐 로딩]
	if (0 < m_strKey.length())
	{
		wstring PortraitTexKey = L"Char_Portrait_";		// portrait
		wstring FullTexKey = L"Char_Full_";				// Full
		wstring MapTexKey = L"Char_Map_";				// map

		PortraitTexKey += m_strKey + L".png";
		FullTexKey += m_strKey + L".png";
		MapTexKey += m_strKey + L".png";

		m_PortraitTex = CResMgr::GetInst()->FindRes<CTexture>(PortraitTexKey);
		m_FullTax = CResMgr::GetInst()->FindRes<CTexture>(FullTexKey);
		m_MapTex = CResMgr::GetInst()->FindRes<CTexture>(MapTexKey);
	}

	for (int i = 0; i < 5; ++i)
	{
		ER_ItemMgr::GetInst()->GetIngredient(m_RootItem[i], &m_IngredientList, &m_NeedFarmingItems);
	}
}

void ER_DataScript_Character::begin()
{
	// 캐릭터 초기능력치를 받아와 레벨 1로 초기화
	m_Stats->Init_To_LevelOne(m_STDStats);

	int a = 0;
	if (GetOwner()->GetRenderComponent() != nullptr && GetOwner()->GetRenderComponent()->GetMaterial(0) != nullptr)
	{
		GetOwner()->GetRenderComponent()->GetMaterial(0)->SetScalarParam(INT_3, &a);
	}

	StatusUpdate();

	UINT StartWeapon = 0;
	if (m_strKey == L"Aya")
	{
		StartWeapon = 20;
		m_SkillPoint = 1;
	}
	else if (m_strKey == L"Hyunwoo")
	{
		StartWeapon = 18;
		m_SkillPoint = 1;
	}
	else if (m_strKey == L"Jackie")
	{
		StartWeapon = 108;
		m_SkillPoint = 1;
	}
	else if (m_strKey == L"Yuki")
	{
		StartWeapon = 9;
		m_SkillPoint = 1;
	}
	else if (m_strKey == L"Rio")
	{
		m_SkillList[(UINT)SKILLIDX::Q_1]->iSkillLevel = 1;
		m_SkillList[(UINT)SKILLIDX::Q_2]->iSkillLevel = 1;
		StartWeapon = 28;
	}

	m_Equipment[0] = ER_ItemMgr::GetInst()->GetItemObj(StartWeapon)->Clone();
	m_Equipment[0]->GetScript<ER_DataScript_Item>()->m_bEquiped = true;
}

void ER_DataScript_Character::tick()
{
	// [ 스테이터스 갱신 ]
	// 경험치 확인 및 레벨업, 현재 경험치가 레벨업 필요 경험치를 넘은경우 반복한다.
	while (m_Stats->iNeedExpForNextLevel <= m_Stats->iExp)
	{
		// 레벨업후, 초과한 잔여 경험치를 현재 Exp에 갱신해준다.
		m_Stats->iExp = m_Stats->iExp - m_Stats->iNeedExpForNextLevel;

		// 레벨업 함수 호출
		LevelUP();
	}

	// SP 자연 회복
	m_fSPRegenTime += DT;
	if (0.5f <= m_fSPRegenTime)
	{
		SPRegen();
		m_fSPRegenTime -= 0.5f;
	}


	//  [ 버프디버프 쿨타임 갱신 ]
	m_StatusEffect->ActionTiemUpdate(DT);


	// [ 스킬 쿨타임 갱신 ]
	float CoolDownRatio = DT + (DT * m_Stats->fCooldownReduction);

	// Developer Func
	// [CoolTime Delete Mode]
	if (KEY_TAP(KEY::F3))
	{
		bCoolDownCheat = !bCoolDownCheat;
	}

	if (bCoolDownCheat)
	{
		m_Stats->iSP = m_Stats->iMaxSP;
		CoolDownRatio = 500.f;
	}

	// [EXP Increase Key]
	if (KEY_TAP(KEY::F2))
	{
		m_Stats->iExp += 100; // 레벨업 필요 경험치 100
	}

	// [ Invincible Mode ]
	if (KEY_TAP(KEY::F4))
	{
		ER_PlayerScript* IsPlayer = GetOwner()->GetScript<ER_PlayerScript>();
		
		if (nullptr != IsPlayer)
		{
			bInvincibleCheat = !bInvincibleCheat;
		}
	}

	if (bInvincibleCheat)
		m_Stats->iHP = m_Stats->iMaxHP;

	for (int i = 0; i < (UINT)SKILLIDX::SKILLMAXSIZE; ++i)
		m_SkillList[i]->SkillStatusUpdate(CoolDownRatio);
};

bool ER_DataScript_Character::SwapItem(CGameObject** _DragItem, CGameObject** _DropItem)
{
	ER_DataScript_Item* DragItem = nullptr;
	int DragItemType = -1;
	bool DragItemEquiped = false;

	ER_DataScript_Item* DropItem = nullptr;
	int DropItemType = -1;
	bool DropItemEquiped = false;

	if (*_DragItem)
	{
		DragItem = (*_DragItem)->GetScript<ER_DataScript_Item>();
		DragItemType = DragItem->m_eSlot;
		DragItemEquiped = DragItem->m_bEquiped;
	}
	if (*_DropItem)
	{
		DropItem = (*_DropItem)->GetScript<ER_DataScript_Item>();
		DropItemType = DropItem->m_eSlot;
		DropItemEquiped = DropItem->m_bEquiped;
	}

	// 장비창 -> 인벤토리
	// 1. 빈칸으로 이동하는경우
	// 그냥 이동, 이동후 아이템 슬롯타입 2로교체
	// 2. 아이템과 교체 하는경우
	// 아이템타입 비교후 교체, 서로 슬롯타입 교체

	// 인벤토리 -> 장비창
	// 1. 빈칸으로 이동하는 경우
	// 아이템 타입 확인, 목적지 주소와 장비슬롯 주소가 같은지 확인
	// 2. 아이템과 교체하는 경우
	// 아이템타입 비교후 교체, 서로 슬롯타입 교체

	// 인벤토리orItemBox 끼리
	// 그냥 교체

	// 장비창 ->
	if (DragItemEquiped && !DropItemEquiped)
	{
		// 장착 슬롯이 같은 경우
		if (DragItemType == DropItemType)
		{
			// 포인터 교환
			CGameObject* tmp = (*_DropItem);
			(*_DropItem) = (*_DragItem);
			(*_DragItem) = tmp;

			// 장착여부 변경
			(*_DragItem)->GetScript<ER_DataScript_Item>()->m_bEquiped = true;
			(*_DropItem)->GetScript<ER_DataScript_Item>()->m_bEquiped = false;

			ItemInfoUpdate();
			StatusUpdate();
		}
		// -> NULLSlot
		else if (DropItemType == -1)
		{
			// -> 장비창 : 다른 슬롯타입의 장비창이므로 옮겨지지 않는다.
			for (int i = 0; i < 5; ++i)
			{
				// 장비창 인덱스 확인
				if (_DropItem == &m_Equipment[i])
				{
					return false;
				}
			}

			// 포인터 교환
			CGameObject* tmp = (*_DropItem);
			(*_DropItem) = (*_DragItem);
			(*_DragItem) = tmp;

			(*_DropItem)->GetScript<ER_DataScript_Item>()->m_bEquiped = false;

			ItemInfoUpdate();
			StatusUpdate();
		}
	}
	// 인벤토리 or ItemBox ->
	else
	{
		// -> 장비창
		for (int i = 0; i < 5; ++i)
		{
			// 장비창 인덱스 확인
			if (_DropItem == &m_Equipment[i])
			{
				// 아이템타입과 슬롯이 일치한다
				if (DragItemType == i)
				{
					// 슬롯이 비어있다
					if (DropItemType == -1)
					{
						CGameObject* tmp = (*_DropItem);
						(*_DropItem) = (*_DragItem);
						(*_DragItem) = tmp;

						(*_DropItem)->GetScript<ER_DataScript_Item>()->m_bEquiped = true;

						ItemInfoUpdate();
						StatusUpdate();
						return true;
					}
					else if (DragItemType == DropItemType)
					{
						// 포인터 교환
						CGameObject* tmp = (*_DropItem);
						(*_DropItem) = (*_DragItem);
						(*_DragItem) = tmp;

						// 장착여부 변경
						(*_DragItem)->GetScript<ER_DataScript_Item>()->m_bEquiped = false;
						(*_DropItem)->GetScript<ER_DataScript_Item>()->m_bEquiped = true;

						ItemInfoUpdate();
						StatusUpdate();
						return true;
					}
				}
				return false;
			}
		}

		// -> 인벤토리 or 아이템 박스

		// Inventory에 있는 아이템이면 
		// Drag가 ItemSlot이 아닌경우
		CGameObject* tmp = (*_DropItem);
		(*_DropItem) = (*_DragItem);
		(*_DragItem) = tmp;

		ItemInfoUpdate();
	}

	return false;
}

void ER_DataScript_Character::AcquireItem(CGameObject** _BoxSlot)
{
	// 아이텝슬롯에 아이템이 있는지 여부
	if ((*_BoxSlot))
	{
		int Slotidx = -1;
		int slottype = 0;

		// 아이템 스크립트 보유 확인 예외처리
		ER_DataScript_Item* Item = (*_BoxSlot)->GetScript<ER_DataScript_Item>();

		// ItemScript를 보유하고있지 않다 - 함수종료
		if (!Item)
			return;

		int emptyslot = -1;
		// 인벤토리 여유공간 확인
		for (int i = 0; i < 10; ++i)
		{
			if (nullptr == m_Inventory[i])
			{
				emptyslot = i;
				break;
			}
		}

		// 인벤토리에 여유공간이 없다. - 함수종료
		if (emptyslot == -1)
			return;

		// 비어있는 인벤토리에 넣어주고, 원래 슬롯을 비워준다.
		m_Inventory[emptyslot] = (*_BoxSlot);
		m_Inventory[emptyslot]->GetScript<ER_DataScript_Item>()->m_bEquiped = false;
		*_BoxSlot = nullptr;

		int ItemID = m_Inventory[emptyslot]->GetScript<ER_DataScript_Item>()->GetCode();

		// 습득한 아이템이 필요 파밍아이템이었다면 리스트를 갱신해준다
		unordered_map<UINT, int>::iterator iter = m_NeedFarmingItems.find(ItemID);
		
		if (iter != m_NeedFarmingItems.end())
		{
			if (iter->second == 1)
				m_NeedFarmingItems.erase(iter);
			else
				iter->second--;
		}

		StatusUpdate();
		ItemInfoUpdate();
	}
}

void ER_DataScript_Character::ItemInfoUpdate()
{
	std::thread t1(&ER_DataScript_Character::ItemInfoUpdate, ItemInfoUpdate);
	t1.detach();

	lock_guard<mutex> lockGuard(m_mMutex);

	
	// 제작가능 아이템 업데이트
	vector<UINT> itemlist;

	for (int i = 0; i < 10; ++i)
	{
		if (i < 5 && m_Equipment[i])
			itemlist.push_back(m_Equipment[i]->GetScript<ER_DataScript_Item>()->m_eItemCode);

		if (m_Inventory[i])
			itemlist.push_back(m_Inventory[i]->GetScript<ER_DataScript_Item>()->m_eItemCode);
	}

	m_CraftList.clear();

	for (int Litem = 0; Litem < itemlist.size(); ++Litem)
	{
		for (int Ritem = Litem + 1; Ritem < itemlist.size(); ++Ritem)
		{
			int tmp = -1;

			// 제작가능한 아이템이다.
			if (S_OK == ER_ItemMgr::GetInst()->SearchRecipe(itemlist[Litem], itemlist[Ritem], tmp))
			{
				// 해당아이템이 제작필요 목록에 있는지 여부
				if (m_IngredientList.end() != m_IngredientList.find(tmp))
					m_CraftList.push_back(tmp);
			}
		}
	}
}

bool ER_DataScript_Character::CraftItem(UINT _Item)
{
	// 해당 아이템을 만들기위한 재료 슬롯 검색
	ER_RECIPE recipe = {};
	recipe.recipe = ER_ItemMgr::GetInst()->m_umapIngredient.find(_Item)->second;

	CGameObject** ItemSlot1 = nullptr;
	CGameObject** ItemSlot2 = nullptr;

	for (int i = 0; i < 10; ++i)
	{
		// 2개 아이템을 다 찾은경우 탐색 종료
		if (ItemSlot1 && ItemSlot2)
			break;

		// 장비창에 아이템이 있는 경우
		if (i < 5 && m_Equipment[i])
		{
			UINT Itemid = m_Equipment[i]->GetScript<ER_DataScript_Item>()->GetCode();

			if (!ItemSlot1 && Itemid == recipe.ingredient_1)
				ItemSlot1 = &m_Equipment[i];
			else if (!ItemSlot2 && Itemid == recipe.ingredient_2)
				ItemSlot2 = &m_Equipment[i];
		}

		// 인벤토리에 아이템이 있는경우
		if (m_Inventory[i])
		{
			UINT Itemid = m_Inventory[i]->GetScript<ER_DataScript_Item>()->GetCode();

			if (!ItemSlot1 && Itemid == recipe.ingredient_1)
				ItemSlot1 = &m_Inventory[i];
			else if (!ItemSlot2 && Itemid == recipe.ingredient_2)
				ItemSlot2 = &m_Inventory[i];
		}
	}

	// 기존 두 재료 슬롯의 아이템 delete
	// odelete(*ItemSlot1);
	// (*ItemSlot1) = nullptr;
	// odelete (*ItemSlot2);
	// (*ItemSlot2) = nullptr;

	delete* ItemSlot1;
	(*ItemSlot1) = nullptr;
	delete* ItemSlot2;
	(*ItemSlot2) = nullptr;

	// 제작한 아이템 매니저에서 클론으로 가져온다.
	CGameObject* NewItem = ER_ItemMgr::GetInst()->GetItemObj(_Item)->Clone();

	int InventoryIdx = -1;
	// 비어있는 인벤토리에 아이템을 넣는다.
	for (int i = 0; i < 10; ++i)
	{
		if (nullptr == m_Inventory[i])
		{
			m_Inventory[i] = NewItem;
			InventoryIdx = i;
			break;
		}
	}

	// 아이템슬롯타입을 확인하고 해당 슬롯이 비어있거나 등급이 더 높은경우 SwapItem을 진행한다.
	ER_DataScript_Item* NewItemInfo = NewItem->GetScript<ER_DataScript_Item>();
	if (!m_Equipment[NewItemInfo->GetSlot()] || m_Equipment[NewItemInfo->GetSlot()]->GetScript<ER_DataScript_Item>()->GetGrade() < NewItemInfo->GetGrade())
	{
		SwapItem(&m_Inventory[InventoryIdx], &m_Equipment[NewItemInfo->GetSlot()]);
	}


	// 제작한 아이템이 필요 제작 목록에 있는경우 카운트를 내리고 0이된경우 지워준다.
	unordered_map<UINT, int>::iterator iter = m_IngredientList.find(_Item);
	if (iter->second == 1)
		m_IngredientList.erase(iter);
	else
		iter->second--;

	ItemInfoUpdate();

	// 아이템을 제작했다면 제작한 아이템의 등급에 따라 경험치 증가
	switch (NewItemInfo->GetGrade())
	{
	case (UINT)ER_ITEM_GRADE::UNCOMMON:
		m_Stats->iExp += 40;
		break;
	case (UINT)ER_ITEM_GRADE::RARE:
		m_Stats->iExp += 60;
		break;
	case (UINT)ER_ITEM_GRADE::EPIC:
		m_Stats->iExp += 80;
		break;
	}

	return true;
}

void ER_DataScript_Character::BeginOverlap(CCollider3D* _Other)
{

}

void ER_DataScript_Character::OnOverlap(CCollider3D* _Other)
{
}

void ER_DataScript_Character::EndOverlap(CCollider3D* _Other)
{
}

void ER_DataScript_Character::BeginRayOverlap()
{
	int a = 3;
	if (GetOwner()->GetRenderComponent() != nullptr && GetOwner()->GetRenderComponent()->GetMaterial(0) != nullptr)
	{
		GetOwner()->GetRenderComponent()->GetMaterial(0)->SetScalarParam(INT_3, &a);
	}
}

void ER_DataScript_Character::OnRayOverlap()
{
}

void ER_DataScript_Character::EndRayOverlap()
{
	int a = 0;
	if (GetOwner()->GetRenderComponent() != nullptr && GetOwner()->GetRenderComponent()->GetMaterial(0) != nullptr)
	{
		GetOwner()->GetRenderComponent()->GetMaterial(0)->SetScalarParam(INT_3, &a);
	}
}

void ER_DataScript_Character::SaveToLevelFile(FILE* _File)
{
	SaveWString(m_strKey, _File);
	SaveWString(m_strName, _File);
	fwrite(&m_STDStats, sizeof(ER_Initial_Stats), 1, _File);
	SaveResRef(m_PortraitTex.Get(), _File);
	SaveResRef(m_FullTax.Get(), _File);
	SaveResRef(m_MapTex.Get(), _File);

	int SkillSize = (int)m_SkillList.size();
	fwrite(&SkillSize, sizeof(int), 1, _File);

	for (int i = 0; i < SkillSize; ++i)
	{
		m_SkillList[i]->Save(_File);
		SaveResRef(m_SkillList[i]->TexSkill.Get(), _File);
	}
}

void ER_DataScript_Character::LoadFromLevelFile(FILE* _File)
{
	LoadWString(m_strKey, _File);
	LoadWString(m_strName, _File);
	fread(&m_STDStats, sizeof(ER_Initial_Stats), 1, _File);
	LoadResRef(m_PortraitTex, _File);
	LoadResRef(m_FullTax, _File);
	LoadResRef(m_MapTex, _File);

	int SkillSize = (int)m_SkillList.size();
	fread(&SkillSize, sizeof(int), 1, _File);

	for (int i = 0; i < SkillSize; ++i)
	{
		ER_SKILL* Skill = onew(ER_SKILL);
		Skill->Load(_File);
		LoadResRef(Skill->TexSkill, _File);

		m_SkillList.push_back(Skill);
	}
}
#include "pch.h"
#include "ER_DataScript_Character.h"
#include "ER_DataScript_Item.h"
#include "ER_struct.h"
#include <FontEngine\FW1FontWrapper.h>
#include "ER_DataScript_Item.h"
#include "ER_ItemMgr.h"

ER_DataScript_Character::ER_DataScript_Character()
	: CScript((UINT)SCRIPT_TYPE::ER_DATASCRIPT_CHARACTER)
	, m_Stats(nullptr)
	, m_StatusEffect(nullptr)
	, m_Skill{}
	, m_bGameDead(false)
	, m_bOutofContorl(false)
	, m_Equipment{}
	, m_Inventory{}
	, m_bHPChangeTrigger(false)
	, m_fSPRegenTime(0.f)
	, m_SkillPoint(0)
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
	, m_Skill{}
	, m_bGameDead(false)
	, m_bOutofContorl(false)
	, m_Equipment{}
	, m_Inventory{}
	, CScript((UINT)SCRIPT_TYPE::ER_DATASCRIPT_CHARACTER)
	, m_bHPChangeTrigger(false)
	, m_fSPRegenTime(0.f)
	, m_SkillPoint(0)
{
	m_Stats = new tIngame_Stats;
	m_StatusEffect = new tStatus_Effect;

	for (int i = 0; i < _origin.m_SkillList.size(); ++i)
	{
		tSkill_Info* tmp = new tSkill_Info;
		*tmp = *_origin.m_SkillList[i];

		m_SkillList.push_back(tmp);
	}

	m_Skill[(UINT)SKILLIDX::E_1] = m_SkillList[(UINT)SKILLIDX::E_1];
	m_Skill[(UINT)SKILLIDX::W_1] = m_SkillList[(UINT)SKILLIDX::W_1];
	m_Skill[(UINT)SKILLIDX::E_1] = m_SkillList[(UINT)SKILLIDX::E_1];
	m_Skill[(UINT)SKILLIDX::R_1] = m_SkillList[(UINT)SKILLIDX::R_1];
}

ER_DataScript_Character::~ER_DataScript_Character()
{
	if (m_Stats)
		delete m_Stats;

	if (m_StatusEffect)
		delete m_StatusEffect;

	Safe_Del_Vec(m_SkillList);
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
	m_Stats->iHP = m_Stats->iMaxHP < HPRegen ?	m_Stats->iMaxHP : HPRegen;
}

void ER_DataScript_Character::SPRegen(float _magnification)
{
	// 회복량 회복 후 HP값
	float SPRegen = m_Stats->iSP + (m_Stats->fSPRegen * _magnification);

	// HP 자연 회복, 최대 HP면 최대HP로 고정
	m_Stats->iSP = m_Stats->iMaxSP < SPRegen ? m_Stats->iMaxSP : SPRegen;
}

void ER_DataScript_Character::LevelUP()
{
	// IngameState level 변수 1 증가
	// Exp 0으로 초기화
	// Skill투자 가능포인트 1 증가

	// [ 이펙트 ]
	// 레벨업 이펙트 및 애니메이션 재생
	// 레벨업 효과음 재생

	// [ 스테이터스 최종 반영 ]
	// StatusUpdate
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

	SkillSlotInit();

	m_Equipment[0] = ER_ItemMgr::GetInst()->GetItemObj(87)->Clone();
	m_Inventory[0] = ER_ItemMgr::GetInst()->GetItemObj(58)->Clone();
}

void ER_DataScript_Character::tick()
{
	// 스킬 쿨타임 갱신
	float CoolDownRatio = DT + (DT * m_Stats->fCooldownReduction);
	for (int i = 0; i < (UINT)SKILLIDX::SKILLMAXSIZE; ++i)
		m_SkillList[i]->SkillStatusUpdate(CoolDownRatio);
	
	// 버프디버프 쿨타임 갱신
	m_StatusEffect->ActionTiemUpdate(DT);

	// SPRegen
	m_fSPRegenTime += DT;
	if (0.5f <= m_fSPRegenTime)
	{
		SPRegen();
		m_fSPRegenTime -= 0.5f;
	}

	if (KEY_TAP(KEY::_5))
	{
		m_SkillPoint++;
	}

}

void ER_DataScript_Character::ChangeSkill(int _Idx)
{

}

void ER_DataScript_Character::SkillSlotInit()
{
	m_Skill[(UINT)SKILLIDX::Q_1] = m_SkillList[(UINT)SKILLIDX::Q_1];
	m_Skill[(UINT)SKILLIDX::W_1] = m_SkillList[(UINT)SKILLIDX::W_1];
	m_Skill[(UINT)SKILLIDX::E_1] = m_SkillList[(UINT)SKILLIDX::E_1];
	m_Skill[(UINT)SKILLIDX::R_1] = m_SkillList[(UINT)SKILLIDX::R_1];
}

CGameObject* ER_DataScript_Character::ItemAcquisition(CGameObject* _ItemObj)
{
	int i = 0;
	//비어있는 인벤 찾음
	while (i < 10) {
		if (!m_Inventory[i])
			break;
		++i;
	}

	if (i == 10)
		return nullptr;

	m_Inventory[i] = _ItemObj;

	return _ItemObj;
}

bool ER_DataScript_Character::SwapItem(CGameObject* _DragmItem, CGameObject* _DropItem)
{
	return false;
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
	fwrite(&m_STDStats,sizeof(ER_Initial_Stats),1,_File);
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
		ER_SKILL* Skill = new ER_SKILL;
		Skill->Load(_File);
		LoadResRef(Skill->TexSkill, _File);

		m_SkillList.push_back(Skill);
	}

	m_Skill[(UINT)SKILLIDX::E_1] = m_SkillList[(UINT)SKILLIDX::E_1];
	m_Skill[(UINT)SKILLIDX::W_1] = m_SkillList[(UINT)SKILLIDX::W_1];
	m_Skill[(UINT)SKILLIDX::E_1] = m_SkillList[(UINT)SKILLIDX::E_1];
	m_Skill[(UINT)SKILLIDX::R_1] = m_SkillList[(UINT)SKILLIDX::R_1];
}
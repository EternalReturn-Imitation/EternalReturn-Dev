#include "pch.h"
#include "ER_BattleSystem.h"

#include "ER_DataScript_Character.h"
#include "ER_ActionScript_Character.h"

//테스트용
#include "ER_GameSystem.h"

ER_BattleSystem::ER_BattleSystem()
{
}

ER_BattleSystem::~ER_BattleSystem()
{
}


void ER_BattleSystem::init()
{
}

void ER_BattleSystem::Battle_Common(CGameObject* AtkObj, CGameObject* HitObj)
{
	tIngame_Stats* AtkInfo = GetStatsInfo(AtkObj);
	tIngame_Stats* HitInfo = GetStatsInfo(HitObj);

	// 공격력 ( 공격력 *100 / ( 100 + 대상방어력) * ( 1+ 치명타여부 0.65)
	
	tStatus_Effect* ATKStatusEfc = AtkObj->GetScript<ER_DataScript_Character>()->GetStatusEffect();
	tStatus_Effect* HitStatusEfc = HitObj->GetScript<ER_DataScript_Character>()->GetStatusEffect();

	int AtkPower = AtkInfo->iAttackPower + ATKStatusEfc->GetIncATK() - ATKStatusEfc->GetDecATK();
	int Def = HitInfo->iDefense + HitStatusEfc->GetIncDEF() - HitStatusEfc->GetDecDEF();

	int CriticalDmg = IsCritical(AtkInfo);

	int FinalDmg = (int)(((AtkPower * 100) / (100 + Def)) + (CriticalDmg * 0.65));

	// 사망처리
	if (HitInfo->iHP - FinalDmg <= 0)
	{
		// 타겟의 HP를 0으로 만든다.
		HitInfo->iHP = 0;

		tFSMData deaddata = {};
		HitObj->GetScript<ER_ActionScript_Character>()->Dead(deaddata);
		return;
	}
	HitInfo->iHP -= FinalDmg;

	// 데미지폰트 출력
}

void ER_BattleSystem::Battle_Skill(CGameObject* _Attacker
	, CGameObject* _HitObj
	, CScript* _Inst
	, SKILL_DMG_CALC _calc)
{
	tIngame_Stats* AtkInfo = GetStatsInfo(_Attacker);
	tIngame_Stats* HitInfo = GetStatsInfo(_HitObj);

	// 공격력 ( 공격력 *100 / ( 100 + 대상방어력) * ( 1+ 치명타여부 0.65)

	tStatus_Effect* ATKStatusEfc = _Attacker->GetScript<ER_DataScript_Character>()->GetStatusEffect();
	tStatus_Effect* HitStatusEfc = _HitObj->GetScript<ER_DataScript_Character>()->GetStatusEffect();

	int SkillDmg = (_Inst->*_calc)();
	SkillDmg += SkillDmg * AtkInfo->iSkillAmplification;
	int Def = HitInfo->iDefense + HitStatusEfc->GetIncDEF() - HitStatusEfc->GetDecDEF();

	int CriticalDmg = IsCritical(AtkInfo);

	int FinalDmg = (int)(((SkillDmg * 100) / (100 + Def)) + (CriticalDmg * 0.65));

	// 사망처리
	if (HitInfo->iHP - FinalDmg <= 0)
	{
		// 타겟의 HP를 0으로 만든다.
		HitInfo->iHP = 0;

		tFSMData deaddata = {};
		_HitObj->GetScript<ER_ActionScript_Character>()->Dead(deaddata);
		return;
	}
	HitInfo->iHP -= FinalDmg;

	// 데미지폰트 출력
}


int ER_BattleSystem::IsCritical(tIngame_Stats* _info)
{
	// 난수생성 초기화
	std::srand(static_cast<unsigned int>(std::time(nullptr)));

	int CriChance	= (int)_info->fCriticalStrikeChance * 100;
	int Probability = std::rand() % 100;

	// 크리티컬 발생
	if (Probability < CriChance)
		return 1;

	return 0;
}

tIngame_Stats* ER_BattleSystem::GetStatsInfo(CGameObject* _obj)
{
	int ObjLayer = _obj->GetLayerIndex();

	tIngame_Stats* Info = nullptr;

	switch (ObjLayer)
	{
	case 11:	// Monster
	{
		// Monster Data Script에서 얻어오기
		break;
	}
	case 12:	// Character
	case 13:	// Player
	{
		Info = _obj->GetScript<ER_DataScript_Character>()->GetStatus();
		break;
	}
	}
	
	return Info;
}